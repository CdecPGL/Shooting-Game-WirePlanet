#pragma once
#include"InitFuncs.h"


#include"FileLoadUtility.h"
#include"FileLoadManager.h"
#include"NormalFolderLoader.h"
#include"ArchiveLoader.h"
#include"GraphicManager.h"
#include"SoundManager.h"

#include"Action.h"
#include"Animation.h"
#include"SVLoader.h"

#include"SaveData.h"

//�t�@�C���֌W�������֐��Q

namespace{
	std::string ANIMAIOTN_DATA_LIST("animation_list.csv");
}

//DX���C�u�����֘A�̏�������A���ׂẴt�@�C���V�X�e�����p�֐��̑O�Ɏ��s�B
int InitFuncs::InitFileSystem(int mode,int ml_mode){
	using namespace cdec;
	FileLoadManager& flm = FileLoadManager::GetInstance();
	if (mode == 0){
		flm.PushLoader(new NormalFolderLoader("resources\\data"));
		flm.PushLoader(new NormalFolderLoader("resources\\test_data"));
	}
	else if (mode == 1){
		flm.PushLoader(new ArchiveLoader("resources\\data.arc", 48209));
	}
	else{
		printf("InitFuncs::InitFileSystem ���������[�h���s���ł��B(%d)\n", mode);
		return -5;
	}
	int res = flm.Init();
	if (res){
		//		printf("InitFileSystem::�t�@�C�����[�h�}�l�[�W���[�̏������Ɏ��s���܂����B\n");
		return -1;
	}
	flm.LoadAllFile();
	//��[�ăB��t���֐���ʂ��ēǂݍ��񂾃f�[�^�͎c���Ă���
	FileLoadUtil::SetLoadedDataTreat(FileLoadUtil::LoadedData::Remain);

	//�摜�ƃT�E���h�ǂݍ���
	SoundManager::GetInstance().SetLoadMode(ml_mode == 0 ? SoundManager::LoadMode::AllInInit : SoundManager::LoadMode::InPlay);
	if (SoundManager::GetInstance().Init())return -2;
	if (GraphicManager::GetInstance().Init())return -3;

	//�A�N�V�����f�[�^�Ǘ��҂ɉ摜���X�g���Z�b�g
	ActionDataAdmin::GetInstance().SetRefGHandles(GraphicManager::GetInstance().GetGhandleList());
	ActionDataAdmin::GetInstance().SetRefGhandleFlag(true);

	//�A�j���[�V�����ǂݍ���
	if (LoadAnimationData() < 0){ return -4; }

	return 0;
}

//�摜�ǂݍ��݌�Ɏ��s
int InitFuncs::LoadAnimationData(){
	using namespace AnimLib;
	using namespace cdec;
	AnimationDataAdmin& ada = AnimationDataAdmin::GetInstace();
	FileLoadManager& flm = FileLoadManager::GetInstance();
	ada.Init();
	ada.SetGHandles(GraphicManager::GetInstance().GetGhandleList());
	//���X�g�ǂݍ���
	CSVLoader csvl;
	int res;
	res = FileLoadUtil::LoadCSVFromFLM(&csvl, ANIMAIOTN_DATA_LIST);
	if (res < 0){
		printf("InitFuncs::LoadAnimationData ���X�g��ǂݍ��߂܂���ł����B(reason:%d,name:%s)\n", res, ANIMAIOTN_DATA_LIST.c_str());
		return -1;
	}
	for (vector<string>& s : csvl){
		if (s.size() != 2){
			printf("InitFuncs::LoadAnimationData ���X�g�̍��ڐ����s���ł��B(%d/2)\n", s.size());
			continue;
		}
		std::shared_ptr<File> f = flm.FindFile(s[1]);
		if (f == nullptr){
			printf("InitFuncs::LoadAnimationData �t�@�C�����ǂݍ��߂܂���ł����B(%s)\n", s[1].c_str());
			continue;
		}
		if (ada.LoadAnimationDataFromBinary(s[0], f->GetTopPointer(), f->GetSize()) < 0){
			printf("InitFuncs::LoadAnimationData �A�j���[�V�����f�[�^���ǂݍ��߂܂���ł����B(%s%s)\n", s[0].c_str(), s[1].c_str());
			continue;
		}
	}
	printf("�A�j���[�V�����f�[�^��ǂݍ��݂܂����B\n");
	return 0;
}

int InitFuncs::LoadSaveData(){
	//�Z�[�u�f�[�^�̓ǂݍ���
	SaveData::GetInstance().SetPath("save_data.sav");
	return SaveData::GetInstance().Load();
}

int InitFuncs::SaveSaveData(){
	return SaveData::GetInstance().Save();
}

int InitFuncs::DeleteFileCash(){
	return cdec::FileLoadManager::GetInstance().UnloadAllFile();
}
