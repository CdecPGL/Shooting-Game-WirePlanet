#include"InitFuncs.h"
//Factories
#include"CharacterFactory.h"
#include"SceneFactory.h"
#include"EffectFactory.h"
//Managers
#include"EnemyMoverManager.h"
#include"EffectManager.h"
#include"CharacterManager.h"
#include"SoundManager.h"
#include"GraphicManager.h"
#include"MassageWindowManager.h"
#include"EventManager.h"
#include"Action.h"
#include"LaserManager.h"
#include"ScreenManager.h"
#include"FileLoadManager.h"
//Others
#include"EnemyGenerator.h"
#include"InitDatas.h"
#include"boost/lexical_cast.hpp"
#include"Key.h"
#include<ctime>
#include"GameInitializer.h"
#include"GPIUtil.h"
#include"VSYNCManager.h"
#include"SystemVariable.h"
#include"resource.h"

namespace {
	constexpr char* INI_FILE = "wireplanet.ini";
}
int InitFuncs::InitGame(){
	// 起動日時取得
	time_t now = time(NULL);
	tm *pnow = new tm;
	localtime_s(pnow, &now);
	int y(pnow->tm_year + 1900), m(pnow->tm_mon + 1), d(pnow->tm_mday), h(pnow->tm_hour), mi(pnow->tm_min), s(pnow->tm_sec);
	delete pnow;
	//起動日時出力
	printf("起動日時 > %d/%d/%d %d:%d:%d\n", y, m, d, h, mi, s);
	//バージョン
	printf("バージョン > %s\n", SysVar::VERSION_STRING);
	//セーブデータ読み込み
	int ret = LoadSaveData();
	if (ret < 0){
		printf("セーブデータの読み込みに失敗しました。(%d)\n",ret);
		return -5;
	}
	else if (ret == 1){
		printf("セーブデータが存在しないため初回起動とします。\n");
	}
	else{
		printf("セーブデータを読み込みました。\n");
	}
	//起動回数加算
	if (ret == 1){ //初回起動時
		printf("初回起動です。\n");
		GPIUtil::SetFirstPlayDate((unsigned)y, (unsigned)m, (unsigned)d, (unsigned)h, (unsigned)mi, (unsigned)s);
	}
	GPIUtil::AddPlayCount();
	//垂直同期の設定
	ret = VSYNCManager::GetInstance().Init();
	if (ret < 0){
		printf("画面の垂直同期設定に失敗しました。\n");
	}
	else if (ret == 1){
		printf("画面の垂直同期を無視します。\n");
	}
	else{
		printf("画面の垂直同期に描画タイミングを合わせます。\n");
	}
	//DXライブラリ初期化
	if (InitDxLibrary()){
		printf("Dxライブラリの初期化に失敗しました。\n");
		return -1;
	}
	//ファイルシステム設定
	try{
		int file_mode;
		try{
			file_mode = boost::lexical_cast<int>(InitDatas::GetInitDatas("System").at("FileMode"));
		}
		catch (out_of_range& e){
			//System->FIleModeがなかったら1(アーカイブからに設定)
			printf("iniファイルにSystem->FileModeが設定されていないので1とします。(%s)\n", e.what());
			file_mode = 1;
		}
		std::string music_load_mode = InitDatas::GetInitDatas("System").at("MusicLoadMode");
		if (InitFileSystem(file_mode, boost::lexical_cast<int>(music_load_mode))){
			printf("ファイルシステムの初期化に失敗しました。\n");
			return -2;
		}
	}
	catch (InitDatas::init_datas_error& e){
		printf("初期化データのセクションが不足しています。(%s)\n", e.what());
		return -2;
	}
	catch (out_of_range& e){
		printf("初期データが不足しています。(%s)\n", e.what());
		return -2;
	}
	catch (boost::bad_lexical_cast& e){
		printf("\"%s\"に無効なデータが含まれています。(%s)\n", INI_FILE, e.what());
		return -2;
	}
	if (InitFactory()){
		printf("ファクトリの初期化に失敗しました。\n");
		return -3;
	}
	if (PrepareManager()){
		printf("マネージャの準備に失敗しました。\n");
		return -4;
	}
	//フォントの読み込み
	auto file = cdec::FileLoadManager::GetInstance().FindFile(SysVar::GAME_FONT_FILE_NAME);
	if (file == nullptr || file->GetStatus() != cdec::FileStatus::Available) {
		printf("フォントファイルの読み込みに失敗しましたが、続行します。\n");
	} else {
		DWORD fn;
		if (AddFontMemResourceEx(reinterpret_cast<LPVOID>(const_cast<void*>(file->GetTopPointer())), file->GetSize(), nullptr, &fn) == 0) {
			printf("フォントの読み込みに失敗しましたが、続行します。\n");
		}
	}
	//ファイルシステムのキャッシュ削除
	DeleteFileCash();
	return 0;
}

int InitFuncs::EndGame(int mode){
	switch (mode){
	case 0: //通常終了
		break;
	case 1: //異常終了
		GPIUtil::AddErrorCount();
		break;
	case 2: //不正終了
		GPIUtil::AddCheatCount();
		break;
	}
	//ゲームプレイ時間加算
	GPIUtil::AddPlayTime(Counter::GetInstance().GetFrameFromStart(), Counter::GetInstance().GetTimeFromStartBySecond());

	SoundManager::GetInstance().End();
	ScreenManager::GetInstance().End();
	InitGraph(); //画像データ初期化
	DxLib_End(); // DXライブラリ終了処理
	int ret = SaveSaveData();
	if (ret < 0){
		printf("セーブデータを保存できませんでした。(%d)\n", ret);
	}
	else{
		printf("セーブデータを保存しました。\n");
	}
#ifdef _DEBUG
	FreeConsole();
#endif
	return ret;
}

int InitFuncs::InitDxLibrary(){
	try{
		InitDatas::LoadInitFile();
		printf("初期データを読み込みました。\n");
	}
	catch (InitDatas::init_datas_error& e){
		printf("初期データファイルの読み込みに失敗しました。(%s)\n", e.what());
	}
	using namespace boost;
	try{
		//タイトルバー変更
		string wt("WirePlanet");
		if (SysVar::DEBUG_LEVEL > 0){
			wt += " ";
			wt += SysVar::VERSION_STRING;
		}
		SetWindowText(wt.c_str());
		SetWindowIconID(ICON_ID_1);

	//	DxSetAllocPrintFlag(true);

		unordered_map<string, string> init_datas = InitDatas::GetInitDatas("Init");
		int wmode = lexical_cast<int>(init_datas.at("WindowMode")); //ウインドウモード
		int sw = lexical_cast<int>(init_datas.at("ScreenWidth")); //画面幅
		int sh = lexical_cast<int>(init_datas.at("ScreenHeight")); //画面高さ
		int cbn = lexical_cast<int>(init_datas.at("ColorBitNum")); //カラービット数
		int dw = lexical_cast<int>(init_datas.at("DrawWidth")); //描画幅
		int dh = lexical_cast<int>(init_datas.at("DrawHeight")); //描画高さ
		bool bgp_flag = boost::lexical_cast<int>(InitDatas::GetInitDatas("System").at("BackGroundProgress")) ? true : false; //バックグラウンド処理フラグ
		int dmode; //描画モード
		string dmode_s = init_datas.at("DrawMode");
		if (dmode_s == "NEAREST"){
			dmode = 0;
		}
		else if (dmode_s == "BILINEAR"){
			dmode = 1;
		}
		else{
			dmode = lexical_cast<int>(dmode_s);
		}
		//初期化
//		SetWaitVSyncFlag(false);
		if (ChangeWindowMode(wmode) == 0){
			if (wmode == 0){ printf("フルスクリーンモードで起動します。\n"); }
			else if (wmode == 1){ printf("ウインドウモードで起動します。\n"); }
		}
		else{
			printf("ウインドウモードの設定に失敗しました。:ChangeWindowMode(%d)\n",wmode);
			return -1;
		}
		if (SetGraphMode(dw, dh, cbn) == 0){
			printf("描画サイズ:%d,%d カラービット数:%d に設定しました。\n",sw,sh,cbn);
		}
		else{
			printf("画面モードの設定に失敗しました。:SetGraphMode(%d,%d,%d)\n",sw,sh,cbn);
			return -1;
		}
		//画面サイズが描画サイズと異なったら拡大縮小する
		if (sw != dw || sh != dh){
			if (SetWindowSizeExtendRate((double)sw/dw,(double)sh/dh)==0){
				printf("画面の拡大率を(%lf,%lf)に設定しました。\n", (double)sw / dw, (double)sh / dh);
			}
			else{
				printf("画面の拡大率の変更に失敗しました。\n");
			}
		}
		else{
			printf("画面の拡大縮小は行いませんでした。\n");
		}
		if (DxLib_Init() == 0){
			printf("Dxライブラリを初期化しました。\n");
		}
		else{
			printf("Dxライブラリの初期化に失敗しました。:DxLib_Init()\n");
			return -1;
		}
		if (SetDrawScreen(DX_SCREEN_BACK) == 0){
		}
		else{
			printf("描画対象画面の設定に失敗しました。:SetDrawScreen(DX_SCREEN_BACK)\n");
			return -1;
		}
		if (SetDrawMode(dmode) == 0){
			printf("描画モードを%sに設定しました。\n", dmode == 0 ? "NEAREST" : (dmode == 1 ? "BILINEAR" : "INVALID_VALUE"));
		}
		else{
			printf("描画モードの設定に失敗しました。:SetDrawMode(%s)\n",dmode==0 ? "NEAREST" : (dmode==1 ? "BILINEAR" : "INVALID_VALUE"));
			return -1;
		}
		//キー設定
		Key::GetInstance().SetDefaultCustomMap();
		SetJoypadInputToKeyInput(DX_INPUT_PAD1, PAD_INPUT_12, KEY_INPUT_SPACE);
		//ライティング計算はしない
		if (SetUseLighting(false)==0){
		}
		else{
			printf("ライティング計算の設定に失敗しました。:SetDrawMode(%s)\n", "ライティング計算:false");
			return -1;
		}
		//バックグラウンド処理の設定
		if (SetAlwaysRunFlag(bgp_flag)){
			printf("バックグラウンド進行の設定に失敗しました。\n");
			return -1;
		}
		if (bgp_flag){
			printf("バックグラウンド進行を有効にしました。\n");
		}
		else{
			printf("バックグラウンド進行を無効にしました。\n");
		}
		return 0;
	}
	catch (InitDatas::init_datas_error& e){
		printf("初期化データのセクションが不足しています。(%s)\n", e.what());
		return -1;
	}
	catch (out_of_range& e){
		printf("初期データが不足しています。(%s)\n", e.what());
		return -1;
	}
	catch (bad_lexical_cast& e){
		printf("\"%s\"に無効なデータが含まれています。(%s)\n", INI_FILE,e.what());
		return -1;
	}
}

int InitFuncs::InitFactory(){
	if (CharacterFactory::GetInstance().Init())return -1;
	if (SceneFactory::GetInstance().Init())return -2;
	if (EnemyMoverManager::GetInstance().Init())return -3;
	if (EnemyGenerator::GetInstance().Init())return -4;
	if (EffectFactory::GetInstance().Init())return -5;
	if (EffectManager::GetInstance().Init())return -6;
	if (MessageWindowManager::GetInstance().Init())return -7;
	if (EventManager::GetInstance().Init())return -8;
	if (ActionDataAdmin::GetInstance().Init())return -9;
	if (LaserManager::GetInstance().Init())return -10;
	if (CharacterManager::GetInstance().Init())return -11;
	if (ScreenManager::GetInstance().Init())return -12;
	if (GameInitializer::GetInstance().Init())return -13;
	return 0;
}

int InitFuncs::ClearObjectInManager(){
	CharacterManager::GetInstance().Clear();
	EffectManager::GetInstance().Clear();
	EnemyMoverManager::GetInstance().Clear();
	EventManager::GetInstance().Clear();
	return 0;
}

int InitFuncs::CleanManager(){
	EnemyMoverManager::GetInstance().Clean();
	EventManager::GetInstance().Clean();
	cdec::FileLoadManager::GetInstance().UnloadAllFile();
	return 0;
}

int InitFuncs::ReloadScriptFile(){
	int ret = 0;
	ret -= EnemyMoverManager::GetInstance().Reload();
	ret -= EventManager::GetInstance().Reload();
	ret -= GameInitializer::GetInstance().Reload();
	if (ret){
		printf("ファイルの再読み込みに失敗しました。(%d)\n",ret);
	}
	return ret;
}

int InitFuncs::PrepareManager(){
	//スクリーンマネージャ準備
	ScreenManager& scrm = ScreenManager::GetInstance();
	scrm.ChangeScreenBufferNum(1);
	return 0;
}

