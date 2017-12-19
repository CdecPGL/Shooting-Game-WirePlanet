#pragma once

#include<string>
#include<unordered_map>
#include<memory>

class DataScript;

class DataBlock
{
public:
	friend DataScript;

	/**
	* @brief �R���X�g���N�^
	*/
	DataBlock();
	/**
	* @brief �R���X�g���N�^
	* @param �f�[�^
	*/
	explicit DataBlock(const std::vector<std::vector<std::string>>&);
	/**
	* @brief �R���X�g���N�^
	* @param �f�[�^
	*/
	explicit DataBlock(std::vector<std::vector<std::string>>&&);
	/**
	* @brief �R�s�[�R���X�g���N�^
	*/
	DataBlock(const DataBlock&);
	/**
	* @brief ���[�u�R���X�g���N�^
	*/
	DataBlock(DataBlock&&);
	/**
	* @brief �f�X�g���N�^
	*/
	~DataBlock();
	/**
	* @brief ������Z�q
	*/
	const DataBlock& operator=(const DataBlock&);
	/**
	* @brief ���[�u������Z�q
	*/
	const DataBlock& operator=(DataBlock&&);
	/**
	* @brief �q�f�[�^�u���b�N��T��(const��)(������Ȃ��ꍇ��std::out_of_range�𓊂���)
	* @param �q�f�[�^�u���b�N��
	* @return ���߂Ɍ��������q�f�[�^�u���b�N�|�C���^
	*/
	std::shared_ptr<const DataBlock> FindChild(const std::string&)const;
	/**
	* @brief �q�f�[�^�u���b�N��T��(��const��)(������Ȃ��ꍇ��std::out_of_range�𓊂���)
	* @param �q�f�[�^�u���b�N��
	* @return ���߂Ɍ��������q�f�[�^�u���b�N�|�C���^
	*/
	std::shared_ptr<DataBlock> FindChild(const std::string&);
	/**
	* @brief �q�f�[�^�u���b�N��S�ĒT��(const��)(������Ȃ��ꍇ��std::out_of_range�𓊂���)
	* @param �q�f�[�^�u���b�N��
	* @return ���߂Ɍ��������q�f�[�^�u���b�N�|�C���^vector
	*/
	const std::vector<std::shared_ptr<const DataBlock>> FindAllChildren(const std::string&)const;
	/**
	* @brief �q�f�[�^�u���b�N��S�ĒT��(��const��)(������Ȃ��ꍇ��std::out_of_range�𓊂���)
	* @param �q�f�[�^�u���b�N��
	* @return ���߂Ɍ��������q�f�[�^�u���b�N�|�C���^vector
	*/
	const std::vector<std::shared_ptr<DataBlock>> FindAllChildren(const std::string&);
	/**
	* @brief �q�f�[�^�u���b�N��ǉ�����
	* @param �q�f�[�^�u���b�N��,�ǉ�����q�f�[�^�u���b�N�|�C���^
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int AddChild(const std::string&,std::shared_ptr<DataBlock>);
	/**
	* @brief �q�f�[�^�u���b�N��ǉ�����
	* @param �q�f�[�^�u���b�N��,�q�u���b�N�ɃZ�b�g����f�[�^
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int AddChild(const std::string&, const std::vector<std::vector<std::string>>&);
	/**
	* @brief �q�f�[�^�u���b�N��ǉ�����
	* @param �q�f�[�^�u���b�N��,�q�u���b�N�ɃZ�b�g����f�[�^
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int AddChild(const std::string&, std::vector<std::vector<std::string>>&&);
	/**
	* @brief �q�f�[�^�u���b�N���X�g���擾
	* @return �q�f�[�^�u���b�N���X�g
	*/
	std::vector<std::string> GetChildrenList()const;
	/**
	* @brief �w�薼�̎q�f�[�^�u���b�N�����擾
	* @param �q�f�[�^�u���b�N��
	* @return �w�薼�̎q�f�[�^�u���b�N��
	*/
	size_t ChildrenCountWithName(const std::string&)const;
	/**
	* @brief �q�f�[�^�u���b�N�̌����擾
	* @return �q�f�[�^�u���b�N��
	*/
	size_t ChildrenCount()const;
	/**
	* @brief �q�f�[�^�u���b�N������(�|�C���^��)
	* @param �q�f�[�^�u���b�N�|�C���^
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int RemoveChild(std::weak_ptr<DataBlock>);
	/**
	* @brief �q�f�[�^�u���b�N������(�q�f�[�^�u���b�N���ŁB�w�肳�ꂽ���O�Ɉ�v������̂͂��ׂď���)
	* @param �q�f�[�^�u���b�N��
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int RemoveChild(const std::string&);


	const std::vector<std::vector<std::string>>& GetAllData()const;
	std::vector<std::vector<std::string>>& GetAllData();
	const std::vector<std::string>& GetLine(int)const;
	std::vector<std::string>& GetLine(int);
	const std::string& GetData(int, int)const;
	std::string& GetData(int, int);
	const std::vector<std::string>& operator[](int)const;
	std::vector<std::string>& operator[](int);

	/**
	* @brief �����ɍs��ǉ�
	* @param �f�[�^�z��
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int AddLineToBack(const std::vector<std::string>&);
	/**
	* @brief �����ɍs��ǉ�
	* @param �f�[�^�z��
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int AddLineToBack(std::vector<std::string>&&);
	/**
	* @brief �f�[�^���Z�b�g
	* @param �f�[�^
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int SetData(const std::vector<std::vector<std::string>>&);
	/**
	* @brief �f�[�^���Z�b�g
	* @param �f�[�^
	* @return 0�Ő��� -1�Ŏ��s
	*/
	int SetData(std::vector<std::vector<std::string>>&&);

	//�݊��p
	const DataBlock& FindDataBlock(const std::string&)const;
	std::vector<std::string> GetDataBlockList()const;
	bool IsDataBlockExist(const std::string&)const;

	const DataBlock& operator[](const std::string&)const;
private:
	std::vector<std::vector<std::string>> datas; //�f�[�^
	std::unordered_multimap<std::string, std::shared_ptr<DataBlock>> _child_datablocks; //�q�f�[�^�u���b�N
};

