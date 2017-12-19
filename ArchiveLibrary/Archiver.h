#pragma once
/*
Version1.0 2014/09/06
Version2.0 2014/09/06 ���k�ɑΉ��B�ȈՈÍ����ɑΉ�
Version2.1 2015/08/11 ���C�u�����t�@�C���ǂݍ��݂�pragma comment�ǉ�
*/
#include<string>
#include<fstream>
#include<vector>
#include<memory>
#include"ArchiveLibDef.h"
//�����̃f�[�^����A�[�J�C�u�E�t�@�C�����쐬����N���X
/*�A�[�J�C�u�t�@�C���̍\���ɂ���
�擪����
�w�b�_�[{
���[�h:1Byte 0:�P������ 1:zlib���k+���� 2:zlib���k+����+�e�f�[�^�̐擪���Í���
�f�[�^���T�C�Y4Byte
}
�f�[�^��{
<[�f�[�^]*�f�[�^�̌���>
}
�t�b�^�[{
�f�[�^�̌�:4Byte
<[�f�[�^��:64Byte,�擪�A�h���X4Byte,���f�[�^�T�C�Y:4Byte �ۑ��T�C�Y:4Byte]*�f�[�^�̌���>
}

<>���͈��k�Ώ�
�f�[�^���̓A���t�@�x�b�g�ŁB
�擪�A�h���X�̓f�[�^�̋L�q�����擪��0�Ƃ��A�o�C�g�P�ʂŕ\���B
*/

/*�e���[�h�ɂ���
0:SimpleBind
�P���A���o�́B
�f�[�^��P���Ɍ������ďo�͂���B
1:Compress
���k�o�́B���k�Ώە�(�f�[�^���ƃt�b�^�[�̃f�[�^���L�^�ʒu�ȊO)��zlib�ň��k���ċL�^����B
2:EXOREncrypt
�r���I�_���a�Í����o��
���k�Ώە������k���A����Ƀw�b�_�̃��[�h�L����ȍ~�����ׂĔr���I�_���a�ɂ��Í���
�Í����ɂ��āB
�L�[��16bit�̃L�[����8�r�b�g�A��8�r�b�g�ɕ����Ďg�p�B
�f�[�^���P�o�C�g�ÂL�[�Ɣr���I�_���a���Z���Ă����B
���̎��̃L�[�͏�8�r�b�g�̐��l����X�^�[�g���A1�o�C�g�Í������邲�Ƃɉ�8�r�b�g���������B
�Ⴆ�΃L�[0x1234�ŉ��̕������Í�������ꍇ�͈Í�������Ώꍇ�A
AA BB CC DD
��
AA^(12) BB^(12+34) CC^(12+34+34) DD^(12+34+34+34)
�ƂȂ�B
*/

namespace ArcLib{
	class EXOREncKey;
}

class Archiver{
public:
	Archiver();
	~Archiver();
	int SetEXOREncryptionKey(uint16_t); //�r���I�_���a�Í����L�[�Z�b�g
	int BeginArchive(const std::string&,ArcLib::Mode,unsigned int=6); //�A�[�J�C�u�J�n(�o�̓t�@�C���A�o�̓��[�h�A���k���x��(1-9))
	int EndArchive(); //�A�[�J�C�u�I��
	int InsertData(const std::string&,const void*,unsigned int); //�f�[�^�}��(BeginArchive�̎��s��AEndArchive�̎��s�O�ɌĂԁB���̊֐��̎g�p��Ƀf�[�^�͏����Ă�OK)
private:
	bool _archiving;
	std::ofstream _ofs;
	struct Header{
		_Uint32t num=0;
		_Uint32t total_data_size = 0;
		struct FileInfo{
			char name[ArcLib::NAME_SIZE];
			_Uint32t address=0;
			_Uint32t org_size=0; //���T�C�Y
			_Uint32t prc_size = 0; //������T�C�Y
		};
		std::vector<std::unique_ptr<FileInfo>> file_infos;
		void clear();
	};

	Header _header;
	ArcLib::Mode _mode;

	unsigned int _comp_level; //���k�x

	//EXOR�Í����֘A
	std::unique_ptr<ArcLib::EXOREncKey> _exor_enc_key;

	//�f�[�^�������݊֘A
	_Uint32t write_simplebind(const void*,unsigned int);
	_Uint32t write_compress(const void*, unsigned int);
	_Uint32t write_exorenc(const void*, unsigned int);
};
