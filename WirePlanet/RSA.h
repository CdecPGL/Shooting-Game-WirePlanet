#pragma once

#include"boost/multiprecision/cpp_int.hpp"
#include"RSAUtility.h"

/*RSA暗号クラスの使用法
暗号化
1.巨大な素数を2つ用意しセットする。
2.正当なエンコードキーをセットする。
3.デコードキーとモジュロが生成されるので取得し保存しておく。
3.5 又はエンコードキーとモジュロをセットする。
4.暗号化する。
5.モジュロとエンコードキーを公開鍵とする。
6.はじめに使用した巨大な2つの素数は破棄する。
7.デコードキーは秘密鍵として保存しておく。
復号化
1.モジュロとデコードキーをセットする。
2.復号化する。
*/
/*使用上の注意
このクラスに各パラメータの正当性を検証する機能は無いので使用前に以下の条件を確認する必要がある。(エンコードキーはチェックされる)
1.元となる素数p,qは3以上でなければならない。
2.エンコードキーは2以上でかつ(p-1)(q-1)と互いに素でなければならない。
3.エンコードキーは(p-1)(q-1)未満。
v1では512bitの素数(1.340781e+154未満)を使用する
*/

namespace cdec{
	class RSA{
	public:
		RSA();
		~RSA();
		//元の素数２つをセット
		void SetPrimeNumbers(const RSAUtil::uint512&, const RSAUtil::uint512&);
		void SetPrimeNumbers(RSAUtil::uint512&&, RSAUtil::uint512&&);
		//エンコードキーセット(戻り値が負数でエラー)
		int SetEncodeKey(const RSAUtil::uint1024&);
		int SetEncodeKey(RSAUtil::uint1024&&);
		//モジュロセット
		void SetModulo(const RSAUtil::uint1024&);
		void SetModulo(RSAUtil::uint1024&&);
		//デコードキーセット
		void SetDecodeKey(const RSAUtil::uint1024&);
		void SetDecodeKey(RSAUtil::uint1024&&);
		//エンコードキー取得
		const RSAUtil::uint1024& GetEncodeKey()const;
		//モジュロ取得
		const RSAUtil::uint1024& GetModulo()const;
		//デコードキー取得
		const RSAUtil::uint1024& GetDecodeKey()const;
		//セキュリティパラメータ取得(1024)
		unsigned int GetSecurityParameter()const;
		//利用可能なデータの最大サイズ取得
		unsigned int GetAvailableDataMaxSize()const;

		//暗号化(失敗したら0を返す)
		RSAUtil::uint1024 Encrypt(const RSAUtil::uint1024&);
//		RSAUtil::uint1024 Encrypt(RSAUtil::uint1024&&);
		//復号化(失敗したら0を返す)
		RSAUtil::uint1024 Decrypt(const RSAUtil::uint1024&);
//		RSAUtil::uint1024 Decrypt(RSAUtil::uint1024&&);
	private:
		RSAUtil::uint512 _prime_number_1;
		RSAUtil::uint512 _prime_number_2;
		RSAUtil::uint1024 _encode_key;
		RSAUtil::uint1024 _modulo;
		RSAUtil::uint1024 _decode_key;
		unsigned int _available_data_max_size; //利用可能データ最大サイズ(バイト)

		//高速化用バッファ

		void CalculateModulo();
		void CalculateDecodeKey();
		void CalculateAvailableDataMaxSize(); //利用可能データ最大サイズ(バイト)を求める

		//剰余を求める(適用対象、乗数、モジュロ)
		static RSAUtil::uint1024 PowMod(const RSAUtil::uint1024&, const RSAUtil::uint1024&, const RSAUtil::uint1024&);
	};
}