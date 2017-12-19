#pragma once

#include<memory>
#include"RSAUtility.h"
#include"BinaryData.h"

/*暗号化データの末尾には最後のデータブロックのサイズ(バイト)を記録。データブロックの最大サイズが128Byteなので1バイトで十分*/

namespace cdec{

	class RSA;

	class RSAEncrypter{
	public:
		RSAEncrypter();
		~RSAEncrypter();
		int SetEncodeKey(const RSAUtil::uint1024&);
		void SetPublicKey(const RSAUtil::uint1024&);
		int EncryptData(const BinaryData& ,BinaryData&)const; //暗号化。
	private:
		std::unique_ptr<RSA> _rsa;
	};
}