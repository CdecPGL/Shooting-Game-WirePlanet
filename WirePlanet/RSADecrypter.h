#pragma once

#include<memory>
#include"RSAUtility.h"
#include"BinaryData.h"

namespace cdec{
	class RSA;

	class RSADecrypter{
	public:
		RSADecrypter();
		~RSADecrypter();
		void SetDecodeKey(const RSAUtil::uint1024&);
		void SetPublicKey(const RSAUtil::uint1024&);
		int DecryptData(const BinaryData&, BinaryData&)const;
	private:
		std::unique_ptr<RSA> _rsa;
	};
}