#include"EXORenckey.h"

namespace ArcLib{
	EXOREncKey::EXOREncKey() : _exor_enc_key(0), _exor_enc_key_h(0), _exor_enc_key_l(0), _current_exor_key(0){}
	EXOREncKey::~EXOREncKey(){}

	void EXOREncKey::SetKey(uint16_t k){
		_exor_enc_key = k;
		_exor_enc_key_h = (unsigned char)(k >> 8);
		_exor_enc_key_l = (unsigned char)(k);
		_current_exor_key = _exor_enc_key_h;
	}

	unsigned char EXOREncKey::ExorEncrypt(unsigned char d){
		unsigned char ret = d ^ _current_exor_key;
		_current_exor_key += _exor_enc_key_l;
		return ret;
	}

	void EXOREncKey::ResetCurrentKey(){
		_current_exor_key = _exor_enc_key_h;
	}

	void EXOREncKey::ExorEncryptData(void* dat, unsigned int size){
		unsigned char* cdat = (unsigned char*)dat;
		for (unsigned int i = 0; i < size; ++i, ++cdat){
			*cdat = ExorEncrypt(*cdat);
		}
	}

	void EXOREncKey::SeekCurrentExorKey(int num){
		if (num > 0){
			for (int i = 0; i < num; ++i){
				_current_exor_key += _exor_enc_key_l;
			}
		}
		else{
			num *= -1;
			for (int i = 0; i < num; ++i){
				_current_exor_key -= _exor_enc_key_l;
			}
		}
	}
}