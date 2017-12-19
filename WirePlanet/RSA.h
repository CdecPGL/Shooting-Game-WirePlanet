#pragma once

#include"boost/multiprecision/cpp_int.hpp"
#include"RSAUtility.h"

/*RSA�Í��N���X�̎g�p�@
�Í���
1.����ȑf����2�p�ӂ��Z�b�g����B
2.�����ȃG���R�[�h�L�[���Z�b�g����B
3.�f�R�[�h�L�[�ƃ��W���������������̂Ŏ擾���ۑ����Ă����B
3.5 ���̓G���R�[�h�L�[�ƃ��W�������Z�b�g����B
4.�Í�������B
5.���W�����ƃG���R�[�h�L�[�����J���Ƃ���B
6.�͂��߂Ɏg�p���������2�̑f���͔j������B
7.�f�R�[�h�L�[�͔閧���Ƃ��ĕۑ����Ă����B
������
1.���W�����ƃf�R�[�h�L�[���Z�b�g����B
2.����������B
*/
/*�g�p��̒���
���̃N���X�Ɋe�p�����[�^�̐����������؂���@�\�͖����̂Ŏg�p�O�Ɉȉ��̏������m�F����K�v������B(�G���R�[�h�L�[�̓`�F�b�N�����)
1.���ƂȂ�f��p,q��3�ȏ�łȂ���΂Ȃ�Ȃ��B
2.�G���R�[�h�L�[��2�ȏ�ł���(p-1)(q-1)�ƌ݂��ɑf�łȂ���΂Ȃ�Ȃ��B
3.�G���R�[�h�L�[��(p-1)(q-1)�����B
v1�ł�512bit�̑f��(1.340781e+154����)���g�p����
*/

namespace cdec{
	class RSA{
	public:
		RSA();
		~RSA();
		//���̑f���Q���Z�b�g
		void SetPrimeNumbers(const RSAUtil::uint512&, const RSAUtil::uint512&);
		void SetPrimeNumbers(RSAUtil::uint512&&, RSAUtil::uint512&&);
		//�G���R�[�h�L�[�Z�b�g(�߂�l�������ŃG���[)
		int SetEncodeKey(const RSAUtil::uint1024&);
		int SetEncodeKey(RSAUtil::uint1024&&);
		//���W�����Z�b�g
		void SetModulo(const RSAUtil::uint1024&);
		void SetModulo(RSAUtil::uint1024&&);
		//�f�R�[�h�L�[�Z�b�g
		void SetDecodeKey(const RSAUtil::uint1024&);
		void SetDecodeKey(RSAUtil::uint1024&&);
		//�G���R�[�h�L�[�擾
		const RSAUtil::uint1024& GetEncodeKey()const;
		//���W�����擾
		const RSAUtil::uint1024& GetModulo()const;
		//�f�R�[�h�L�[�擾
		const RSAUtil::uint1024& GetDecodeKey()const;
		//�Z�L�����e�B�p�����[�^�擾(1024)
		unsigned int GetSecurityParameter()const;
		//���p�\�ȃf�[�^�̍ő�T�C�Y�擾
		unsigned int GetAvailableDataMaxSize()const;

		//�Í���(���s������0��Ԃ�)
		RSAUtil::uint1024 Encrypt(const RSAUtil::uint1024&);
//		RSAUtil::uint1024 Encrypt(RSAUtil::uint1024&&);
		//������(���s������0��Ԃ�)
		RSAUtil::uint1024 Decrypt(const RSAUtil::uint1024&);
//		RSAUtil::uint1024 Decrypt(RSAUtil::uint1024&&);
	private:
		RSAUtil::uint512 _prime_number_1;
		RSAUtil::uint512 _prime_number_2;
		RSAUtil::uint1024 _encode_key;
		RSAUtil::uint1024 _modulo;
		RSAUtil::uint1024 _decode_key;
		unsigned int _available_data_max_size; //���p�\�f�[�^�ő�T�C�Y(�o�C�g)

		//�������p�o�b�t�@

		void CalculateModulo();
		void CalculateDecodeKey();
		void CalculateAvailableDataMaxSize(); //���p�\�f�[�^�ő�T�C�Y(�o�C�g)�����߂�

		//��]�����߂�(�K�p�ΏہA�搔�A���W����)
		static RSAUtil::uint1024 PowMod(const RSAUtil::uint1024&, const RSAUtil::uint1024&, const RSAUtil::uint1024&);
	};
}