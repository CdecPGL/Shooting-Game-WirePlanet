#pragma once

#include<string>
#include<unordered_map>

class INIData{
public:
	INIData();
	~INIData();
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator begin(){ return _data.begin(); }
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator end(){ return _data.end(); }
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator find(const std::string& s){ return _data.find(s); }
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator begin()const{ return _data.begin(); }
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator end()const{ return _data.end(); }
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator find(const std::string& s)const{ return _data.find(s); }

	const std::unordered_map<std::string, std::string>& GetSection(const std::string& s)const{ return _data.at(s); }
	std::unordered_map<std::string, std::string>& RefSection(const std::string& s){ return _data.at(s); }
	void SetSection(const std::string& sn, std::unordered_map<std::string, std::string>&& s){ _data.insert(std::make_pair(sn, std::move(s))); }
	void SetSection(const std::string& sn, const std::unordered_map<std::string, std::string>& s){ _data.insert(std::make_pair(sn, s)); }
private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _data;
};