#pragma once

#include<string>
#include<vector>

namespace cdec{
	//�t�@�C���̏��(���p�\�A�Í�������Ă���A���[�h����Ă��Ȃ��A��蔭��)
	enum class FileStatus{ Available, Encrypted, Unloaded, Error };

	class File
	{
	public:
		File();
		File(const std::string&); //�g���q�Z�b�g�ŃR���X�g���N�^
		~File();
		std::string GetExtension()const;
		void UnloadData();
		unsigned int GetSize()const;
		const void* GetTopPointer()const;
		FileStatus GetStatus()const;
		void SetData(void*, unsigned int, bool = false); //�f�[�^�Z�b�g(�擪�A�h���X�A�T�C�Y�A�Í����f�[�^��)
		void ErrorOccured(); //�G���[����

	private:
		const std::string extension;
		FileStatus status = FileStatus::Unloaded;
		void* data_top = nullptr;
		unsigned int size = 0;
	};
}
