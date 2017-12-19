#include "FileLoadManager.h"

using namespace cdec;

FileLoadManager::FileLoadManager()
{
	printf("FileLoadManager�̃C���X�^���X���쐬����܂����B\n");
}

FileLoadManager::~FileLoadManager()
{
	for (auto it : _loaders){
		delete it;
	}
	printf("FileLoadManager�̃C���X�^���X���j������܂����B\n");
}

FileLoadManager& FileLoadManager::GetInstance(){
	static FileLoadManager flm;
	return flm;
}

int FileLoadManager::Init(){
	for (auto it : _loaders){
		if (it->Init()){
			printf("FileLoadManager�̏������Ɏ��s���܂����B\n");
			return -1;
		}
	}
	printf("FileLoadManager������������܂����B\n");
	return 0;
}

void FileLoadManager::PushLoader(LoaderBase* lb){
	_loaders.push_back(lb);
}

std::shared_ptr<File> FileLoadManager::FindFile(const std::string& fn){
	std::shared_ptr<File> f;
	//�������烍�[�_�[�������A����������I��
	for (auto it = _loaders.rbegin(); it != _loaders.rend(); ++it){
		f = (*it)->FindFile(fn);
		if (f){ break; }
	}
	if (f == nullptr){
		printf("FileLoadManager::FindFile �t�@�C�������݂��Ȃ����ǂݍ��݃G���[���������܂����B(%s)\n", fn.c_str());
	}
	return f;
}

unsigned int FileLoadManager::GetUsingMemory()const{
	unsigned int res = 0;
	for (auto it : _loaders){
		res += it->GetUsingMemory();
	}
	return res;
}

int FileLoadManager::LoadAllFile(){
	int err = 0;
	for (auto it : _loaders){
		err = it->LoadAllFile();
	}
	return err;
}

int FileLoadManager::UnloadAllFile(){
	for (auto it : _loaders){
		it->UnloadAllFile();
	}
	return 0;
}

int FileLoadManager::ReloadAllFIle(){
	for (auto it : _loaders){
		it->UpdateFileList();
	}
	return LoadAllFile();
}