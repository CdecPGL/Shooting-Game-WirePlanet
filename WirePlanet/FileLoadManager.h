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
		void PushLoader(LoaderBase*); //���[�_�[���Z�b�g�B�J����FileLoadManager���s���B�������O�ɌĂяo���K�v������B
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