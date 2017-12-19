#include "SVLoader.h"
#include<fstream>
#include"boost/algorithm/string.hpp"
#include<algorithm>

CSVLoader::CSVLoader()
{
}

CSVLoader::~CSVLoader()
{
}

int CSVLoader::LoadCSV(const std::string& filename){
	std::ifstream ifs(filename);
	if (ifs.fail()){
		printf("\"%s\"���J���܂���ł����B\n", filename.c_str());
		return -1;
	}
	std::vector<std::string> lines;
	std::string buf;
	while (getline(ifs, buf)){
		lines.push_back(move(buf));
	}
	ifs.close();
	return CreateCSVFromLines(lines);
}

int CSVLoader::LoadCSVByBynary(const void* b_data, unsigned int size){
	if (b_data == nullptr){ return -1; }
	const char* c_ptr = (const char*)b_data;
	unsigned int pos = 0;
	std::vector<std::string> lines;
	std::string buf;
	while (pos < size){
		char w = c_ptr[pos];
		if (w == '\r'){
		}
		else if (w == '\n'){
			lines.push_back(move(buf));
			buf.clear();
		}
		else{
			buf.push_back(w);
		}
		++pos;
	}
	//	buf.pop_back();
	if (buf.size() != 0){ lines.push_back(move(buf)); }
	return CreateCSVFromLines(lines);
}

int CSVLoader::CreateCSVFromLines(std::vector<std::string>& lines){
	loaded_datas.clear();
	using namespace boost;
	//�����ȍs����菜��(�󂾂�����)
	std::remove_if(lines.begin(), lines.end(), [](std::string line)->bool{
		if (line.size() == 0)return true;
		return false;
	});
	//�s���Ȃ�������G���[
	if (lines.size() == 0){
		printf("csv�t�@�C���̓ǂݍ��݃G���[�B:�s�̒��o�Ɏ��s���܂����B\n");
		return -1;
	}
	//�f�[�^���o
	int i = 1;
	for (std::string& line : lines){
		std::vector<std::string> datas;
		algorithm::split(datas, line, algorithm::is_any_of(","));
		if (datas.size() == 0){
			printf("csv�t�@�C���̓ǂݍ��݃G���[�B:�f�[�^�̒��o�Ɏ��s���܂���(%d�s��)�B\n", i);
			return -1;
		}
		++i;
		//\n��ނ菜��
		for (std::string& data : datas){
			std::remove_if(data.begin(), data.end(), [](char w)->bool{return w == '/n'; });
		}
		//�f�[�^��ۑ�
		loaded_datas.push_back(std::move(datas));
	}
	return 0;
}

CSVLoader::CSV_DATA::iterator CSVLoader::begin(){ return loaded_datas.begin(); }
CSVLoader::CSV_DATA::const_iterator CSVLoader::cbegin()const{ return loaded_datas.cbegin(); }

CSVLoader::CSV_DATA::iterator CSVLoader::end(){ return loaded_datas.end(); }
CSVLoader::CSV_DATA::const_iterator CSVLoader::cend()const{ return loaded_datas.cend(); }