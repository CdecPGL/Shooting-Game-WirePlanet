//簡易アーカイバー
/*Version1.0 2014/09/06
Version2.0 2014/09/06 圧縮に対応。簡易暗号化に対応
*/
#include"Archiver.h"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include"boost/lexical_cast.hpp"
#include<climits>

const std::string VERSION("2.0");
const std::string DATE("2014/09/07");
const std::string LOG_FILE("easy_archiver_log.txt"); //ログファイルパス
const std::string DEFAULT_OUT_PATH("easy_archiver_out.arc"); //デフォルト出力先

//データを読み込む
int LoadData(const std::string&, void**, unsigned int*);

int main(int argc,char* argv[]){
	namespace bfs = boost::filesystem;
	std::vector<bfs::path> file_paths;
	bfs::path out_path(DEFAULT_OUT_PATH);
	ArcLib::Mode mode = ArcLib::Mode::SimpleBind;
	unsigned int com_level=6; //圧縮レベル(デフォルトはzlibのデフォルト値6)
	unsigned int key = 0; //復号化キー
	//ログファイルを開く
	std::ofstream ofs(LOG_FILE,std::ios::trunc);
	ofs << "EasyArchiver " << VERSION << " " << DATE << std::endl;
	//コマンドライン引数解析
	for (int i = 1; i < argc; ++i){
		//オプション指定しだったら
		if (argv[i][0] == '-'){
			std::string str(argv[i]);
			switch (str[1]){
				//出力先設定
			case 'O':
			{
				bfs::path op(str.substr(2, str.size() - 2));
				if (op.has_filename()){
					out_path = op;
				}
				else{
					ofs << "出力先ファイルが不正です。:" << str.substr(2, str.size() - 2) << std::endl;
				}
			}
				break;
			case 'M':
				try{
					mode = ArcLib::ConvertUnsignedIntToMode(boost::lexical_cast<unsigned int>(str.substr(2, str.size() - 2)));
				}
				catch (boost::bad_lexical_cast& e){
					ofs << "出力モードが不正です。:" << str.substr(2, str.size() - 2) << std::endl;
				}
				break;
			case 'C':
				try{
					com_level = boost::lexical_cast<unsigned int>(str.substr(2, str.size() - 2));
				}
				catch (boost::bad_lexical_cast& e){
					ofs << "圧縮レベルが不正です。:" << str.substr(2, str.size() - 2) << std::endl;
				}
				break;
			case 'K':
				try{
					key = boost::lexical_cast<unsigned int>(str.substr(2, str.size() - 2));
				}
				catch (boost::bad_lexical_cast& e){
					ofs << "キーが不正です。:" << str.substr(2, str.size() - 2) << std::endl;
				}
				break;
			default:
				ofs << "無効な識別子です。:" << str << std::endl;
				break;
			}
		}
		//ファイルパスだったら
		else{
			bfs::path pth(argv[i]);
			//ディレクトリだったら
			if (bfs::is_directory(pth)){
				BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(pth),
					bfs::recursive_directory_iterator())) {
					if (!bfs::is_directory(p)){
						file_paths.push_back(p);
					}
				}
			}
			//ファイルだったら
			else if (bfs::exists(pth)){
				file_paths.push_back(pth);
			}
			else{
				ofs << "入力パスが不正です。:" << pth << std::endl;
			}
		}
	}

	ofs << "対象ファイル" << std::endl;
	for (auto& in_file : file_paths){
		ofs << in_file << std::endl;
	}

	ofs << "アーカイブファイルを作成します。入力ファイル数:" << file_paths.size() << ",出力先:" << out_path << ",モード:" << ArcLib::ConvertModeToString(mode) << ",圧縮レベル:" << com_level << std::endl;

	//アーカイブ作成
	Archiver arcr;
	if (mode == ArcLib::Mode::EXOREncrypt){
		if ((int)std::numeric_limits<uint16_t>::max() < key){
			ofs << "暗号化キーが大きすぎます。:" << key << ",最大値:" << std::numeric_limits<uint16_t>::max() << std::endl;
			return -1;
		}
		ofs << "暗号化キー:" << key << std::endl;
		arcr.SetEXOREncryptionKey((uint16_t)key);
	}
	if (arcr.BeginArchive(out_path.string(),mode,com_level) < 0){
		ofs << "アーカイブの開始に失敗しました。" << std::endl;
	}

	for (auto& fn : file_paths){
		unsigned char* ptr;
		unsigned int size;
		int res = LoadData(fn.string(), (void**)&ptr, &size);
		if (res < 0){
			ofs << "ファイルの読み込みでエラーが発生しました。:" << res << "," << fn << std::endl;
			continue;
		}
		arcr.InsertData(fn.filename().string(), ptr, size);
		delete[] ptr;
	}

	int res = arcr.EndArchive();
	if (res < 0){
		ofs << "アーカイブファイルの作成に失敗しました。:" << res<<std::endl;
	}
	else{
		ofs << "アーカイブファイルを作成しました。サイズ" << bfs::file_size(out_path) << "Bytes" << std::endl;
	}

	//ログ出力先を閉じる
	ofs.close();
	return 0;
}

int LoadData(const std::string& name, void** ptr, unsigned int* size){
	std::ifstream ifs(name, std::ios::binary);
	if (!ifs){ return -1; }
	//サイズ取得
	ifs.seekg(0, std::ios::end);
	unsigned int nsize = (int)ifs.tellg();
	//メモリ確保
	unsigned char* nptr = new unsigned char[nsize];
	if (ptr == nullptr){
		ifs.close();
		return -2;
	}
	//読み込み
	ifs.seekg(0, std::ios::beg);
	ifs.read((char*)nptr, sizeof(unsigned char)*nsize);
	ifs.close();

	*ptr = nptr;
	*size = nsize;
	return 0;
}
