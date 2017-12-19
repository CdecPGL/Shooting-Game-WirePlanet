#include"Archiver.h"
#include<fstream>
#include"zlib_cdec.h"
#include"EXORenckey.h"

using namespace std;
using namespace ArcLib;

Archiver::Archiver():_archiving(false),_exor_enc_key(std::make_unique<EXOREncKey>()){}

Archiver::~Archiver(){}

int Archiver::BeginArchive(const string& fn,Mode mode,unsigned int cl){
	if (_archiving){ return -10; }
	if (cl < 1 || cl>9){ return -4; }
	_comp_level = cl;
	_mode = mode;
	_ofs.open(fn, ios::binary | ios::trunc);
	if (!_ofs){ return -1; }
	if (_ofs.fail()){ return -2; }
	_header.clear();
//	_datas.clear();
	//�w�b�_�[
	//���[�h
	unsigned char m = ConvertModeToUnsignedInt(_mode);
	_ofs.write((char*)&m, 1);
	if (mode == Mode::EXOREncrypt){ //�f�[�^�T�C�Y�L�ڈ�(4Bytes)������EXOR�L�[��i�߂�
		_exor_enc_key->SeekCurrentExorKey(4);
	}
	//�f�[�^�T�C�Y�̋L�ڕ�������������ł���
	_ofs.write((char*)&_header.total_data_size, sizeof(_header.total_data_size));
	_archiving = true;
	return 0;
}

int Archiver::EndArchive(){
	if (!_archiving){ return -10; }
	//�t�b�_�[�o��
	int cmp_tar_footer_size = (NAME_SIZE + 4 + 4 + 4)*_header.num; //���k�Ώۂ̃t�b�^�[�T�C�Y
	unsigned char* fptr = new unsigned char[cmp_tar_footer_size]; //���k�Ώۃt�b�^�[�f�[�^�̃|�C���^
	int idx = 0;
	//�t�@�C�����L�^
	_Uint32t data_num = _header.num;
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ExorEncryptData(&data_num, sizeof(data_num));
	}
	_ofs.write((char*)&data_num, sizeof(data_num));
	//fptr�Ƀf�[�^��ǉ����Ă����֐�
	auto push_fptr = [&idx, fptr, cmp_tar_footer_size](const void* ptr, unsigned int size)->void{memcpy_s(fptr + idx, cmp_tar_footer_size - idx, ptr, size); idx += size; };

	//�t�@�C���������k�Ώۃt�b�^�[�f�[�^�ɋL�^
	for (auto& fi : _header.file_infos){
		push_fptr(fi->name, NAME_SIZE);
		push_fptr((char*)&fi->address, sizeof(fi->address));
		push_fptr((char*)&fi->org_size, sizeof(fi->org_size));
		push_fptr((char*)&fi->prc_size, sizeof(fi->prc_size));
	}
	//���k�Ώۃt�b�^�[���������ĕۑ�
	switch (_mode)
	{
	case Mode::SimpleBind:
		write_simplebind(fptr, cmp_tar_footer_size);
		break;
	case Mode::Compress:
		write_compress(fptr, cmp_tar_footer_size);
		break;
	case Mode::EXOREncrypt:
		write_exorenc(fptr, cmp_tar_footer_size);
		break;
	default:
		break;
	}
	delete[] fptr;
	//�f�[�^�T�C�Y��������
	_ofs.seekp(1); //�擪�̃f�[�^�T�C�Y�L�^�ʒu�Ɉړ�(���[�h�L�^(1Byte)�̂�)
	uint32_t data_size = _header.total_data_size;
	if (_mode == Mode::EXOREncrypt){
		_exor_enc_key->ResetCurrentKey();
		_exor_enc_key->ExorEncryptData(&data_size, sizeof(data_size));
	}
	_ofs.write((char*)&data_size, sizeof(data_size));
	//�I��
	_ofs.close();
	_archiving = false;
	return 0;
}

int Archiver::InsertData(const string& name, const void* tp, unsigned int s){
	if (!_archiving){ return -10; }
	//���O�T�C�Y��NAME_SIZEByte�ȏゾ������G���[
	if (name.size() > NAME_SIZE){ return -1; }
	//�f�[�^��������
	_Uint32t write_size;
	switch (_mode)
	{
	case Mode::SimpleBind:
		write_size = write_simplebind(tp, s);
		break;
	case Mode::Compress:
		write_size = write_compress(tp, s);
		break;
	case Mode::EXOREncrypt:
		write_size = write_exorenc(tp, s);
		break;
	default:
		break;
	}
	//�w�b�_�[�ҏW
	std::unique_ptr<Header::FileInfo> fi = make_unique<Header::FileInfo>();
	fi->address = _header.total_data_size;
	_header.total_data_size += write_size;
	fi->org_size = s;
	fi->prc_size = write_size;
	for (int i = 0; i < NAME_SIZE; ++i){
		fi->name[i] = 0;
	}
	strcpy_s(fi->name, NAME_SIZE, name.c_str());
	_header.file_infos.push_back(move(fi));
	++_header.num;
	return 0;
}

_Uint32t Archiver::write_simplebind(const void* tp,unsigned int s){
	_ofs.write((char*)tp, s);
	return s;
}

_Uint32t Archiver::write_compress(const void* tp, unsigned int s){
	uLong write_size = compressBound(s);
	Bytef* wtp = new Bytef[write_size];
	if (compress2(wtp, &write_size, (const Bytef*)tp, s, _comp_level)==Z_MEM_ERROR){
		throw std::runtime_error("Archiver::insert_compress �������[�s���ň��k�Ɏ��s���܂����B");
	}
	_ofs.write((char*)wtp, write_size);
	delete[] wtp;
	return write_size;
}

_Uint32t Archiver::write_exorenc(const void* tp, unsigned int s){
	uLong write_size = compressBound(s);
	Bytef* wtp = new Bytef[write_size];
	if (compress2(wtp, &write_size, (const Bytef*)tp, s, _comp_level) == Z_MEM_ERROR){
		throw std::runtime_error("Archiver::insert_compress �������[�s���ň��k�Ɏ��s���܂����B");
	}
	_exor_enc_key->ExorEncryptData(wtp, write_size); //EXOR�Í���
	_ofs.write((char*)wtp, write_size);
	delete[] wtp;
	return write_size;
}

//EXOR�Í����֘A
int Archiver::SetEXOREncryptionKey(uint16_t k){
	if (_archiving){ return -1; }
	_exor_enc_key->SetKey(k);
	return 0;
}

void Archiver::Header::clear(){
	num = 0;
	total_data_size = 0;
	file_infos.clear();
}
