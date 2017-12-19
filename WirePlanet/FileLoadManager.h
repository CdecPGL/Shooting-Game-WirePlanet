#pragma once
#include"LoaderBase.h"
#include<string>
#include<vector>

namespace cdec{
	class FileLoadManager
	{
	public:
		FileLoadManager(const FileLoadManager&) = delete;
		~FileLoadManager();
		static FileLoadManager& GetInstance();
		int Init();
		void PushLoader(LoaderBase*); //ローダーをセット。開放はFileLoadManagerが行う。初期化前に呼び出す必要がある。
		std::shared_ptr<File> FindFile(const std::string&);
		int LoadAllFile();
		int UnloadAllFile();
		int ReloadAllFIle();
		unsigned int GetUsingMemory()const;
	private:
		FileLoadManager();
		std::vector<LoaderBase*> _loaders;
	};
}