#pragma once

#include<string>
#include<vector>

namespace cdec{
	//ファイルの状態(利用可能、暗号化されている、ロードされていない、問題発生)
	enum class FileStatus{ Available, Encrypted, Unloaded, Error };

	class File
	{
	public:
		File();
		File(const std::string&); //拡張子セット版コンストラクタ
		~File();
		std::string GetExtension()const;
		void UnloadData();
		unsigned int GetSize()const;
		const void* GetTopPointer()const;
		FileStatus GetStatus()const;
		void SetData(void*, unsigned int, bool = false); //データセット(先頭アドレス、サイズ、暗号化データか)
		void ErrorOccured(); //エラー発生

	private:
		const std::string extension;
		FileStatus status = FileStatus::Unloaded;
		void* data_top = nullptr;
		unsigned int size = 0;
	};
}
