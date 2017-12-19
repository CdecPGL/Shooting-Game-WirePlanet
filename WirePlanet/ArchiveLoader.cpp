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
			printf("ArchiveLoader�̏������Ɏ��s���܂����B�������L�[�̐ݒ�Ɏ��s���܂����B(%s)\n", _path.c_str());
		}
		int res = _extracter->OpenAchiveFile(_path);
		if (res < 0){
			printf("ArchiveLoader�̏������Ɏ��s���܂����B(reason:%d,path:%s)\n", res, _path.c_str());
		}
		//�t�@�C�����X�g�擾
		std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		for (auto& fn : f_list){
			boost::filesystem::path p(fn);
			_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
		}
		printf("ArchiveLoader������������܂����B(�p�X:%s,�t�@�C����:%d)\n", _path.c_str(), (int)_files.size());
		return 0;
	}

	int ArchiveLoader::LoadAllFile(){
		int err = 0;
		for (auto& f : _files){
			if (f.second->GetStatus() == FileStatus::Available){ continue; }
			err = LoadData(f.first, f.second);
			if (err){
				printf("ArchiveLoader::LoadAllFile �t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(name:%s)\n", f.first.c_str());

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
			printf("ArchiveLoader::UpdateFileList �A�[�J�C�u�t�@�C���̃I�[�v���Ɏ��s���܂����B(reason:%d,path:%s)\n", res, _path.c_str());
		}
		//�t�@�C�����X�g�擾
		std::vector<std::string> f_list = std::move(_extracter->GetFileList());
		for (auto& fn : f_list){
			boost::filesystem::path p(fn);
			_files.emplace(std::make_pair(fn, std::make_shared<File>(p.extension().string())));
		}
		return 0;
	}

	std::shared_ptr<File> ArchiveLoader::FindFile(const std::string& fn){
		auto it = _files.find(fn);
		if (it == _files.end()){ return nullptr; } //�Ȃ�������h��
		if ((*it).second->GetStatus() == FileStatus::Available){ return (*it).second; } //�ǂݍ��ݍς݂�������Ԃ�
		if (LoadData(fn,(*it).second)){ //�ǂݍ��ݎ��s������h��
			it->second->ErrorOccured();
			return nullptr;
		}
		else{ //�ǂݍ��񂾂�Ԃ�
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
