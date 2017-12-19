//�ȈՃA�[�J�C�o�[
/*Version1.0 2014/09/06
Version2.0 2014/09/06 ���k�ɑΉ��B�ȈՈÍ����ɑΉ�
*/
#include"Archiver.h"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include"boost/lexical_cast.hpp"
#include<climits>

const std::string VERSION("2.0");
const std::string DATE("2014/09/07");
const std::string LOG_FILE("easy_archiver_log.txt"); //���O�t�@�C���p�X
const std::string DEFAULT_OUT_PATH("easy_archiver_out.arc"); //�f�t�H���g�o�͐�

//�f�[�^��ǂݍ���
int LoadData(const std::string&, void**, unsigned int*);

int main(int argc,char* argv[]){
	namespace bfs = boost::filesystem;
	std::vector<bfs::path> file_paths;
	bfs::path out_path(DEFAULT_OUT_PATH);
	ArcLib::Mode mode = ArcLib::Mode::SimpleBind;
	unsigned int com_level=6; //���k���x��(�f�t�H���g��zlib�̃f�t�H���g�l6)
	unsigned int key = 0; //�������L�[
	//���O�t�@�C�����J��
	std::ofstream ofs(LOG_FILE,std::ios::trunc);
	ofs << "EasyArchiver " << VERSION << " " << DATE << std::endl;
	//�R�}���h���C���������
	for (int i = 1; i < argc; ++i){
		//�I�v�V�����w�肵��������
		if (argv[i][0] == '-'){
			std::string str(argv[i]);
			switch (str[1]){
				//�o�͐�ݒ�
			case 'O':
			{
				bfs::path op(str.substr(2, str.size() - 2));
				if (op.has_filename()){
					out_path = op;
				}
				else{
					ofs << "�o�͐�t�@�C�����s���ł��B:" << str.substr(2, str.size() - 2) << std::endl;
				}
			}
				break;
			case 'M':
				try{
					mode = ArcLib::ConvertUnsignedIntToMode(boost::lexical_cast<unsigned int>(str.substr(2, str.size() - 2)));
				}
				catch (boost::bad_lexical_cast& e){
					ofs << "�o�̓��[�h���s���ł��B:" << str.substr(2, str.size() - 2) << std::endl;
				}
				break;
			case 'C':
				try{
					com_level = boost::lexical_cast<unsigned int>(str.substr(2, str.size() - 2));
				}
				catch (boost::bad_lexical_cast& e){
					ofs << "���k���x�����s���ł��B:" << str.substr(2, str.size() - 2) << std::endl;
				}
				break;
			case 'K':
				try{
					key = boost::lexical_cast<unsigned int>(str.substr(2, str.size() - 2));
				}
				catch (boost::bad_lexical_cast& e){
					ofs << "�L�[���s���ł��B:" << str.substr(2, str.size() - 2) << std::endl;
				}
				break;
			default:
				ofs << "�����Ȏ��ʎq�ł��B:" << str << std::endl;
				break;
			}
		}
		//�t�@�C���p�X��������
		else{
			bfs::path pth(argv[i]);
			//�f�B���N�g����������
			if (bfs::is_directory(pth)){
				BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(pth),
					bfs::recursive_directory_iterator())) {
					if (!bfs::is_directory(p)){
						file_paths.push_back(p);
					}
				}
			}
			//�t�@�C����������
			else if (bfs::exists(pth)){
				file_paths.push_back(pth);
			}
			else{
				ofs << "���̓p�X���s���ł��B:" << pth << std::endl;
			}
		}
	}

	ofs << "�Ώۃt�@�C��" << std::endl;
	for (auto& in_file : file_paths){
		ofs << in_file << std::endl;
	}

	ofs << "�A�[�J�C�u�t�@�C�����쐬���܂��B���̓t�@�C����:" << file_paths.size() << ",�o�͐�:" << out_path << ",���[�h:" << ArcLib::ConvertModeToString(mode) << ",���k���x��:" << com_level << std::endl;

	//�A�[�J�C�u�쐬
	Archiver arcr;
	if (mode == ArcLib::Mode::EXOREncrypt){
		if ((int)std::numeric_limits<uint16_t>::max() < key){
			ofs << "�Í����L�[���傫�����܂��B:" << key << ",�ő�l:" << std::numeric_limits<uint16_t>::max() << std::endl;
			return -1;
		}
		ofs << "�Í����L�[:" << key << std::endl;
		arcr.SetEXOREncryptionKey((uint16_t)key);
	}
	if (arcr.BeginArchive(out_path.string(),mode,com_level) < 0){
		ofs << "�A�[�J�C�u�̊J�n�Ɏ��s���܂����B" << std::endl;
	}

	for (auto& fn : file_paths){
		unsigned char* ptr;
		unsigned int size;
		int res = LoadData(fn.string(), (void**)&ptr, &size);
		if (res < 0){
			ofs << "�t�@�C���̓ǂݍ��݂ŃG���[���������܂����B:" << res << "," << fn << std::endl;
			continue;
		}
		arcr.InsertData(fn.filename().string(), ptr, size);
		delete[] ptr;
	}

	int res = arcr.EndArchive();
	if (res < 0){
		ofs << "�A�[�J�C�u�t�@�C���̍쐬�Ɏ��s���܂����B:" << res<<std::endl;
	}
	else{
		ofs << "�A�[�J�C�u�t�@�C�����쐬���܂����B�T�C�Y" << bfs::file_size(out_path) << "Bytes" << std::endl;
	}

	//���O�o�͐�����
	ofs.close();
	return 0;
}

int LoadData(const std::string& name, void** ptr, unsigned int* size){
	std::ifstream ifs(name, std::ios::binary);
	if (!ifs){ return -1; }
	//�T�C�Y�擾
	ifs.seekg(0, std::ios::end);
	unsigned int nsize = (int)ifs.tellg();
	//�������m��
	unsigned char* nptr = new unsigned char[nsize];
	if (ptr == nullptr){
		ifs.close();
		return -2;
	}
	//�ǂݍ���
	ifs.seekg(0, std::ios::beg);
	ifs.read((char*)nptr, sizeof(unsigned char)*nsize);
	ifs.close();

	*ptr = nptr;
	*size = nsize;
	return 0;
}
