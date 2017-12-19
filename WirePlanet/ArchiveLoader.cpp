#include "ArchiveLoader.h"
#include"Extracter.h"
#include"boost/filesystem/path.hpp"

namespace cdec{

	ArchiveLoader::ArchiveLoader(const std::string& path) :LoaderBase(path), _extracter(std::make_unique<Extracter>()), _key(0)
	{
	}

	ArchiveLoader::ArchiveLoader(const std::string& path, unsigned int k) : LoaderBase(path), _extracter(std::make_unique<Extracter>()), _key(k)
	{
	}

	ArchiveLoader::~ArchiveLoader()
	{
		_extracter->CloseArchiveFile();
	}

	int ArchiveLoader::Init(){
		if (_extracter->SetEXOREncryptionKey((uint16_t)_key)){
			printf("ArchiveLoaderの初期化に失敗しました。復号化キーの設定に失敗しました。(%s)\n", _path.c_str());
		}
		int res = _extracter->OpenAchiveFile(_path);
		if (res < 0){
			printf("ArchiveLoaderの初期化に失敗しました。(reason:%d,path:%s)\n", res, _path.c_str());
		}
		//ファイルリスト取得
		std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		for (auto& fn : f_list){
			boost::filesystem::path p(fn);
			_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
		}
		printf("ArchiveLoaderが初期化されました。(パス:%s,ファイル数:%d)\n", _path.c_str(), (int)_files.size());
		return 0;
	}

	int ArchiveLoader::LoadAllFile(){
		int err = 0;
		for (auto& f : _files){
			if (f.second->GetStatus() == FileStatus::Available){ continue; }
			err = LoadData(f.first, f.second);
			if (err){
				printf("ArchiveLoader::LoadAllFile ファイルの読み込みに失敗しました。(name:%s)\n", f.first.c_str());

			}
		}
		return err;
	}

	int ArchiveLoader::UnloadAllFile(){
		for (auto& f : _files){
			f.second->UnloadData();
		}
		return 0;
	}

	int ArchiveLoader::UpdateFileList(){
		UnloadAllFile();
		_extracter->CloseArchiveFile();
		_files.clear();
		int res = _extracter->OpenAchiveFile(_path);
		if (res < 0){
			printf("ArchiveLoader::UpdateFileList アーカイブファイルのオープンに失敗しました。(reason:%d,path:%s)\n", res, _path.c_str());
		}
		//ファイルリスト取得
		std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		for (auto& fn : f_list){
			boost::filesystem::path p(fn);
			_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
		}
		return 0;
	}

	std::shared_ptr<File> ArchiveLoader::FindFile(const std::string& fn){
		auto it = _files.find(fn);
		if (it == _files.end()){ return nullptr; } //なかったら塗る
		if ((*it).second->GetStatus() == FileStatus::Available){ return (*it).second; } //読み込み済みだったら返す
		if (LoadData(fn,(*it).second)){ //読み込み失敗したら塗る
			it->second->ErrorOccured();
			return nullptr;
		}
		else{ //読み込んだら返す
			return it->second;
		}
	}

	int ArchiveLoader::LoadData(const std::string& fn,std::shared_ptr<File> file){
		auto data = _extracter->GetFile(fn);
		if (data.first == nullptr){
			return -1;
		}
		file->SetData(data.first, data.second);
		return 0;
	}

	unsigned int ArchiveLoader::GetUsingMemory()const{
		unsigned int res = 0;
		for (auto& it : _files){
			res += it.second->GetSize();
		}
		return res;
	}

	void ArchiveLoader::SetKey(unsigned int k){ _key = k; }
}
