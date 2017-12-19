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
		virtual int UnloadAllFile() = 0; //�S�t�@�C���f�[�^�̔j��
		virtual int LoadAllFile() = 0; //�S�t�@�C���f�[�^�t�@�C���̓ǂݍ���(�ǂݍ��ݍς݂̂��̂̓X�L�b�v)
		virtual int UpdateFileList() = 0; //�t�@�C�����X�g���X�V(������UnloadAllFile���Ă΂�܂�)
		virtual unsigned int GetUsingMemory()const = 0;
	protected:
		std::string _path;
	};
}