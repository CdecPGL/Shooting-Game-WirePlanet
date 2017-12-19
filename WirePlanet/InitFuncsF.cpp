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

//ファイル関係初期化関数群

namespace{
	std::string ANIMAIOTN_DATA_LIST("animation_list.csv");
}

//DXライブラリ関連の初期化後、すべてのファイルシステム利用関数の前に実行。
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
		printf("InitFuncs::InitFileSystem 初期化モードが不正です。(%d)\n", mode);
		return -5;
	}
	int res = flm.Init();
	if (res){
		//		printf("InitFileSystem::ファイルロードマネージャーの初期化に失敗しました。\n");
		return -1;
	}
	flm.LoadAllFile();
	//ゆーてィリtぃ関数を通して読み込んだデータは残しておく
	FileLoadUtil::SetLoadedDataTreat(FileLoadUtil::LoadedData::Remain);

	//画像とサウンド読み込み
	SoundManager::GetInstance().SetLoadMode(ml_mode == 0 ? SoundManager::LoadMode::AllInInit : SoundManager::LoadMode::InPlay);
	if (SoundManager::GetInstance().Init())return -2;
	if (GraphicManager::GetInstance().Init())return -3;

	//アクションデータ管理者に画像リストをセット
	ActionDataAdmin::GetInstance().SetRefGHandles(GraphicManager::GetInstance().GetGhandleList());
	ActionDataAdmin::GetInstance().SetRefGhandleFlag(true);

	//アニメーション読み込み
	if (LoadAnimationData() < 0){ return -4; }

	return 0;
}

//画像読み込み後に実行
int InitFuncs::LoadAnimationData(){
	using namespace AnimLib;
	using namespace cdec;
	AnimationDataAdmin& ada = AnimationDataAdmin::GetInstace();
	FileLoadManager& flm = FileLoadManager::GetInstance();
	ada.Init();
	ada.SetGHandles(GraphicManager::GetInstance().GetGhandleList());
	//リスト読み込み
	CSVLoader csvl;
	int res;
	res = FileLoadUtil::LoadCSVFromFLM(&csvl, ANIMAIOTN_DATA_LIST);
	if (res < 0){
		printf("InitFuncs::LoadAnimationData リストを読み込めませんでした。(reason:%d,name:%s)\n", res, ANIMAIOTN_DATA_LIST.c_str());
		return -1;
	}
	for (vector<string>& s : csvl){
		if (s.size() != 2){
			printf("InitFuncs::LoadAnimationData リストの項目数が不正です。(%d/2)\n", s.size());
			continue;
		}
		std::shared_ptr<File> f = flm.FindFile(s[1]);
		if (f == nullptr){
			printf("InitFuncs::LoadAnimationData ファイルが読み込めませんでした。(%s)\n", s[1].c_str());
			continue;
		}
		if (ada.LoadAnimationDataFromBinary(s[0], f->GetTopPointer(), f->GetSize()) < 0){
			printf("InitFuncs::LoadAnimationData アニメーションデータが読み込めませんでした。(%s%s)\n", s[0].c_str(), s[1].c_str());
			continue;
		}
	}
	printf("アニメーションデータを読み込みました。\n");
	return 0;
}

int InitFuncs::LoadSaveData(){
	//セーブデータの読み込み
	SaveData::GetInstance().SetPath("save_data.sav");
	return SaveData::GetInstance().Load();
}

int InitFuncs::SaveSaveData(){
	return SaveData::GetInstance().Save();
}

int InitFuncs::DeleteFileCash(){
	return cdec::FileLoadManager::GetInstance().UnloadAllFile();
}
