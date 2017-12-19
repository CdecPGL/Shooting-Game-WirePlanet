#include "SaveData.h"
#include"GamePlayInformations.h"
#include"boost/archive/binary_iarchive.hpp"
#include"boost/archive/binary_oarchive.hpp"
#include"Archiver.h"
#include<fstream>
#include"boost/filesystem.hpp"
#include"RSAEncrypter.h"
#include"RSADecrypter.h"

using namespace std;
using namespace cdec;

namespace{
	static RSAUtil::uint1024 PublicKey(){
		RSAUtil::uint1024 out(0);
		vector<uint64_t> k = { 0x8290115FDC57BEA,0x0E2699546D44F1EF,0xE39C6469D38E0B16,0x7F29636FDA73C4FA,0xA90D0FF0C7B71099,0x1EDB66FD58357087,0x24A929A748642BFC,0xBFAB5325D9005A49,0x4DF4028702DA7447,0x62476542EB033C33,0xCAC7EEF13ECAC5BE,0x347956F5B5081D6B,0xA63BA87EFC323556,0xF7DCD47610B87B6F,0xB8FEE9C5806D5CDD };
		for (auto it = k.rbegin(); it != k.rend(); ++it){
			out += (RSAUtil::uint1024)(*it) << 64 * (it - k.rbegin());
		}
		return out;
	}
	static RSAUtil::uint1024 EncodeKey(){
		RSAUtil::uint1024 out(0);
		vector<uint64_t> k = { 0xC91C4AF43A2,0x1F962E48C2E52BDB,0x022624FBD4412BBB,0xA59AB2529917DB01,0xC1BE26FAECCB01E3,0x8E38F0A47B59948B,0xE976AF67144F7E04,0xAEADD036FA622AF8,0xC1C6248F389A6A52,0x66342A53B16D2B9E,0x31BCC0D3B1528173,0x55613A9F60C19EB6,0xA0672E0DA5FD39DD };
		for (auto it = k.rbegin(); it != k.rend(); ++it){
			out += (RSAUtil::uint1024)(*it) << 64 * (it - k.rbegin());
		}
		return out;
	}
	static RSAUtil::uint1024 DecodeKey(){
		RSAUtil::uint1024 out(0);
		vector<uint64_t> k = { 0x2468501E73D2CEB,0xEC09781AE94BC267,0x7897B3F63FE1C58C,0x2663BBA6EE770A45,0x1D10F9C6900FD8AC,0xFA4842AD17A8E0D0,0xD8DF6E161C3D9DBA,0xCA0C740410F2C7A3,0xCAD169F5D2B7A9C0,0x78232E820F25F3F0,0x81753D731F208532,0x3D692CBDD9E0D62B,0x86DD9CA118903962,0x3E38F2F06F1DF418,0xE48FB8C52B9464B5 };
		for (auto it = k.rbegin(); it != k.rend(); ++it){
			out += (RSAUtil::uint1024)(*it) << 64 * (it - k.rbegin());
		}
		return out;
	}
}

SaveData::SaveData()
{
}


SaveData::~SaveData()
{
}

SaveData& SaveData::GetInstance(){
	static SaveData sd;
	return sd;
}

int SaveData::SetPath(const std::string& p){ 
//	if (boost::filesystem::is_regular_file(p) == false){ return -1; }
	_path = p;
	return 0;
}

int SaveData::Save()const{
	//ゲーム情報のバイナリ化
	std::stringstream ss;
	boost::archive::binary_oarchive arc(ss);
	arc << GamePlayInformations::GetInstance();
	const BinaryData data = std::move(_get_data_from_sstream(ss));
	if (data.size() == 0){ return - 1; }
	if (_save_data(data) < 0){ return -2; }
	return 0;
}

int SaveData::Load(){
	if (boost::filesystem::exists(_path) == false){ //セーブファイルがなかったら読み込まない
		return 1;
	}
	else{
		BinaryData data = std::move(_load_data());
		if (data.size() == 0){ return -1; }
		std::stringstream ss;
		_set_data_to_sstream(data, ss);
		boost::archive::binary_iarchive arc(ss);
		arc >> GamePlayInformations::GetInstance();
		return 0;
	}
}

BinaryData SaveData::_load_data()const{
	//読み込み
	ifstream ifs(_path, ios::binary);
	if (!ifs){ return BinaryData(); }
	ifs.seekg(0, ios::end);
	unsigned int s = (unsigned int)ifs.tellg();
	ifs.seekg(0, ios::beg);
	BinaryData dat(s);
	ifs.read((char*)dat.get(), s);
	ifs.close();
	//復号化
	RSADecrypter rsad;
	rsad.SetPublicKey(PublicKey());
	rsad.SetDecodeKey(DecodeKey());
	BinaryData ddat;
	rsad.DecryptData(dat, ddat);
	return std::move(ddat);
}

void SaveData::_set_data_to_sstream(const cdec::BinaryData dat, stringstream& ss){
	for (unsigned int i = 0; i < dat.size(); ++i){
		ss << dat[i];
	}
}

int SaveData::_save_data(const cdec::BinaryData dat)const{
	//暗号化
	RSAEncrypter rsae;
	rsae.SetPublicKey(PublicKey());
	rsae.SetEncodeKey(EncodeKey());
	BinaryData edat;
	rsae.EncryptData(dat, edat);
	//保存
	ofstream ofs(_path, ios::binary);
	if (!ofs){ return -1; }
	ofs.write((char*)edat.get(), edat.size());
	return 0;
}

cdec::BinaryData SaveData::_get_data_from_sstream(stringstream& ss)const{
	ss.seekg(0, ios::end);
	unsigned int s = (unsigned int)ss.tellg();
	ss.seekg(0, ios::beg);
	BinaryData out(s);
	ss.read((char*)out.get(), s);
	return std::move(out);
}

//void SaveData::SetInteger(const std::string& name, int dat){
//	auto it = _integer.find(name);
//	if (it == _integer.end()){
//		_integer.insert(make_pair(name, new int(dat)));
//	}
//	else{
//		(*it).second.set(dat);
//	}
//}
//void SaveData::SetUInteger(const std::string& name, unsigned int dat){
//	auto it = _uinteger.find(name);
//	if (it == _uinteger.end()){
//		_uinteger.insert(make_pair(name, new int(dat)));
//	}
//	else{
//		(*it).second.set(dat);
//	}
//}
//void SaveData::SetNumber(const std::string& name, double dat){
//	auto it = _number.find(name);
//	if (it == _number.end()){
//		_number.insert(make_pair(name, new double(dat)));
//	}
//	else{
//		(*it).second.set(dat);
//	}
//}
//void SaveData::SetString(const std::string& name, const string& dat){
//	auto it = _string.find(name);
//	if (it == _string.end()){
//		_string.insert(make_pair(name, new string(dat)));
//	}
//	else{
//		(*it).second.set(dat);
//	}
//}
//
//int SaveData::GetInteger(const std::string& name)const{
//	auto it = _integer.find(name);
//	if (it == _integer.end()){ 
//		string estr("SaveData::GetInteger ");
//		estr += name;
//		estr += "は整数型セーブデータに存在しません。";
//		throw out_of_range(estr);
//	}
//	return it->second.get();
//}
//unsigned int SaveData::GetUInteger(const std::string& name)const{
//	auto it = _uinteger.find(name);
//	if (it == _uinteger.end()){
//		string estr("SaveData::GetUInteger ");
//		estr += name;
//		estr += "は符号なし整数型セーブデータに存在しません。";
//		throw out_of_range(estr);
//	}
//	return it->second.get();
//}
//double SaveData::GetNumber(const std::string& name)const{
//	auto it = _number.find(name);
//	if (it == _number.end()){
//		string estr("SaveData::GetNumber ");
//		estr += name;
//		estr += "は実数型セーブデータに存在しません。";
//		throw out_of_range(estr);
//	}
//	return it->second.get();
//}
//std::string SaveData::GetString(const std::string& name)const{
//	auto it = _string.find(name);
//	if (it == _string.end()){
//		string estr("SaveData::GetString ");
//		estr += name;
//		estr += "は文字列型セーブデータに存在しません。";
//		throw out_of_range(estr);
//	}
//	return it->second.get();
//}
