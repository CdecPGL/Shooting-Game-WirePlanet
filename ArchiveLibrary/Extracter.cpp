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
	//�w�b�_�[�ǂݍ���
	//���[�h�擾
	unsigned char m;
	_ifs.read((char*)&m, 1);
	_mode = ConvertUnsignedIntToMode(m);
	_header.header_size = 1 + 4; //�w�b�_�̃T�C�Y�v�Z
	//�f�[�^���T�C�Y�擾
	_Uint32t d_size;
	_ifs.read((char*)&d_size, sizeof(d_size));
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ExorEncryptData(&d_size, sizeof(d_size));
	}
	//�t�b�^�[�ǂݍ���
	//�f�[�^���擾
	_ifs.seekg(d_size,ios::cur); //�t�b�^�܂ňړ�(���݈ʒu[�w�b�_�[�̖���]����f�[�^�T�C�Y�������V�[�N)
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->SeekCurrentExorKey(d_size);
	}
	unsigned int f_top_pos = (unsigned int)_ifs.tellg(); //�t�b�^�̐擪�ʒu�L�^
	//�f�[�^�����擾
	_ifs.read((char*)&_header.num, sizeof(_header.num));
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ExorEncryptData((char*)&_header.num, sizeof(_header.num));
	}
	//�ۑ��t�b�^�[�T�C�Y���擾
	_ifs.seekg(0, ios::end); //�t�@�C�������܂ňړ���
	unsigned int _prc_f_size = (unsigned int)_ifs.tellg() - (f_top_pos + 4); //�t�@�C�������̈ʒu����t�b�_�[�̐擪�ʒu�ƃf�[�^���ۑ����̃T�C�Y������
	_ifs.seekg(f_top_pos + 4); //�t�b�_�[�̃f�[�^���ۑ����̎��Ɉړ�
	//�t�b�^�[�T�C�Y���߂�
	unsigned int _footer_size = (NAME_SIZE + 4 + 4 + 4)*_header.num; //�W�J��̈��k�Ώۃt�b�^�[�T�C�Y�����߂�
	unsigned char* fptr = new unsigned char[_footer_size]; //���k�Ώۃt�b�^�[�f�[�^�i�[��
	_ifs.read((char*)fptr, _prc_f_size); //�ۑ�����Ă��鈳�k�Ώۃt�b�^�[��ǂݍ���
	//�t�b�^�[������
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
	//fptr����f�[�^���擾���Ă����֐�
	int idx = 0;
	auto get_fptr = [&idx, fptr, _footer_size](void* ptr, unsigned int size)->void{memcpy_s(ptr, size, fptr+idx, size); idx += size; };
	for (unsigned int i = 0; i < _header.num; ++i){
		unique_ptr<Header::FileInfo> fi = make_unique<Header::FileInfo>();
		//�f�[�^���擾
		char name[NAME_SIZE];
		get_fptr(name, NAME_SIZE);
		//�A�h���X�擾
		get_fptr((char*)&fi->address, sizeof(fi->address));
		//���f�[�^�T�C�Y�擾
		get_fptr((char*)&fi->org_size, sizeof(fi->org_size));
		//�ۑ��T�C�Y�擾
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
		_exor_enc_key->SeekCurrentExorKey(_header.header_size + (*it).second->address - 1); //�t�@�C���X�g���[���̓w�b�_�擪�̃��[�h�w��1Byte���l�����邪�AEXORKEY�͍l�����Ȃ��̂�-1���Ă���
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
		throw std::runtime_error("Extracter::get_compress �������[�s���œW�J�Ɏ��s���܂����B");
	}
	delete[] tp;
	return ntp;
}

void* Extracter::get_exorenc(void* tp, unsigned int in_size, unsigned int out_size){
	_exor_enc_key->ExorEncryptData(tp, in_size); //������
	Bytef* ntp = new Bytef[out_size];
	int res = uncompress(ntp, (uLong*)&out_size, (Bytef*)tp, in_size);
	if (res == Z_MEM_ERROR){
		throw std::runtime_error("Extracter::get_compress �������[�s���œW�J�Ɏ��s���܂����B");
	}
	if (res == Z_DATA_ERROR){
		throw std::runtime_error("Extracter::get_compress �W�J�Ɏ��s���܂����B�s���ȃf�[�^�ł��B");
	}
	else if(res!=Z_OK){
		throw std::runtime_error("Extracter::get_compress �W�J�Ɏ��s���܂����B");
	}
	delete[] tp;
	return ntp;
}

//EXOR�Í����֘A
int Extracter::SetEXOREncryptionKey(uint16_t k){
	if (_extracting){ return -1; }
	_exor_enc_key->SetKey(k);
	return 0;
}

void Extracter::Header::clear(){
	num = 0;
	file_infos.clear();
}
