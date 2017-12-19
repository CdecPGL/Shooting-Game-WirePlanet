/**
* ����f�[�^�X�N���v�g����N���X�B
* 2014/1/26 Version1.0
* 2014/2/1 Version 2.0 �X�N���v�g�̃t�@�C���ւ̕ۑ�������
* 2014/2/4 Version 2.0.1 �t�@�C��������}����d�ɂȂ��Ă��܂��s����b��I�ɉ��
* 2014/3/31 Version 3.0 �\����͂�boost::regex���g�p�B���m�̕s������B
* 2014/4/1 Version 3.0.1 �w�b�_�t�@�C����using namespace std;���폜�B�\�ȂƂ����const,�Q�Ƃ�ݒ�B
* 2014/5/2 Version 3.1 �R�����g�@�\����(//�ŃR�����g�s)
* 2014/5/26 Version 3.2 �����񂩂�̓ǂݍ��݂ɑΉ��B
* 2014/6/22 Version 3.3 ���[�u�Z�}���e�B�N�X���g�p��������?
* 2014/7/18 Version 4.0 �f�[�^�u���b�N�̑��d�\���ɑΉ�(�������݌n�̋@�\�͖�����)
* 2014/7/22 Version 4.1 �s�͂��߂̐����^�u�����͏�������悤�ύX
* 2014/8/30 Version 4.2 �o�C�i���f�[�^����̓ǂݍ��݂ɑΉ��BSJIS��2�o�C�g�R�[�h���l�����������ɕύX�B�J�b�R�̐�������Ȃ�����std::domain_error�𓊂���悤�ύX�B
* 2014/12/17 Version 5.0 DataBlock�啝�ύX�B������Z�q�ǉ��B�q�u���b�N�̖��O�d�������B�f�[�^�̕ҏW�ɑΉ��B�v���W�F�N�g�ݒ��OriginalScriptLibrary.lib��ݒ肵�Ȃ��Ă��悢�悤�ύX(pragma comment)�B�v���v���Z�X�����@�\�ǉ�
* 2014/12/21 Version 5.1 DataScript�W���f�[�^�\������
* 2015/1/8 boostlibrary��1.57.0�ɕύX�Blua���X�^�e�B�b�N�����N�ɕύX

* ���ݒ�ł͊g���q�͒ǉ�����Ȃ��B
* ���@
* Version 1,2,3
* �f�[�^��{�f�[�^}
* ,�f�[�^�̋�؂�
* ;�f�[�^���s
* //�R�����g�s(�s�̓r������͕s�\�B�K���s���߂���)
* �ʒu�w��͍s(�c)��(��)�̏��Ȃ̂�(x,y�w��Ƌt)����
* �f�[�^�̗�̗v�f���͂��ׂč��킹��(�G���[�ɂȂ�Ȃ�������)
* Version 4
* �����u���b�N�ɑΉ�
* �f�[�^�u���b�N:�f�[�^�u���b�N��{�f�[�^�܂��̓f�[�^�u���b�N}
* �f�[�^:�s�ō\���B�s��;�ŋ�؂�
* �s:�f�[�^�ō\���B�f�[�^��,�ŋ�؂�B(�s�͂��߂̐����^�u�����͏��O����܂�)
* //�R�����g�s(�s�̓r������͕s�\�B�K���s���߂���)
*------V4�����ɔ���V1-3�p�֐��̌݊���
*V1-3�܂ł̃t�@�C����Ǎ��݉\�B
*GetDataAll,GetDataLine,GetDataAssigned,GetDataName,GetSize,CheckDataExistence�͎g�p�\
*ChangeDataAll,ChangeDataLine,ChangeDataAssigned,DeleteData,ChangeSize,CreateData�͎g�p�s�\(-1��Ԃ�)
* Version 5
* V4�ɉ����ē����u���b�N�ɑΉ�
* �v���v���Z�X�����@�\�ǉ��BSetUseMacro�Ńt���O�𗧂Ă�Ǝg�p�\�B�ォ����s����܂��B�A���t�@�x�b�g�̂ݑΉ�
* <<�v���v���Z�X����(PPP)>>
* PPP�̒�`��#�Ŏn�܂�;�ŏI���B�������m��,�ŋ�؂�B�t�@�C���̍ŏ��ɘA�����ċL�q����K�v������B����ȊO�͒ʏ�̕�����Ƃ��ď��������B
* <<PPP���߈ꗗ>>
����������(* PPP����������(����ȍ~�̃v��Process�������X�L�b�v����):INVALIDATE_PPP
* PPP�L��������(INVALIDATE_PPP�Ŗ���������Ă����ꍇ�B����ȍ~���������s����):VALIDATE_PPP)
* �u������:REPLACE �u���Ώە����� �u���㕶����(�L�q���ɒu�������B�u����̕�������u���ΏۂɂȂ�̂Œ���)


*/

#ifndef DEF_DATASCRIPT_H
#define DEF_DATASCRIPT_H

#include"DataBlock.h"
#include<string>
//#include<map>
#include<unordered_map>

//DSVer1_4���w�肵�Ă�������DSVer1_5�ɒu���������܂��B
enum class DataScriptVersion{ DSVer1_4,DSVer1_5};

class DataScript
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	DataScript();
	/**
	* @brief �R���X�g���N�^
	* @param �g���q(.�͕s�v)
	*/
	DataScript(const std::string&);
	/**
	* @brief �f�X�g���N�^
	*/
	~DataScript();
	/**
	* @brief �X�N���v�g����f�[�^�ǂݍ���
	* @param filename �X�N���v�g�t�@�C���� (add_flag �ǉ��ǂݍ��݃t���O�Btrue�Ō��݂̃f�[�^���������ɒǉ��ǂݍ��݁B�f�t�H���g��false�B)
	* @return ����������0�B���s������-1�B
	*/
	int LoadDataFromScript(const std::string&,bool = false);
	/**
	* @brief �����񂩂�f�[�^�ǂݍ���(���s,�����^�u�𖳎����Ȃ��s�����)
	* @param ������ (add_flag �ǉ��ǂݍ��݃t���O�Btrue�Ō��݂̃f�[�^���������ɒǉ��ǂݍ��݁B�f�t�H���g��false�B)
	* @return ����������0�B���s������-1�B
	*/
	int LoadDataFromString(const std::string&, bool = false);
	/**
	* @brief �o�C�i���f�[�^����f�[�^�ǂݍ���
	* @param top_ptr �o�C�i���f�[�^�̐擪�A�h���X size �o�C�i���f�[�^�T�C�Y (add_flag �ǉ��ǂݍ��݃t���O�Btrue�Ō��݂̃f�[�^���������ɒǉ��ǂݍ��݁B�f�t�H���g��false�B)
	* @return ����������0�B���s������-1�B
	*/
	int LoadDataFromBynary(const void*,unsigned int, bool = false);
	/**
	* @brief �w�薼�̃X�N���v�g�Ƀf�[�^�ۑ�
	* @param filename �X�N���v�g�t�@�C���� (add_flag �ǉ��ǂݍ��݃t���O�Btrue�ŏ������ݐ�̃f�[�^���������ɒǉ��������݁B�f�t�H���g��false�B)
	* @return ����������0�B���s������-1�B
	*/
	int SaveDataToScript(const std::string&, bool = false)const;
	/**
	* @brief �ǂݍ��񂾃X�N���v�g�Ƀf�[�^�ۑ�
	* @param filename �X�N���v�g�t�@�C���� (add_flag �ǉ��ǂݍ��݃t���O�Btrue�Ō��݂̃f�[�^���������ɒǉ��ǂݍ��݁B�f�t�H���g��false�B)
	* @return ����������0�B���s������-1�B
	*/
	int SaveDataToLoadedScript()const;
	/**
	* @brief �g���q�ݒ�
	* @param file extention �g���q��(.�͕s�v)
	*/
	void SetExtension(const std::string&);
	/**
	* @brief �w�薼�̃f�[�^�S�擾(V1-3)
	* @param dataname �擾�������f�[�^�� data:�f�[�^���i�[����std::vector
	* @return 0:���� -1�w�肳�ꂽ�f�[�^�����݂��Ȃ�
	*/
	int GetDataAll(const std::string&, std::vector<std::vector<std::string>>&)const;
	/**
	* @brief �w�薼�̃f�[�^�s�擾(V1-3)
	* @param dataname �擾�������f�[�^�� line �s�ԍ�(0~) data:�f�[�^���i�[����std::vector
	* @return 0:���� -1:�w�肳�ꂽ�f�[�^�����݂��Ȃ� -2:�s���s��
	*/
	int GetDataLine(const std::string&, int, std::vector<std::string>&)const;
	/**
	* @brief �w�薼�̃f�[�^�ʒu�w��擾(V1-3)
	* @param dataname �擾�������f�[�^�� line,raw:�w��s�� data:�f�[�^���i�[����string
	* @return 0:���� -1:�w�肳�ꂽ�f�[�^�����݂��Ȃ� -2:�s�񂪕s��
	*/
	int GetDataAssigned(const std::string&, int, int, std::string&)const;
	/**
	* @brief �w�薼�̃f�[�^�u���b�N�ύX(V1-3)
	* @param dataname �ύX�������f�[�^�� data:�f�[�^
	* @return 0:���� -1�w�肳�ꂽ�f�[�^�����݂��Ȃ�
	*/
	int ChangeDataAll(const std::string&, const std::vector<std::vector<std::string>>&);
	/**
	* @brief �w�薼�̃f�[�^�s�ύX(V1-3)
	* @param dataname �擾�������f�[�^�� line �s�ԍ�(0~) data:�f�[�^
	* @return 0:���� -1:�w�肳�ꂽ�f�[�^�����݂��Ȃ� -2:�s���s��
	*/
	int ChangeDataLine(const std::string&, int, const std::vector<std::string>&);
	/**
	* @brief �w�薼�̃f�[�^�ʒu�w��ύX(V1-3)
	* @param dataname �擾�������f�[�^�� line,raw:�w��s�� data:�f�[�^
	* @return 0:���� -1:�w�肳�ꂽ�f�[�^�����݂��Ȃ� -2:�s�񂪕s��
	*/
	int ChangeDataAssigned(const std::string&, int, int, const std::string&);
	/**
	* @brief �w�薼�̃f�[�^����(V1-3)
	* @param �폜�������f�[�^��
	* @return 0:���� -1:���s
	*/
	int DeleteData(const std::string&);
	/**
	* @brief �f�[�^�����X�g���擾(V1-3)
	* @return �f�[�^�����X�g
	*/
	std::vector<std::string> GetDataName()const;
	/**
	* @brief �w�薼�̃f�[�^�̃T�C�Y�擾(�s�~��)(V1-3)
	* @param dataname:�f�[�^�� line,raw:�T�C�Y�i�[��
	* @return 0:���� -1:�w�肳�ꂽ�f�[�^�����݂��Ȃ�
	*/
	int GetSize(const std::string&,int&,int&)const;
	/**
	* @brief �w�薼�̃f�[�^�̃T�C�Y�ύX(�s�~��)(V1-3)
	* @param dataname:�f�[�^�� line,raw:�T�C�Y (blank:�󔒂𖄂߂镶������w��)
	* @return 0:���� -1:�w�肳�ꂽ�f�[�^�����݂��Ȃ�
	*/
	int ChangeSize(const std::string&, int, int,std::string = "");
	/**
	* @brief �w�薼�̃f�[�^�쐬(V1-3)
	* @param dataname:�f�[�^�� line,raw:�T�C�Y (blank:�󔒂𖄂߂镶������w��)
	* @return 0:���� -1:���łɑ��݂���
	*/
	int CreateData(const std::string&, int, int,std::string = "");
	/**
	* @brief �m�F�������f�[�^�w�薼(V1-3)
	* @param dataname:�f�[�^��
	* @return true:���݂��� false:���݂��Ȃ�
	*/
	bool CheckDataExistence(const std::string&)const;

	/**
	* @brief ���[�g�f�[�^�u���b�N���擾(const��)
	* @return ���[�g�f�[�^�u���b�N�̎Q��
	*/
	std::shared_ptr<const DataBlock> GetRoot()const;
	/**
	* @brief ���[�g�f�[�^�u���b�N���擾(��const��)
	* @return ���[�g�f�[�^�u���b�N�̎Q��
	*/
	std::shared_ptr<DataBlock> GetRoot();

	/**
	* @brief �ǂݍ��񂾃f�[�^�폜
	*/
	void Clear();
	/**
	* @brief �ǂݍ��񂾃f�[�^�̕\��(�f�o�b�O�p)
	*/
	void DrawData()const;
	/**
	* @brief �v���v���Z�X�@�\���g�p���邩�ݒ�
	* @param true:�g�p���� false:�g�p���Ȃ�
	*/
	void SetPreProcessFlag(bool);
	/**
	* @brief ������̎擾
	* @return ������
	*/
	std::string ToString()const;
	/**
	* @brief �G���[������̎擾
	* @return �G���[������
	*/
	std::vector<std::string> GetErrorString()const;
	/**
	* @brief �����E�\���o�[�W�����̐ݒ�
	* @param �o�[�W����
	*/
	void SetVersion(DataScriptVersion);
	/**
	* @brief ���ݐݒ肳��Ă��鏈���E�\���o�[�W�����̎擾
	* @return �o�[�W����
	*/
	DataScriptVersion GetVersion()const;
	/**
	* @brief ����e�X�g
	* @param �t�@�C����,�ǂݍ��݉�
	* @return -1:�G���[���� 0:����I��
	*/
	int Test(const std::string&,int = 1);

	/**
	* @brief ���[�g�f�[�^�u���b�N���擾(const��)(�݊��p)
	* @return ���[�g�f�[�^�u���b�N�̎Q��
	*/
	const DataBlock& GetRootDataBlock()const;
	/**
	* @brief ���[�g�f�[�^�u���b�N���擾(��const��)(�݊��p)
	* @return ���[�g�f�[�^�u���b�N�̎Q��
	*/
	DataBlock& GetRootDataBlock();
private:
	std::string extension; //�g���q
//	std::map<std::string, std::vector<std::vector<std::string>>> data; //�f�[�^
	std::string script_name; //�ǂݍ��񂾃X�N���v�g��
	std::vector<std::string> error_strings; //�G���[������
	void SetErrorString(const std::string&); //�G���[������Z�b�g
	DataScriptVersion version; //�g�p�o�[�W����
	std::shared_ptr<DataBlock> root_data_block; //���[�g�f�[�^�u���b�N
	bool _preproc_valid_flag; //�v��Process�@�\�L�����t���O

	//���[�h�����֐�
	int process_load(std::string);
	//���[�h�O�����֐�
	int pre_process_before_load(std::string&); //�}�N���Ȃǂ̃v��Process����(�v��Process���߂͏��������)
	//�f�[�^���[�h�֐�(�����̓f�[�^������)
	std::unordered_map < DataScriptVersion, int(DataScript::*)(std::string&)> loaddata;
	//�f�[�^�𕶎��񉻂���֐�
	std::unordered_map<DataScriptVersion, std::string(DataScript::*)()const> datatostring;
	//�o�[�W����5�̊֐�
	int LoadDataV1_5(std::string&);
	static std::shared_ptr<DataBlock> LoadDataBlock(const std::string&);
	std::string DataToStringV1_5()const;
	static std::string DataBlockToString(const DataBlock&,int = 0);

	//SJIS�̃}���`�o�C�g�����R�[�h���m�F����
	static bool SJISMultiCheck(unsigned char);
};

#endif