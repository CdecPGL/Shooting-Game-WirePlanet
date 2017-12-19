#pragma once
#include <cstdint>

namespace ArcLib{
	class EXOREncKey{
	public:
		EXOREncKey();
		~EXOREncKey();
		void SetKey(uint16_t);
		void ResetCurrentKey(); //現在のキーを最初に戻す
		void ExorEncryptData(void*, unsigned int); //EXOR暗号化
		void SeekCurrentExorKey(int); //指定バイト分EXOR暗号化キーを移動
		unsigned char ExorEncrypt(unsigned char); //EXOR暗号化
	private:
		uint16_t _exor_enc_key; //EXOR暗号化モードキー
		unsigned char _exor_enc_key_h; //EXOR暗号化キー上位8ビット
		unsigned char _exor_enc_key_l; //EXOR暗号化キー下位8ビット
		unsigned char _current_exor_key; //現在のEXOR暗号化キー
	};
}