#pragma once
#include "LoaderBase.h"
#include<unordered_map>

class Extracter;

namespace cdec{
	class ArchiveLoader :
		public LoaderBase
	{
	public:
		ArchiveLoader(const std::string&);
		ArchiveLoader(const std::string&,unsigned int);
		~ArchiveLoader();
		int Init()override;
		int LoadAllFile()override;
		int UnloadAllFile()override;
		int UpdateFileList()override;
		void SetKey(unsigned int); //復号化キーセット
		std::shared_ptr<File> FindFile(const std::string&)override;
		unsigned int GetUsingMemory()const override;
	private:
		int LoadData(const std::string&,std::shared_ptr<File>);
		std::unordered_map<std::string, std::shared_ptr<File>> _files;
		std::unique_ptr<Extracter> _extracter;
		unsigned int _key; //復号化キー
	};
}
