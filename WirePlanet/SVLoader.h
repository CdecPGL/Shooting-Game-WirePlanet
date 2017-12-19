/*CSVLoader
Version1.0 2014/05
Version1.1 2014/06/22 ムーブセマンティクスを使用し高速化
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
	int LoadCSV(const std::string&); //ファイル名
	int LoadCSVByBynary(const void*, unsigned int); //バイナリデータから読み込み(先頭ポインタ、バイトサイズ)
	CSV_DATA::iterator begin();
	CSV_DATA::const_iterator cbegin()const;
	CSV_DATA::iterator end();
	CSV_DATA::const_iterator cend()const;
private:
	std::vector<std::vector<std::string>> loaded_datas;

	int CreateCSVFromLines(std::vector<std::string>&);
};
