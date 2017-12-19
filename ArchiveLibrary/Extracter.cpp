#include"Extracter.h"
#include"zlib_cdec.h"
#include"EXORenckey.h"

using namespace std;
using namespace ArcLib;

Extracter::Extracter():_exor_enc_key(make_unique<EXOREncKey>()),_extracting(false){}

Extracter::~Extracter(){}

int Extracter::OpenAchiveFile(const string& fn){
	if (_extracting){ return -10; }
	_ifs.open(fn, ios::binary);
	if (!_ifs){ return -1; }
	if (_ifs.fail()){ return -2; }
	_header.clear();
	//ヘッダー読み込み
	//モード取得
	unsigned char m;
	_ifs.read((char*)&m, 1);
	_mode = ConvertUnsignedIntToMode(m);
	_header.header_size = 1 + 4; //ヘッダのサイズ計算
	//データ部サイズ取得
	_Uint32t d_size;
	_ifs.read((char*)&d_size, sizeof(d_size));
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ExorEncryptData(&d_size, sizeof(d_size));
	}
	//フッター読み込み
	//データ数取得
	_ifs.seekg(d_size,ios::cur); //フッタまで移動(現在位置[ヘッダーの末尾]からデータサイズ分だけシーク)
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->SeekCurrentExorKey(d_size);
	}
	unsigned int f_top_pos = (unsigned int)_ifs.tellg(); //フッタの先頭位置記録
	//データ数を取得
	_ifs.read((char*)&_header.num, sizeof(_header.num));
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ExorEncryptData((char*)&_header.num, sizeof(_header.num));
	}
	//保存フッターサイズを取得
	_ifs.seekg(0, ios::end); //ファイル末尾まで移動し
	unsigned int _prc_f_size = (unsigned int)_ifs.tellg() - (f_top_pos + 4); //ファイル末尾の位置からフッダーの先頭位置とデータ数保存部のサイズを引く
	_ifs.seekg(f_top_pos + 4); //フッダーのデータ数保存部の次に移動
	//フッターサイズ求める
	unsigned int _footer_size = (NAME_SIZE + 4 + 4 + 4)*_header.num; //展開後の圧縮対象フッターサイズを求める
	unsigned char* fptr = new unsigned char[_footer_size]; //圧縮対象フッターデータ格納先
	_ifs.read((char*)fptr, _prc_f_size); //保存されている圧縮対象フッターを読み込む
	//フッター復号化
	switch (_mode)
	{
	case ArcLib::Mode::SimpleBind:
		fptr = (unsigned char*)get_simplebind(fptr, _prc_f_size, _footer_size);
		break;
	case ArcLib::Mode::Compress:
		fptr = (unsigned char*)get_compress(fptr, _prc_f_size, _footer_size);
		break;
	case ArcLib::Mode::EXOREncrypt:
		fptr = (unsigned char*)get_exorenc(fptr, _prc_f_size, _footer_size);
		break;
	default:
		break;
	}
	//fptrからデータを取得していく関数
	int idx = 0;
	auto get_fptr = [&idx, fptr, _footer_size](void* ptr, unsigned int size)->void{memcpy_s(ptr, size, fptr+idx, size); idx += size; };
	for (unsigned int i = 0; i < _header.num; ++i){
		unique_ptr<Header::FileInfo> fi = make_unique<Header::FileInfo>();
		//データ名取得
		char name[NAME_SIZE];
		get_fptr(name, NAME_SIZE);
		//アドレス取得
		get_fptr((char*)&fi->address, sizeof(fi->address));
		//元データサイズ取得
		get_fptr((char*)&fi->org_size, sizeof(fi->org_size));
		//保存サイズ取得
		get_fptr((char*)&fi->prc_size, sizeof(fi->prc_size));
		_header.file_infos.insert(unordered_map<string,unique_ptr<Header::FileInfo>>::value_type(name, move(fi)));
	}
	delete[] fptr;
	_extracting = true;
	return 0;
}

int Extracter::CloseArchiveFile(){
	if (!_extracting){ return -10; }
	_ifs.close();
	_extracting = false;
	return 0;
}

vector<string> Extracter::GetFileList()const{
	if (!_extracting){ return vector<string>(); }
	vector<string> out;
	for (auto& fi : _header.file_infos){
		out.push_back(fi.first);
	}
	return move(out);
}

pair<void*, unsigned int> Extracter::GetFile(const string& fn){
	if (!_extracting){ return make_pair(nullptr, 0); }
	auto it = _header.file_infos.find(fn);
	if (it == _header.file_infos.end()){ return make_pair(nullptr, 0); }

	_ifs.seekg(_header.header_size + (*it).second->address);
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ResetCurrentKey();
		_exor_enc_key->SeekCurrentExorKey(_header.header_size + (*it).second->address - 1); //ファイルストリームはヘッダ先頭のモード指定1Byteを考慮するが、EXORKEYは考慮しないので-1しておく
	}
	void* data = new unsigned char[(*it).second->prc_size];
	_ifs.read((char*)data, sizeof(char)*(*it).second->prc_size);
	switch (_mode)
	{
	case ArcLib::Mode::SimpleBind:
		data = get_simplebind(data, (*it).second->prc_size, (*it).second->org_size);
		break;
	case ArcLib::Mode::Compress:
		data = get_compress(data, (*it).second->prc_size, (*it).second->org_size);
		break;
	case ArcLib::Mode::EXOREncrypt:
		data = get_exorenc(data, (*it).second->prc_size, (*it).second->org_size);
		break;
	default:
		break;
	}
	return make_pair(data, (*it).second->org_size);
}

void* Extracter::get_simplebind(void* tp,unsigned int,unsigned int){
	return tp;
}

void* Extracter::get_compress(void* tp, unsigned int in_size, unsigned int out_size){
	Bytef* ntp = new Bytef[out_size];
	if(uncompress(ntp, (uLong*)&out_size, (Bytef*)tp, in_size)==Z_MEM_ERROR){
		throw std::runtime_error("Extracter::get_compress メモリー不足で展開に失敗しました。");
	}
	delete[] tp;
	return ntp;
}

void* Extracter::get_exorenc(void* tp, unsigned int in_size, unsigned int out_size){
	_exor_enc_key->ExorEncryptData(tp, in_size); //復号化
	Bytef* ntp = new Bytef[out_size];
	int res = uncompress(ntp, (uLong*)&out_size, (Bytef*)tp, in_size);
	if (res == Z_MEM_ERROR){
		throw std::runtime_error("Extracter::get_compress メモリー不足で展開に失敗しました。");
	}
	if (res == Z_DATA_ERROR){
		throw std::runtime_error("Extracter::get_compress 展開に失敗しました。不正なデータです。");
	}
	else if(res!=Z_OK){
		throw std::runtime_error("Extracter::get_compress 展開に失敗しました。");
	}
	delete[] tp;
	return ntp;
}

//EXOR暗号化関連
int Extracter::SetEXOREncryptionKey(uint16_t k){
	if (_extracting){ return -1; }
	_exor_enc_key->SetKey(k);
	return 0;
}

void Extracter::Header::clear(){
	num = 0;
	file_infos.clear();
}
