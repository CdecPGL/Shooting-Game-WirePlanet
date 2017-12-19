#pragma once

#include<unordered_map>
#include<string>
#include"SecureObject.h"
#include<sstream>
#include"BinaryData.h"

class SaveData
{
public:
	SaveData(const SaveData&) = delete;
	~SaveData();
	SaveData& operator=(const SaveData&) = delete;

	static SaveData& GetInstance();

	int Save()const;
	int Load(); //セーブデータの読み込み。(0で成功 0未満でエラー 1で初読み込み(ファイルが存在しない))
	int SetPath(const std::string&);

	
	/*void SetInteger(const std::string&, int);
	void SetUInteger(const std::string&, unsigned int);
	void SetNumber(const std::string&, double);
	void SetString(const std::string&, const std::string&);
	int GetInteger(const std::string&)const;
	unsigned int GetUInteger(const std::string&)const;
	double GetNumber(const std::string&)const;
	std::string GetString(const std::string&)const;*/
private:
	SaveData();
	std::string _path;
	
	cdec::BinaryData _load_data()const;
	static void _set_data_to_sstream(const cdec::BinaryData, std::stringstream&);

	int _save_data(const cdec::BinaryData)const;
	cdec::BinaryData _get_data_from_sstream(std::stringstream&)const;

	/*std::unordered_map<std::string, secure_obj::SecureObject<int>> _integer;
	std::unordered_map<std::string, secure_obj::SecureObject<int>> _uinteger;
	std::unordered_map<std::string, secure_obj::SecureObject<double>> _number;
	std::unordered_map<std::string, secure_obj::SecureObject<std::string>> _string;*/
};

