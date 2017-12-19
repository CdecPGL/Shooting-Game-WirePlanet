#pragma once

/*INITFILELoader
* 2014/5 Version 1.0
* 2014/6/22 Version 1.1 ムーブセマンティクスを使用し高速化
* 2015/3/4 Version 2.0 保存機能追加
*/

#include<vector>

#include"INIData.h"

class INILoader
{
public:
	INILoader();
	~INILoader();
	int LoadINI(const std::string&); //INIファイル読み込み
	std::unordered_map<std::string, std::string> operator[](const std::string&)const; //セクション取得(セクション名)
	std::unordered_map<std::string, std::string> GetSection(const std::string&)const; //セクション取得(セクション名)
	bool CheckSectionExistence(const std::string&)const; //セクションが存在するか確認
	std::string GetValue(const std::string&, const std::string&)const; //値取得(セクション名,値名)(無かったらエラーメッセージか返る)
	const INIData& GetINIData()const{ return _data; }
private:
	INIData _data;

};

