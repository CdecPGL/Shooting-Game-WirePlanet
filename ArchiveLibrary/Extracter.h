#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<memory>
#include<unordered_map>
#include"ArchiveLibDef.h"

//�A�[�J�C�u�t�@�C������t�@�C���𒊏o����N���X

namespace ArcLib{
	class EXOREncKey;
}

class Extracter{
public:
	Extracter();
	~Extracter();
	int SetEXOREncryptionKey(uint16_t); //�r���I�_���a�Í����L�[�Z�b�g
	int OpenAchiveFile(const std::string&); //�A�[�J�C�u�t�@�C�����J��
	int CloseArchiveFile(); //�A�[�J�C�u�t�@�C�������
	std::vector<std::string> GetFileList()const; //�t�@�C�����X�g�擾
	std::pair<void*,unsigned int> GetFile(const std::string&); //�t�@�C�����擾����(���̊֐��Ŏ擾�����f�[�^�͎g�p���delete[]�Ŋe���폜����B)

private:
	std::ifstream _ifs;
	bool _extracting;
	struct Header{
		_Uint32t num = 0;
		_Uint32t header_size = 0;
		struct FileInfo{
			_Uint32t address = 0;
			_Uint32t org_size = 0;
			_Uint32t prc_size = 0;
		};
		std::unordered_map<std::string,std::unique_ptr<FileInfo>> file_infos;
		void clear();
	};
	Header _header;
	ArcLib::Mode _mode;

	//EXOR�Í����֘A
	std::unique_ptr<ArcLib::EXOREncKey> _exor_enc_key;

	//���͂����f�[�^�ƈقȂ�f�[�^���o�͂���ꍇ�͓��̓f�[�^���֐�����delete����B(���̓f�[�^�A���̓T�C�Y�A�o�̓T�C�Y)
	void* get_simplebind(void*,unsigned int,unsigned int);
	void* get_compress(void*, unsigned int,unsigned int);
	void* get_exorenc(void*, unsigned int, unsigned int);
};