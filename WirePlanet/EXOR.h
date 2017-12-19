#pragma once

//�r���I�_���a��p�����Í��A������B(�쐬��)

#include"boost/multiprecision/cpp_int.hpp"

namespace cdec{
	class EXOR{
	public:
		using cpp_int = boost::multiprecision::cpp_int;
		EXOR();
		~EXOR();
		int SetEncryptUnitSize(unsigned int); //�Í����P�ʃT�C�Y(�o�C�g�B1024�܂�)
		int SetKey(const cpp_int&); //�L�[�Z�b�g
		cpp_int Encrypt(const cpp_int&); //�Í���
		cpp_int Decrypt(const cpp_int&); //������
	private:
		cpp_int _key; //�T�C�Y���Í����P�ʃT�C�Y��菬��������
		unsigned int _enc_unit_size; //�Í����P�ʃT�C�Y(�o�C�g)
	};
}