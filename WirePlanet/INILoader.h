#pragma once

/*INITFILELoader
* 2014/5 Version 1.0
* 2014/6/22 Version 1.1 ���[�u�Z�}���e�B�N�X���g�p��������
* 2015/3/4 Version 2.0 �ۑ��@�\�ǉ�
*/

#include<vector>

#include"INIData.h"

class INILoader
{
public:
	INILoader();
	~INILoader();
	int LoadINI(const std::string&); //INI�t�@�C���ǂݍ���
	std::unordered_map<std::string, std::string> operator[](const std::string&)const; //�Z�N�V�����擾(�Z�N�V������)
	std::unordered_map<std::string, std::string> GetSection(const std::string&)const; //�Z�N�V�����擾(�Z�N�V������)
	bool CheckSectionExistence(const std::string&)const; //�Z�N�V���������݂��邩�m�F
	std::string GetValue(const std::string&, const std::string&)const; //�l�擾(�Z�N�V������,�l��)(����������G���[���b�Z�[�W���Ԃ�)
	const INIData& GetINIData()const{ return _data; }
private:
	INIData _data;

};

