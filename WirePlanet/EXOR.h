#pragma once

//排他的論理和を用いた暗号、復号器。(作成中)

#include"boost/multiprecision/cpp_int.hpp"

namespace cdec{
	class EXOR{
	public:
		using cpp_int = boost::multiprecision::cpp_int;
		EXOR();
		~EXOR();
		int SetEncryptUnitSize(unsigned int); //暗号化単位サイズ(バイト。1024まで)
		int SetKey(const cpp_int&); //キーセット
		cpp_int Encrypt(const cpp_int&); //暗号化
		cpp_int Decrypt(const cpp_int&); //復号化
	private:
		cpp_int _key; //サイズが暗号化単位サイズより小さいもの
		unsigned int _enc_unit_size; //暗号化単位サイズ(バイト)
	};
}