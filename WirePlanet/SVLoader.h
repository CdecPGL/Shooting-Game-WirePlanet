/*CSVLoader
Version1.0 2014/05
Version1.1 2014/06/22 ���[�u�Z�}���e�B�N�X���g�p��������
*/

#pragma once
#include<vector>
#include<string>
#include<exception>

class CSVLoader
{
	using CSV_DATA = std::vector < std::vector<std::string> > ;
public:
	CSVLoader();
	~CSVLoader();
	int LoadCSV(const std::string&); //�t�@�C����
	int LoadCSVByBynary(const void*, unsigned int); //�o�C�i���f�[�^����ǂݍ���(�擪�|�C���^�A�o�C�g�T�C�Y)
	CSV_DATA::iterator begin();
	CSV_DATA::const_iterator cbegin()const;
	CSV_DATA::iterator end();
	CSV_DATA::const_iterator cend()const;
private:
	std::vector<std::vector<std::string>> loaded_datas;

	int CreateCSVFromLines(std::vector<std::string>&);
};
