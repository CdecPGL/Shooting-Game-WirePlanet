#include"EXOR.h"
#include"RSAUtility.h"

namespace cdec{
	EXOR::EXOR():_enc_unit_size(8){}
	EXOR::~EXOR(){}

	int EXOR::SetEncryptUnitSize(unsigned int eus){
		if (eus == 0){ return -1; }
		_enc_unit_size = eus;
		if (RSAUtil::GetIntegerBit(_key, _enc_unit_size * 8) == 0){
			_key = 0;
			return -2;
		}
	}

	int EXOR::SetKey(const cpp_int& k){
		if (RSAUtil::GetIntegerBit(k, _enc_unit_size * 8) == 0){
			return -1;
		}
		_key = k;
		return 0;
	}

	EXOR::cpp_int EXOR::Encrypt(const cpp_int& dat){
		if (dat>0 && RSAUtil::GetIntegerBit(dat, _enc_unit_size * 8) == 0){
			return -1;
		}
		cpp_int out = dat;
		return std::move(dat ^ _key);
	}

	EXOR::cpp_int EXOR::Decrypt(const cpp_int& dat){
		if (dat>0 && RSAUtil::GetIntegerBit(dat, _enc_unit_size * 8) == 0){
			return -1;
		}
		cpp_int out = dat;
		return std::move(dat ^ _key);
	}
}