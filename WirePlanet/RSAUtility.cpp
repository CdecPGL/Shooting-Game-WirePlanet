#include"RSAUtility.h"
#include"boost/math/common_factor.hpp"

using namespace cdec;

RSAUtil::uint1024 RSAUtil::SearchAvalableEncodeKey(const uint512& p1, const uint512& p2, const uint1024& num){
	uint1024 res;
	uint1024 u = (uint1024)(p1 - 1)*(p2 - 1);
	for (res = num;;){
		if (boost::math::gcd(u, res) == 1){
			break;
		}
		try{
			++res;
		}
		catch(std::overflow_error&){
			return std::move(uint1024(0));
		}
	}
	return std::move(res);
}

unsigned int RSAUtil::GetIntegerBit(const cpp_int& intg,unsigned int max_bit){
	using cpp_int = boost::multiprecision::cpp_int;
	unsigned int bit;
	cpp_int pow_num(1);
	for (bit = 0;bit<max_bit + 1; ++bit){
		if (intg < pow_num){
			break;
		}
		pow_num *= 2;
	}
	return bit > max_bit ? 0 : bit;
}

RSAUtil::uint1024 RSAUtil::CombineIntegers1024(const std::vector<uint64_t>& ui64ts){
	uint1024 out(0);
	for (size_t i = 0; i < ui64ts.size() || i < 16; ++i){
		uint1024 buf = ui64ts[i];
		out |= (buf << (i * 64));
	}
	return std::move(out);
}

RSAUtil::uint512 RSAUtil::CombineIntegers512(const std::vector<uint64_t>& ui64ts){
	uint512 out(0);
	for (size_t i = 0; i < ui64ts.size() || i < 8; ++i){
		uint512 buf = ui64ts[i];
		out |= (buf << (i * 64));
	}
	return std::move(out);
}

unsigned char RSAUtil::GetAssignedData1024(const uint1024& ci, size_t s){
	return (unsigned char)((ci >> (s * 8)) & 0xFF);
}

void RSAUtil::SetAssignedData1024(uint1024& ci, size_t s,unsigned char d){
	uint1024 c = d;
	c <<= (s * 8);
	ci |= c;
}
