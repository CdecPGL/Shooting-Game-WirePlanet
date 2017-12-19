#pragma once
#include "LoaderBase.h"
#include<unordered_map>

namespace cdec{
	class NormalFolderLoader :
		public LoaderBase
	{
	public:
		NormalFolderLoader(const std::string&);
		~NormalFolderLoader()override;
		int Init()override;
		int LoadAllFile()override;
		int UnloadAllFile()override;
		int UpdateFileList()override;
		std::shared_ptr<File> FindFile(const std::string&)override;
		unsigned int GetUsingMemory()const override;
	private:
		std::unordered_map<std::string, std::pair<std::string, std::shared_ptr<File>>> _files; //ファイルリスト(<ファイル名<ファイルパス,ファイルポインタ>>)
		static int LoadFile(std::shared_ptr<File>, const std::string&);
	};
}
