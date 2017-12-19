#pragma once

#include"DataScript.h"

//データスクリプト標準データ構造関連
namespace DSsds{
	/**
	* @brief データが存在しない場合に返される文字列
	*/
	const std::string not_exist_string("NULL");
	/**
	* @brief データーIDに対応したデータブロック名を取得
	* @param データID
	* @return データブロック名。存在しない場合はnot_exist_string
	*/
	const std::string GetDataBlockNameByDataID(const DataScript&, const std::string&);
	/**
	* @brief データーIDを指定してデータブロックを取得
	* @param データID
	* @return データブロックポインタ。存在しない場合はnullptr
	*/
	std::shared_ptr<const DataBlock> GetDataBlockByDataID(const DataScript&, const std::string&);
	/**
	* @brief 指定デしたデーターIDが有効化確認。
	* @param データID
	* @return 有効な場合はtrue,無効な場合はfalse。IDは登録されているが、対応するデータブロックが存在しない場合もfalseとなる。
	*/
	bool IsValidDataID(const DataScript&, const std::string&);
	/**
	* @brief DSがDSdsdか確認
	* @param DS
	* @return true or false
	*/
	bool IsDSSDS(const DataScript&);

}