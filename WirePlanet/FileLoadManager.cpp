#include "FileLoadManager.h"

using namespace cdec;

FileLoadManager::FileLoadManager()
{
	printf("FileLoadManagerのインスタンスが作成されました。\n");
}

FileLoadManager::~FileLoadManager()
{
	for (auto it : _loaders){
		delete it;
	}
	printf("FileLoadManagerのインスタンスが破棄されました。\n");
}

FileLoadManager& FileLoadManager::GetInstance(){
	static FileLoadManager flm;
	return flm;
}

int FileLoadManager::Init(){
	for (auto it : _loaders){
		if (it->Init()){
			printf("FileLoadManagerの初期化に失敗しました。\n");
			return -1;
		}
	}
	printf("FileLoadManagerが初期化されました。\n");
	return 0;
}

void FileLoadManager::PushLoader(LoaderBase* lb){
	_loaders.push_back(lb);
}

std::shared_ptr<File> FileLoadManager::FindFile(const std::string& fn){
	std::shared_ptr<File> f;
	//末尾からローダーを試し、成功したら終了
	for (auto it = _loaders.rbegin(); it != _loaders.rend(); ++it){
		f = (*it)->FindFile(fn);
		if (f){ break; }
	}
	if (f == nullptr){
		printf("FileLoadManager::FindFile ファイルが存在しないか読み込みエラーが発生しました。(%s)\n", fn.c_str());
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