#include"RSAEncrypter.h"
#include"RSA.h"

namespace cdec{
	using namespace RSAUtil;
	RSAEncrypter::RSAEncrypter():_rsa(std::make_unique<RSA>()){}

	RSAEncrypter::~RSAEncrypter(){
	}

	int RSAEncrypter::SetEncodeKey(const RSAUtil::uint1024& k){
		return _rsa->SetEncodeKey(k);
	}

	void RSAEncrypter::SetPublicKey(const RSAUtil::uint1024& k){
		_rsa->SetModulo(k);
	}

	int RSAEncrypter::EncryptData(const BinaryData& in,BinaryData& out)const{
		size_t m_bit = GetIntegerBit(_rsa->GetModulo());
		size_t data_block_size = m_bit % 8 ? m_bit / 8 + 1 : m_bit / 8; //データブロックサイズ
		size_t data_max_size = _rsa->GetAvailableDataMaxSize(); //データ単位最大サイズ
		size_t is = in.size() % data_max_size ? (in.size() / data_max_size + 1)*data_max_size : in.size(); //元データ調整済みサイズ
		size_t es = is / data_max_size*data_block_size; //暗号化後サイズ
		out.resize(es+1); //最後のブロックサイズ記録分、1バイト多く摂る
		//最後のブロックのサイズ記録
		out[out.size() - 1] = data_max_size - (is - in.size());
		for (size_t i = 0; i < is/data_max_size; ++i){
			uint1024 d(0);
			for (unsigned int j = 0; j < data_max_size; ++j){
				if (i*data_max_size + j >= in.size()){ break; }
#ifdef _DEBUG
				RSAUtil::SetAssignedData1024(d, j, in.at(i*data_max_size+j));
#else
				RSAUtil::SetAssignedData1024(d, j, in[i*data_max_size + j]);
#endif
			}
			uint1024 e = std::move(_rsa->Encrypt(d));
//			if (e > pow(uint1024(2), ((data_size + 1) * 8))){ throw 1; }
			for (unsigned int j = 0; j < data_block_size; ++j){
#ifdef _DEBUG
				out.at(i*data_block_size + j) = RSAUtil::GetAssignedData1024(e, j);
#else
				out[i*data_block_size + j] = RSAUtil::GetAssignedData1024(e, j);
#endif
			}
		}
		return 0;
	}
}