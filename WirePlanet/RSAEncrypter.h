#pragma once

#include<memory>
#include"RSAUtility.h"
#include"BinaryData.h"

/*�Í����f�[�^�̖����ɂ͍Ō�̃f�[�^�u���b�N�̃T�C�Y(�o�C�g)���L�^�B�f�[�^�u���b�N�̍ő�T�C�Y��128Byte�Ȃ̂�1�o�C�g�ŏ\��*/

namespace cdec{

	class RSA;

	class RSAEncrypter{
	public:
		RSAEncrypter();
		~RSAEncrypter();
		int SetEncodeKey(const RSAUtil::uint1024&);
		void SetPublicKey(const RSAUtil::uint1024&);
		int EncryptData(const BinaryData& ,BinaryData&)const; //�Í����B
	private:
		std::unique_ptr<RSA> _rsa;
	};
}