#pragma once

#include<string>
#include<unordered_map>
#include<memory>

class DataScript;

class DataBlock
{
public:
	friend DataScript;

	/**
	* @brief コンストラクタ
	*/
	DataBlock();
	/**
	* @brief コンストラクタ
	* @param データ
	*/
	explicit DataBlock(const std::vector<std::vector<std::string>>&);
	/**
	* @brief コンストラクタ
	* @param データ
	*/
	explicit DataBlock(std::vector<std::vector<std::string>>&&);
	/**
	* @brief コピーコンストラクタ
	*/
	DataBlock(const DataBlock&);
	/**
	* @brief ムーブコンストラクタ
	*/
	DataBlock(DataBlock&&);
	/**
	* @brief デストラクタ
	*/
	~DataBlock();
	/**
	* @brief 代入演算子
	*/
	const DataBlock& operator=(const DataBlock&);
	/**
	* @brief ムーブ代入演算子
	*/
	const DataBlock& operator=(DataBlock&&);
	/**
	* @brief 子データブロックを探す(const版)(見つからない場合はstd::out_of_rangeを投げる)
	* @param 子データブロック名
	* @return 初めに見つかった子データブロックポインタ
	*/
	std::shared_ptr<const DataBlock> FindChild(const std::string&)const;
	/**
	* @brief 子データブロックを探す(非const版)(見つからない場合はstd::out_of_rangeを投げる)
	* @param 子データブロック名
	* @return 初めに見つかった子データブロックポインタ
	*/
	std::shared_ptr<DataBlock> FindChild(const std::string&);
	/**
	* @brief 子データブロックを全て探す(const版)(見つからない場合はstd::out_of_rangeを投げる)
	* @param 子データブロック名
	* @return 初めに見つかった子データブロックポインタvector
	*/
	const std::vector<std::shared_ptr<const DataBlock>> FindAllChildren(const std::string&)const;
	/**
	* @brief 子データブロックを全て探す(非const版)(見つからない場合はstd::out_of_rangeを投げる)
	* @param 子データブロック名
	* @return 初めに見つかった子データブロックポインタvector
	*/
	const std::vector<std::shared_ptr<DataBlock>> FindAllChildren(const std::string&);
	/**
	* @brief 子データブロックを追加する
	* @param 子データブロック名,追加する子データブロックポインタ
	* @return 0で成功 -1で失敗
	*/
	int AddChild(const std::string&,std::shared_ptr<DataBlock>);
	/**
	* @brief 子データブロックを追加する
	* @param 子データブロック名,子ブロックにセットするデータ
	* @return 0で成功 -1で失敗
	*/
	int AddChild(const std::string&, const std::vector<std::vector<std::string>>&);
	/**
	* @brief 子データブロックを追加する
	* @param 子データブロック名,子ブロックにセットするデータ
	* @return 0で成功 -1で失敗
	*/
	int AddChild(const std::string&, std::vector<std::vector<std::string>>&&);
	/**
	* @brief 子データブロックリストを取得
	* @return 子データブロックリスト
	*/
	std::vector<std::string> GetChildrenList()const;
	/**
	* @brief 指定名の子データブロック数を取得
	* @param 子データブロック名
	* @return 指定名の子データブロック数
	*/
	size_t ChildrenCountWithName(const std::string&)const;
	/**
	* @brief 子データブロックの個数を取得
	* @return 子データブロック数
	*/
	size_t ChildrenCount()const;
	/**
	* @brief 子データブロックを除去(ポインタ版)
	* @param 子データブロックポインタ
	* @return 0で成功 -1で失敗
	*/
	int RemoveChild(std::weak_ptr<DataBlock>);
	/**
	* @brief 子データブロックを除去(子データブロック名版。指定された名前に一致するものはすべて除去)
	* @param 子データブロック名
	* @return 0で成功 -1で失敗
	*/
	int RemoveChild(const std::string&);


	const std::vector<std::vector<std::string>>& GetAllData()const;
	std::vector<std::vector<std::string>>& GetAllData();
	const std::vector<std::string>& GetLine(int)const;
	std::vector<std::string>& GetLine(int);
	const std::string& GetData(int, int)const;
	std::string& GetData(int, int);
	const std::vector<std::string>& operator[](int)const;
	std::vector<std::string>& operator[](int);

	/**
	* @brief 末尾に行を追加
	* @param データ配列
	* @return 0で成功 -1で失敗
	*/
	int AddLineToBack(const std::vector<std::string>&);
	/**
	* @brief 末尾に行を追加
	* @param データ配列
	* @return 0で成功 -1で失敗
	*/
	int AddLineToBack(std::vector<std::string>&&);
	/**
	* @brief データをセット
	* @param データ
	* @return 0で成功 -1で失敗
	*/
	int SetData(const std::vector<std::vector<std::string>>&);
	/**
	* @brief データをセット
	* @param データ
	* @return 0で成功 -1で失敗
	*/
	int SetData(std::vector<std::vector<std::string>>&&);

	//互換用
	const DataBlock& FindDataBlock(const std::string&)const;
	std::vector<std::string> GetDataBlockList()const;
	bool IsDataBlockExist(const std::string&)const;

	const DataBlock& operator[](const std::string&)const;
private:
	std::vector<std::vector<std::string>> datas; //データ
	std::unordered_multimap<std::string, std::shared_ptr<DataBlock>> _child_datablocks; //子データブロック
};

