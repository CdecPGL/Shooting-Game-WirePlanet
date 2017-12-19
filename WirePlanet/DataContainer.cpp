#include "DataContainer.h"
#include<algorithm>

using namespace std;

//コンストラクタ
DataContainer::DataContainer()
{
}

//デストラクタ
DataContainer::~DataContainer(){
	/*for (unordered_map<string, boost::any>::iterator it = data_info.begin(); it != data_info.end(); ++it){
		delete (*it).second;
	}*/
}

//コピーコンストラクタ
DataContainer::DataContainer(const DataContainer& obj){
	//引数のデータをコピー
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		data_info.insert(unordered_map<string, boost::any>::value_type((*it).first,(*it).second));
	}
}

//指定データの削除
int DataContainer::DeleteData(const string& id){
	unordered_map<string, boost::any>::iterator it = data_info.find(id);
	if (it == data_info.end()){ return -1; } //指定されたIDに存在しない
//	delete (*it).second; //データを削除
	data_info.erase(it); //リストから削除
	return 0;
}

//全消去
void DataContainer::clear(){
	for (unordered_map<string, boost::any>::iterator it = data_info.begin(); it != data_info.end(); ++it){
//		delete (*it).second;
	}
	data_info.clear();
}

//サイズ取得
size_t DataContainer::size(){
	return data_info.size();
}

//演算子
DataContainer DataContainer::operator + (const DataContainer& obj){
	DataContainer out; //出力用
	out = *this; //自分のデータを代入
	//引数のデータでoutにないものをoutにコピー
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if(fit == out.data_info.end()){
			out.data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return out;
}
DataContainer DataContainer::operator - (const DataContainer& obj){
	DataContainer out; //出力用
	out = *this; //自分のデータを代入
	//引数のデータでoutにあるものをoutから削除
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if (fit != out.data_info.end()){
			out.data_info.erase(fit);
		}
	}
	return out;
}
DataContainer DataContainer::operator & (const DataContainer& obj){
	DataContainer out; //出力用
	out = *this; //自分のデータを代入
	//outのデータで引数にないものを削除
	for (unordered_map<string, boost::any>::iterator it =out.data_info.begin(); it != out.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = const_cast<DataContainer&>(obj).data_info.find((*it).first);
		if (fit == obj.data_info.end()){
			out.data_info.erase(it);
		}
	}
	return out;
}
DataContainer DataContainer::operator | (const DataContainer& obj){
	DataContainer out; //出力用
	out = *this; //自分のデータを代入
	//引数のデータでoutにないものをoutにコピー
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if (fit == out.data_info.end()){
			out.data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return out;
}
DataContainer DataContainer::operator ^ (const DataContainer& obj){
	DataContainer out; //出力用
	out = *this; //自分のデータを代入
	//引数のデータでoutにないものをoutにコピーしてあるものをoutから削除
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = out.data_info.find((*it).first);
		if (fit == out.data_info.end()){
			out.data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
		else{
			out.data_info.erase(fit);
		}
	}
	return out;
}
DataContainer DataContainer::operator = (const DataContainer& obj){
	DataContainer out; //出力用
	//自分が持っているデータを削除
	clear();
	//引数のデータをコピー
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
	}
	return out;
}
//bool DataContainer::operator == (const DataContainer& obj){
//	//データ数が違ったらF
//	if (data_info.size() != obj.data_info.size()){ return false; }
//	//違うものがあったらF
//	auto pair = std::mismatch(data_info.begin(), data_info.end(), obj.data_info.begin());
//	if (pair.first != data_info.end()){ return false; }
//	return true;
//}

//bool DataContainer::operator != (const DataContainer& obj){
//	//データ数が違ってたらT
//	if (data_info.size() != obj.data_info.size()){ return true; }
//	//違うものが無かったらF
//	auto pair = std::mismatch(data_info.begin(), data_info.end(), obj.data_info.begin());
//	if (pair.first == data_info.end()){ return false; }
//	return true;
//}

bool DataContainer::operator < (const DataContainer& obj){
	if (data_info.size() < const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

bool DataContainer::operator > (const DataContainer& obj){
	if (data_info.size() > const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

bool DataContainer::operator <= (const DataContainer& obj){
	if (data_info.size() <= const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

bool DataContainer::operator >= (const DataContainer& obj){
	if (data_info.size() >= const_cast<DataContainer&>(obj).size()){ return true; }
	return false;
}

DataContainer& DataContainer::operator += (const DataContainer& obj){
	//引数のデータで自分にないものを自分にコピー
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit == data_info.end()){
			data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return *this;
}
DataContainer& DataContainer::operator -= (const DataContainer& obj){
	//引数のデータで自分にあるものを自分から削除
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit != data_info.end()){
			data_info.erase(fit);
		}
	}
	return *this;
}
DataContainer& DataContainer::operator &= (const DataContainer& obj){
	//自分のデータで引数にないものを削除
	for (unordered_map<string, boost::any>::iterator it = data_info.begin(); it != data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = const_cast<DataContainer&>(obj).data_info.find((*it).first);
		if (fit == obj.data_info.end()){
			data_info.erase(it);
		}
	}
	return *this;
}
DataContainer& DataContainer::operator |= (const DataContainer& obj){
	//引数のデータで自分にないものを自分にコピー
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit == data_info.end()){
			data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
	}
	return *this;
}
DataContainer& DataContainer::operator ^= (const DataContainer& obj){
	//引数のデータで自分にないものを自分にコピーしてあるものを自分から削除
	for (unordered_map<string, boost::any>::iterator it = const_cast<DataContainer&>(obj).data_info.begin(); it != obj.data_info.end(); ++it){
		unordered_map<string, boost::any>::iterator fit = data_info.find((*it).first);
		if (fit == data_info.end()){
			data_info.insert(unordered_map<string, boost::any>::value_type((*it).first, (*it).second));
		}
		else{
			data_info.erase(fit);
		}
	}
	return *this;
}
