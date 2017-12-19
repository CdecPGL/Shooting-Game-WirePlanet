//2014/4/21 Version1.0
//2015/1/8 Version1.1 boostlibrary��1.57.0�ɕύX
#pragma once

#include<unordered_map>
#include<string>
#include"boost/any.hpp"

/*�f�[�^�i�[�N���X(�f�t�H���g�R���X�g���N�^�K�{)
�f�[�^���R�s�[���A�R�s�[�I�u�W�F�N�g��boost::any�ŊǗ�
*/

class DataContainer
{
public:
	DataContainer(); //�R���X�g���N�^
	DataContainer(const DataContainer&); //�R�s�[�R���X�g���N�^
	virtual ~DataContainer(); //�f�X�g���N�^
	//�f�[�^�̃Z�b�g(0:�����̃f�[�^�ɃZ�b�g,1:�V�K�Ƀf�[�^���쐬���ĂɃZ�b�g,-1:�^���Ⴄ)
	template<typename T> int SetData(const std::string&, const T&);
	//�f�[�^�̎擾(0:����,-1:�w��f�[�^��������Ȃ�����,-2:�^�̕s��v)
	template<typename T> int GetData(const std::string&, T&);
	//�f�[�^�̏���(0:����,-1:�w��f�[�^��������Ȃ�����,-2:�^�̕s��v)
	int DeleteData(const std::string&);
	//�S����
	void clear();
	//�T�C�Y�擾
	size_t size();
	//���Z�q
	DataContainer operator + (const DataContainer&); //�����Z(this|param)
	DataContainer operator - (const DataContainer&); //�����Z(this&~param)
	DataContainer operator = (const DataContainer&); //���
	DataContainer operator & (const DataContainer&); //�ϏW��(this|param)
	DataContainer operator | (const DataContainer&); //�a�W��(this&param)
	DataContainer operator ^ (const DataContainer&); //�r���I�a�W��(this^param)

	//bool operator == (const DataContainer&); //����(�L�[�̔�r)
	//bool operator != (const DataContainer&); //�񓙍�(�L�[�̔�r)
	bool operator < (const DataContainer&); //�s����(�T�C�Y)
	bool operator > (const DataContainer&); //�s����(�T�C�Y)
	bool operator <= (const DataContainer&); //�s����(�T�C�Y)
	bool operator >= (const DataContainer&); //�s����(�T�C�Y)

	DataContainer& operator += (const DataContainer&); //�����ɑ���(this|param)
	DataContainer& operator -= (const DataContainer&); //�����������(this&~param)
	DataContainer& operator &= (const DataContainer&); //�����ɏW���ω��Z
	DataContainer& operator |= (const DataContainer&); //�����ɘa�W�����Z
	DataContainer& operator ^= (const DataContainer&); //�����ɔr���I�a�W�����Z
private:
	std::unordered_map<std::string, boost::any> data_info; //�f�[�^���
};

template<typename T>
int DataContainer::SetData(const std::string& id,const  T& data){
	std::unordered_map<std::string, boost::any>::iterator it = data_info.find(id);
	//������������
	if (it == data_info.end()){
		data_info.insert(std::unordered_map<std::string,boost::any>::value_type(id,T(data)));
		return 1;
	}
	else{
		try{
			boost::any_cast<T>((*it).second);
		}
		catch(boost::bad_any_cast){ //�^����v���Ă��Ȃ�������
			return -1;
		}
		(*it).second = T(data); //�f�[�^���R�s�[����
		return 0;
	}
}

template<typename T> int DataContainer::GetData(const std::string& id, T& data){
	std::unordered_map<std::string, boost::any>::iterator it = data_info.find(id);
	if (it == data_info.end()){ return -1; } //�w�肳�ꂽID�ɑ��݂��Ȃ�
	try{
		data = boost::any_cast<T>((*it).second);
	}
	catch (boost::bad_any_cast){//�^����v���Ȃ�
		return -2;
	}
	return 0;
}
