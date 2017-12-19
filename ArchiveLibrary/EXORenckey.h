#pragma once
#include <cstdint>

namespace ArcLib{
	class EXOREncKey{
	public:
		EXOREncKey();
		~EXOREncKey();
		void SetKey(uint16_t);
		void ResetCurrentKey(); //���݂̃L�[���ŏ��ɖ߂�
		void ExorEncryptData(void*, unsigned int); //EXOR�Í���
		void SeekCurrentExorKey(int); //�w��o�C�g��EXOR�Í����L�[���ړ�
		unsigned char ExorEncrypt(unsigned char); //EXOR�Í���
	private:
		uint16_t _exor_enc_key; //EXOR�Í������[�h�L�[
		unsigned char _exor_enc_key_h; //EXOR�Í����L�[���8�r�b�g
		unsigned char _exor_enc_key_l; //EXOR�Í����L�[����8�r�b�g
		unsigned char _current_exor_key; //���݂�EXOR�Í����L�[
	};
}