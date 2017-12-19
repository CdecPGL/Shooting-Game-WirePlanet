#include"RSA.h"

namespace cdec{
	using namespace RSAUtil;

	const int SECURITY_PARAMETER(1024);

	RSA::RSA() :_encode_key(0), _modulo(0), _prime_number_1(0), _prime_number_2(0), _decode_key(0), _available_data_max_size(0){}

	RSA::~RSA(){
	}

	void RSA::SetPrimeNumbers(const uint512& p1, const uint512& p2){
		if (p1 < 3 || p2 < 3){ return; }
		_prime_number_1 = p1;
		_prime_number_2 = p2;
		CalculateModulo();
		CalculateDecodeKey();
	}

	void RSA::SetPrimeNumbers(uint512&& p1, uint512&& p2){
		_prime_number_1 = std::move(p1);
		_prime_number_2 = std::move(p2);
		CalculateModulo();
		CalculateDecodeKey();
	}

	int RSA::SetEncodeKey(const uint1024& pk){
		if (pk < 2){ return -1; }
		if (_prime_number_1 > 0 || _prime_number_2 > 0){
			if (pk >= (uint1024)(_prime_number_1 - 1)*(_prime_number_2 - 1)){ return -2; }
			if (boost::multiprecision::gcd((uint1024)(_prime_number_1 - 1) * (_prime_number_2 - 1), pk) != 1){ return -3; }
			_encode_key = pk;
			CalculateDecodeKey();
		}
		else{
			_encode_key = pk;
		}
		return 0;
	}

	int RSA::SetEncodeKey(uint1024&& pk){
		if (pk < 2){ return -1; }
		if (_prime_number_1 > 0 || _prime_number_2 > 0){
			if (pk >= (uint1024)(_prime_number_1 - 1)*(_prime_number_2 - 1)){ return -2; }
			if (boost::multiprecision::gcd((uint1024)(_prime_number_1 - 1) * (_prime_number_2 - 1), pk) != 1){ return -3; }
			_encode_key = std::move(pk);
			CalculateDecodeKey();
		}
		else{
			_encode_key = std::move(pk);
		}
		return 0;
	}

	void RSA::SetDecodeKey(const uint1024& dk){
		_decode_key = dk;
		//	CalculateDecodeKey();
	}

	void RSA::SetDecodeKey(uint1024&& dk){
		_decode_key = std::move(dk);
		//	CalculateDecodeKey();
	}

	void RSA::SetModulo(const uint1024& m){
		_modulo = m;
		CalculateAvailableDataMaxSize();
	}

	void RSA::SetModulo(uint1024&& m){
		_modulo = std::move(m);
		CalculateAvailableDataMaxSize();
	}

	const uint1024& RSA::GetEncodeKey()const{ return _encode_key; }
	const uint1024& RSA::GetDecodeKey()const{ return _decode_key; }
	const uint1024& RSA::GetModulo()const{ return _modulo; }
	unsigned int RSA::GetSecurityParameter()const{ return SECURITY_PARAMETER; }
	unsigned int RSA::GetAvailableDataMaxSize()const{ return _available_data_max_size; }

	void RSA::CalculateModulo(){
		_modulo = (uint1024)_prime_number_1*(uint1024)_prime_number_2;
		CalculateAvailableDataMaxSize();
	}

	void RSA::CalculateDecodeKey(){
		if (_encode_key == 0 || _prime_number_1 == 0 || _prime_number_2 == 0){ return; }
		uint1024 u = (uint1024)(_prime_number_1 - 1) * (uint1024)(_prime_number_2 - 1);
		uint1024 u0 = u;
		uint1024 a0 = _encode_key;

		uint1024 t0 = 0, t = 1;
		uint1024 r = u0 % a0;
		uint1024 _prime_number_2 = (u0 - r) / a0;
		int MAX_STEPS = 10000000;

		for (int i = 0; r > 0 && i < MAX_STEPS; ++i) {
			boost::multiprecision::cpp_int tmp = (boost::multiprecision::cpp_int)t0 - _prime_number_2 * t;

			if (tmp >= 0){ tmp = tmp % u; }
			else{ tmp = u - ((-tmp) % u); }

			t0 = t, t = (uint1024)tmp, u0 = a0, a0 = r;
			r = u0 % a0;
			_prime_number_2 = (u0 - r) / a0;
		}

		_decode_key = t % u;
	}

	void RSA::CalculateAvailableDataMaxSize(){
		if (_modulo == 0){ _available_data_max_size = 0; return; }
		/*int bit;
		uint1024 pow_num(2);
		for (bit = 1;bit<1024; ++bit,pow_num*=2){
		if (_modulo < pow_num){
		--bit;
		break;
		}
		}
		_available_data_max_size = (unsigned int)bit / 8;*/
		_available_data_max_size = (unsigned int)RSAUtil::GetIntegerBit(_modulo) / 8;
	}

	//暗号化
	uint1024 RSA::Encrypt(const uint1024& d){
		if (d >= _modulo){ return std::move(uint1024(0)); }
		return std::move(PowMod(d, _encode_key, _modulo));
	}

	//復号化
	uint1024 RSA::Decrypt(const uint1024& d){
		if (d >= _modulo){ return std::move(uint1024(0)); }
		//	return std::move(boost::multiprecision::powm(d,_decode_key,_modulo)); //何故か遅い
		return std::move(PowMod(d, _decode_key, _modulo));
	}

	//剰余を求める
	uint1024 RSA::PowMod(const uint1024& num, const uint1024& e, const uint1024& modulo){
		if (num == 0){ return std::move(uint1024(0)); }

		//累乗バッファ作成
		/*static std::vector<uint1024> _two_pow_buf; //2の二乗のバッファ
		if (_two_pow_buf.size() == 0){
		int cnt = 0;
		for (cpp_int i = 1;cnt<1024; i *= 2,++cnt){
		_two_pow_buf.push_back((uint1024)i);
		}
		}*/

		//	uint1024 res(1);
		boost::multiprecision::uint1024_t res(1);
		uint1024 pow_buf = num;
		for (uint1024 i = 1; i < e; i *= 2){
			if (e & i){
				res = (uint1024)((cpp_int)res*pow_buf % modulo);
			}
			pow_buf = (uint1024)((cpp_int)pow_buf * pow_buf % modulo);
		}
		/*int e_bit = RSAUtil::GetIntegerBit(e);
		auto it = _two_pow_buf.begin();
		for (int i = 0; i < e_bit; ++i,++it){
		if (e & *it){
		res = (uint1024)((boost::multiprecision::cpp_int)res*pow_buf % modulo);
		}
		pow_buf = (uint1024)((boost::multiprecision::cpp_int)pow_buf * pow_buf % modulo);
		}*/
		return std::move(res);
	}
}