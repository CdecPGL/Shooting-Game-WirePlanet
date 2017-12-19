#pragma once

#include"boost/multiprecision/cpp_int.hpp"
#include<vector>

namespace cdec{
	namespace RSAUtil{
		using uint1024 = boost::multiprecision::uint1024_t;
		using uint512 = boost::multiprecision::uint512_t;
		using cpp_int = boost::multiprecision::cpp_int;
		//指定以上の大きさで最小の使用可能なキーを検索する。見つからなかったら0を返す。(素数1、素数2、探索開始数)
		uint1024 SearchAvalableEncodeKey(const uint512&, const uint512&, const uint1024&);
		unsigned int GetIntegerBit(const cpp_int&,unsigned int = 1024); //整数のビット数を取得(対象変数、最大bit数。計測対象がこれを超えていたら0を返す)
		uint1024 CombineIntegers1024(const std::vector<uint64_t>&); //64bit符号なし整数を結合して1024bit符号なし整数を作成。(配列戦闘ポインタ、配列サイズ)
		uint512 CombineIntegers512(const std::vector<uint64_t>&); //64bit符号なし整数を結合して512bit符号なし整数を作成。(配列戦闘ポインタ、配列サイズ)
		unsigned char GetAssignedData1024(const uint1024&, size_t); //1024bit符号なし整数の指定バイト目のデータを取得
		void SetAssignedData1024(uint1024&, size_t,unsigned char); //1024bit符号なし整数の指定バイト目のデータをセット
	}
}