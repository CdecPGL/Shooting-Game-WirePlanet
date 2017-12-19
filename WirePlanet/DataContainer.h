//2014/4/21 Version1.0
//2015/1/8 Version1.1 boostlibraryを1.57.0に変更
#pragma once

#include<unordered_map>
#include<string>
#include"boost/any.hpp"

/*データ格納クラス(デフォルトコンストラクタ必須)
データをコピーし、コピーオブジェクトをboost::anyで管理
*/

class DataContainer
{
public:
	DataContainer(); //コンストラクタ
	DataContainer(const DataContainer&); //コピーコンストラクタ
	virtual ~DataContainer(); //デストラクタ
	//データのセット(0:既存のデータにセット,1:新規にデータを作成してにセット,-1:型が違う)
	template<typename T> int SetData(const std::string&, const T&);
	//データの取得(0:成功,-1:指定データが見つからなかった,-2:型の不一致)
	template<typename T> int GetData(const std::string&, T&);
	//データの消去(0:成功,-1:指定データが見つからなかった,-2:型の不一致)
	int DeleteData(const std::string&);
	//全消去
	void clear();
	//サイズ取得
	size_t size();
	//演算子
	DataContainer operator + (const DataContainer&); //足し算(this|param)
	DataContainer operator - (const DataContainer&); //引き算(this&~param)
	DataContainer operator = (const DataContainer&); //代入
	DataContainer operator & (const DataContainer&); //積集合(this|param)
	DataContainer operator | (const DataContainer&); //和集合(this&param)
	DataContainer operator ^ (const DataContainer&); //排他的和集合(this^param)

	//bool operator == (const DataContainer&); //等号(キーの比較)
	//bool operator != (const DataContainer&); //非等号(キーの比較)
	bool operator < (const DataContainer&); //不等号(サイズ)
	bool operator > (const DataContainer&); //不等号(サイズ)
	bool operator <= (const DataContainer&); //不等号(サイズ)
	bool operator >= (const DataContainer&); //不等号(サイズ)

	DataContainer& operator += (const DataContainer&); //自分に足す(this|param)
	DataContainer& operator -= (const DataContainer&); //自分から引く(this&~param)
	DataContainer& operator &= (const DataContainer&); //自分に集合積演算
	DataContainer& operator |= (const DataContainer&); //自分に和集合演算
	DataContainer& operator ^= (const DataContainer&); //自分に排他的和集合演算
private:
	std::unordered_map<std::string, boost::any> data_info; //データ情報
};

template<typename T>
int DataContainer::SetData(const std::string& id,const  T& data){
	std::unordered_map<std::string, boost::any>::iterator it = data_info.find(id);
	//無かったら作る
	if (it == data_info.end()){
		data_info.insert(std::unordered_map<std::string,boost::any>::value_type(id,T(data)));
		return 1;
	}
	else{
		try{
			boost::any_cast<T>((*it).second);
		}
		catch(boost::bad_any_cast){ //型が一致していなかったら
			return -1;
		}
		(*it).second = T(data); //データをコピーする
		return 0;
	}
}

template<typename T> int DataContainer::GetData(const std::string& id, T& data){
	std::unordered_map<std::string, boost::any>::iterator it = data_info.find(id);
	if (it == data_info.end()){ return -1; } //指定されたIDに存在しない
	try{
		data = boost::any_cast<T>((*it).second);
	}
	catch (boost::bad_any_cast){//型が一致しない
		return -2;
	}
	return 0;
}
