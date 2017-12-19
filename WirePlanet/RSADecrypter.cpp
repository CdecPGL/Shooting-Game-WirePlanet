#include"RSADecrypter.h"
#include"RSA.h"

namespace cdec{
	using namespace RSAUtil;
	RSADecrypter::RSADecrypter():_rsa(std::make_unique<RSA>()){};
	RSADecrypter::~RSADecrypter(){};

	void RSADecrypter::SetDecodeKey(const uint1024& k){
		_rsa->SetDecodeKey(k);
	}

	void RSADecrypter::SetPublicKey(const uint1024& k){
		_rsa->SetModulo(k);
	}

	int RSADecrypter::DecryptData(const BinaryData& in, BinaryData& out)const{
		size_t m_bit = GetIntegerBit(_rsa->GetModulo());
		size_t data_block_size = m_bit % 8 ? m_bit / 8 + 1 : m_bit / 8;
		size_t data_max_size = _rsa->GetAvailableDataMaxSize();

		//最後のブロックのサイズ取得
		size_t last_block_size = in[in.size() - 1];

		if ((in.size()-1)%data_block_size != 0){ //サイズが合ってなかったら
			return -1;
		}

		out.resize(in.size() / data_block_size * data_max_size);

		for (size_t i = 0; i < in.size() / data_block_size; ++i){
			uint1024 e(0);
			for (unsigned int j = 0; j < data_block_size; ++j){
#ifdef _DEBUG
				RSAUtil::SetAssignedData1024(e, j, in.at(i*data_block_size+j));
#else
				RSAUtil::SetAssignedData1024(e, j, in[i*data_block_size + j]);
#endif
			}
			uint1024 d = std::move(_rsa->Decrypt(e));
//			if (d > pow(uint1024(2),((data_size+1)*8))){throw 1; }
			size_t ds = data_max_size;
			//最後のデータブロックだったらサイズ確認
			if (i == in.size() / data_block_size - 1){
				ds = last_block_size;
				//サイズの変更
				if(ds<data_max_size)out.resize(out.size() - (data_max_size - ds));
			}
			for (unsigned int j = 0; j < ds; ++j){
#ifdef _DEBUG
				out.at(i*data_max_size + j) =  RSAUtil::GetAssignedData1024(d, j);
#else
				out[i*data_max_size + j] = RSAUtil::GetAssignedData1024(d, j);
#endif
			}
		}
		return 0;
	}
}