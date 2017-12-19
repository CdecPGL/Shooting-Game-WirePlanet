#pragma once

#include"boost/multiprecision/cpp_int.hpp"
#include<vector>

namespace cdec{
	namespace RSAUtil{
		using uint1024 = boost::multiprecision::uint1024_t;
		using uint512 = boost::multiprecision::uint512_t;
		using cpp_int = boost::multiprecision::cpp_int;
		//�w��ȏ�̑傫���ōŏ��̎g�p�\�ȃL�[����������B������Ȃ�������0��Ԃ��B(�f��1�A�f��2�A�T���J�n��)
		uint1024 SearchAvalableEncodeKey(const uint512&, const uint512&, const uint1024&);
		unsigned int GetIntegerBit(const cpp_int&,unsigned int = 1024); //�����̃r�b�g�����擾(�Ώەϐ��A�ő�bit���B�v���Ώۂ�����𒴂��Ă�����0��Ԃ�)
		uint1024 CombineIntegers1024(const std::vector<uint64_t>&); //64bit�����Ȃ���������������1024bit�����Ȃ��������쐬�B(�z��퓬�|�C���^�A�z��T�C�Y)
		uint512 CombineIntegers512(const std::vector<uint64_t>&); //64bit�����Ȃ���������������512bit�����Ȃ��������쐬�B(�z��퓬�|�C���^�A�z��T�C�Y)
		unsigned char GetAssignedData1024(const uint1024&, size_t); //1024bit�����Ȃ������̎w��o�C�g�ڂ̃f�[�^���擾
		void SetAssignedData1024(uint1024&, size_t,unsigned char); //1024bit�����Ȃ������̎w��o�C�g�ڂ̃f�[�^���Z�b�g
	}
}