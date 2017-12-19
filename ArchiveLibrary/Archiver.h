#pragma once
/*
Version1.0 2014/09/06
Version2.0 2014/09/06 圧縮に対応。簡易暗号化に対応
Version2.1 2015/08/11 ライブラリファイル読み込みのpragma comment追加
*/
#include<string>
#include<fstream>
#include<vector>
#include<memory>
#include"ArchiveLibDef.h"
//複数のデータからアーカイブ・ファイルを作成するクラス
/*アーカイブファイルの構造について
先頭から
ヘッダー{
モード:1Byte 0:単純結合 1:zlib圧縮+結合 2:zlib圧縮+結合+各データの先頭を暗号化
データ部サイズ4Byte
}
データ部{
<[データ]*データの個数分>
}
フッター{
データの個数:4Byte
<[データ名:64Byte,先頭アドレス4Byte,元データサイズ:4Byte 保存サイズ:4Byte]*データの個数分>
}

<>内は圧縮対象
データ名はアルファベットで。
先頭アドレスはデータの記述部分先頭を0とし、バイト単位で表す。
*/

/*各モードについて
0:SimpleBind
単純連結出力。
データを単純に結合して出力する。
1:Compress
圧縮出力。圧縮対象部(データ部とフッターのデータ数記録位置以外)をzlibで圧縮して記録する。
2:EXOREncrypt
排他的論理和暗号化出力
圧縮対象部を圧縮し、さらにヘッダのモード記憶域以降をすべて排他的論理和により暗号化
暗号化について。
キーは16bitのキーを上8ビット、下8ビットに分けて使用。
データを１バイトづつキーと排他的論理和演算していく。
この時のキーは上8ビットの数値からスタートし、1バイト暗号化するごとに下8ビット分足される。
例えばキー0x1234で下の平文を暗号化する場合は暗号化するば場合、
AA BB CC DD
は
AA^(12) BB^(12+34) CC^(12+34+34) DD^(12+34+34+34)
となる。
*/

namespace ArcLib{
	class EXOREncKey;
}

class Archiver{
public:
	Archiver();
	~Archiver();
	int SetEXOREncryptionKey(uint16_t); //排他的論理和暗号化キーセット
	int BeginArchive(const std::string&,ArcLib::Mode,unsigned int=6); //アーカイブ開始(出力ファイル、出力モード、圧縮レベル(1-9))
	int EndArchive(); //アーカイブ終了
	int InsertData(const std::string&,const void*,unsigned int); //データ挿入(BeginArchiveの実行後、EndArchiveの実行前に呼ぶ。この関数の使用後にデータは消してもOK)
private:
	bool _archiving;
	std::ofstream _ofs;
	struct Header{
		_Uint32t num=0;
		_Uint32t total_data_size = 0;
		struct FileInfo{
			char name[ArcLib::NAME_SIZE];
			_Uint32t address=0;
			_Uint32t org_size=0; //元サイズ
			_Uint32t prc_size = 0; //処理後サイズ
		};
		std::vector<std::unique_ptr<FileInfo>> file_infos;
		void clear();
	};

	Header _header;
	ArcLib::Mode _mode;

	unsigned int _comp_level; //圧縮度

	//EXOR暗号化関連
	std::unique_ptr<ArcLib::EXOREncKey> _exor_enc_key;

	//データ書き込み関連
	_Uint32t write_simplebind(const void*,unsigned int);
	_Uint32t write_compress(const void*, unsigned int);
	_Uint32t write_exorenc(const void*, unsigned int);
};
