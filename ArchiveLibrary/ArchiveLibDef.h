#pragma once
#include<string>

namespace ArcLib{
	//以下の2つの値が異なる状態で生成されたアーカイブファイルは他の値のもとで生成されたファイルと互換性がありません。
	const unsigned int NAME_SIZE(64); //使用可能なファイル名のサイズ(Byte)
	const unsigned int ENCRYPT_SIZE(128); //暗号化するサイズ(Byte)
	enum class Mode{ SimpleBind = 0, Compress = 1, EXOREncrypt = 2 };
	unsigned int ConvertModeToUnsignedInt(Mode);
	std::string ConvertModeToString(Mode);
	Mode ConvertUnsignedIntToMode(unsigned int);
}