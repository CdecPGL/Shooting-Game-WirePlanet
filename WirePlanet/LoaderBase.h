#pragma once

#include<memory>
#include<string>
#include"CdecFile.h"

namespace cdec{
	class LoaderBase{
	public:
		LoaderBase(const std::string& p) :_path(p){}
		virtual ~LoaderBase() = default;
		virtual int Init() = 0;
		virtual std::shared_ptr<File> FindFile(const std::string&) = 0;
		virtual int UnloadAllFile() = 0; //全ファイルデータの破棄
		virtual int LoadAllFile() = 0; //全ファイルデータファイルの読み込み(読み込み済みのものはスキップ)
		virtual int UpdateFileList() = 0; //ファイルリストを更新(内部でUnloadAllFileが呼ばれます)
		virtual unsigned int GetUsingMemory()const = 0;
	protected:
		std::string _path;
	};
}