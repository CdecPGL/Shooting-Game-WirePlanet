#pragma once
#include<string>

namespace ArcLib{
	//�ȉ���2�̒l���قȂ��ԂŐ������ꂽ�A�[�J�C�u�t�@�C���͑��̒l�̂��ƂŐ������ꂽ�t�@�C���ƌ݊���������܂���B
	const unsigned int NAME_SIZE(64); //�g�p�\�ȃt�@�C�����̃T�C�Y(Byte)
	const unsigned int ENCRYPT_SIZE(128); //�Í�������T�C�Y(Byte)
	enum class Mode{ SimpleBind = 0, Compress = 1, EXOREncrypt = 2 };
	unsigned int ConvertModeToUnsignedInt(Mode);
	std::string ConvertModeToString(Mode);
	Mode ConvertUnsignedIntToMode(unsigned int);
}