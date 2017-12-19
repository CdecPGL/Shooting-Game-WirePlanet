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
		printf("\"%s\"が開けませんでした。\n", filename.c_str());
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
	//無効な行を取り除く(空だったら)
	std::remove_if(lines.begin(), lines.end(), [](std::string line)->bool{
		if (line.size() == 0)return true;
		return false;
	});
	//行がなかったらエラー
	if (lines.size() == 0){
		printf("csvファイルの読み込みエラー。:行の抽出に失敗しました。\n");
		return -1;
	}
	//データ抽出
	int i = 1;
	for (std::string& line : lines){
		std::vector<std::string> datas;
		algorithm::split(datas, line, algorithm::is_any_of(","));
		if (datas.size() == 0){
			printf("csvファイルの読み込みエラー。:データの抽出に失敗しました(%d行目)。\n", i);
			return -1;
		}
		++i;
		//\nを釣り除く
		for (std::string& data : datas){
			std::remove_if(data.begin(), data.end(), [](char w)->bool{return w == '/n'; });
		}
		//データを保存
		loaded_datas.push_back(std::move(datas));
	}
	return 0;
}

CSVLoader::CSV_DATA::iterator CSVLoader::begin(){ return loaded_datas.begin(); }
CSVLoader::CSV_DATA::const_iterator CSVLoader::cbegin()const{ return loaded_datas.cbegin(); }

CSVLoader::CSV_DATA::iterator CSVLoader::end(){ return loaded_datas.end(); }
CSVLoader::CSV_DATA::const_iterator CSVLoader::cend()const{ return loaded_datas.cend(); }