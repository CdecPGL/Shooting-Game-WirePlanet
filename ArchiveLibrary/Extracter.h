#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<memory>
#include<unordered_map>
#include"ArchiveLibDef.h"

//アーカイブファイルからファイルを抽出するクラス

namespace ArcLib{
	class EXOREncKey;
}

class Extracter{
public:
	Extracter();
	~Extracter();
	int SetEXOREncryptionKey(uint16_t); //排他的論理和暗号化キーセット
	int OpenAchiveFile(const std::string&); //アーカイブファイルを開く
	int CloseArchiveFile(); //アーカイブファイルを閉じる
	std::vector<std::string> GetFileList()const; //ファイルリスト取得
	std::pair<void*,unsigned int> GetFile(const std::string&); //ファイルを取得する(この関数で取得したデータは使用後にdelete[]で各自削除する。)

private:
	std::ifstream _ifs;
	bool _extracting;
	struct Header{
		_Uint32t num = 0;
		_Uint32t header_size = 0;
		struct FileInfo{
			_Uint32t address = 0;
			_Uint32t org_size = 0;
			_Uint32t prc_size = 0;
		};
		std::unordered_map<std::string,std::unique_ptr<FileInfo>> file_infos;
		void clear();
	};
	Header _header;
	ArcLib::Mode _mode;

	//EXOR暗号化関連
	std::unique_ptr<ArcLib::EXOREncKey> _exor_enc_key;

	//入力したデータと異なるデータを出力する場合は入力データを関数内でdeleteする。(入力データ、入力サイズ、出力サイズ)
	void* get_simplebind(void*,unsigned int,unsigned int);
	void* get_compress(void*, unsigned int,unsigned int);
	void* get_exorenc(void*, unsigned int, unsigned int);
};