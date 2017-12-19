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
	// �N�������擾
	time_t now = time(NULL);
	tm *pnow = new tm;
	localtime_s(pnow, &now);
	int y(pnow->tm_year + 1900), m(pnow->tm_mon + 1), d(pnow->tm_mday), h(pnow->tm_hour), mi(pnow->tm_min), s(pnow->tm_sec);
	delete pnow;
	//�N�������o��
	printf("�N������ > %d/%d/%d %d:%d:%d\n", y, m, d, h, mi, s);
	//�o�[�W����
	printf("�o�[�W���� > %s\n", SysVar::VERSION_STRING);
	//�Z�[�u�f�[�^�ǂݍ���
	int ret = LoadSaveData();
	if (ret < 0){
		printf("�Z�[�u�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B(%d)\n",ret);
		return -5;
	}
	else if (ret == 1){
		printf("�Z�[�u�f�[�^�����݂��Ȃ����ߏ���N���Ƃ��܂��B\n");
	}
	else{
		printf("�Z�[�u�f�[�^��ǂݍ��݂܂����B\n");
	}
	//�N���񐔉��Z
	if (ret == 1){ //����N����
		printf("����N���ł��B\n");
		GPIUtil::SetFirstPlayDate((unsigned)y, (unsigned)m, (unsigned)d, (unsigned)h, (unsigned)mi, (unsigned)s);
	}
	GPIUtil::AddPlayCount();
	//���������̐ݒ�
	ret = VSYNCManager::GetInstance().Init();
	if (ret < 0){
		printf("��ʂ̐��������ݒ�Ɏ��s���܂����B\n");
	}
	else if (ret == 1){
		printf("��ʂ̐��������𖳎����܂��B\n");
	}
	else{
		printf("��ʂ̐��������ɕ`��^�C�~���O�����킹�܂��B\n");
	}
	//DX���C�u����������
	if (InitDxLibrary()){
		printf("Dx���C�u�����̏������Ɏ��s���܂����B\n");
		return -1;
	}
	//�t�@�C���V�X�e���ݒ�
	try{
		int file_mode;
		try{
			file_mode = boost::lexical_cast<int>(InitDatas::GetInitDatas("System").at("FileMode"));
		}
		catch (out_of_range& e){
			//System->FIleMode���Ȃ�������1(�A�[�J�C�u����ɐݒ�)
			printf("ini�t�@�C����System->FileMode���ݒ肳��Ă��Ȃ��̂�1�Ƃ��܂��B(%s)\n", e.what());
			file_mode = 1;
		}
		std::string music_load_mode = InitDatas::GetInitDatas("System").at("MusicLoadMode");
		if (InitFileSystem(file_mode, boost::lexical_cast<int>(music_load_mode))){
			printf("�t�@�C���V�X�e���̏������Ɏ��s���܂����B\n");
			return -2;
		}
	}
	catch (InitDatas::init_datas_error& e){
		printf("�������f�[�^�̃Z�N�V�������s�����Ă��܂��B(%s)\n", e.what());
		return -2;
	}
	catch (out_of_range& e){
		printf("�����f�[�^���s�����Ă��܂��B(%s)\n", e.what());
		return -2;
	}
	catch (boost::bad_lexical_cast& e){
		printf("\"%s\"�ɖ����ȃf�[�^���܂܂�Ă��܂��B(%s)\n", INI_FILE, e.what());
		return -2;
	}
	if (InitFactory()){
		printf("�t�@�N�g���̏������Ɏ��s���܂����B\n");
		return -3;
	}
	if (PrepareManager()){
		printf("�}�l�[�W���̏����Ɏ��s���܂����B\n");
		return -4;
	}
	//�t�H���g�̓ǂݍ���
	auto file = cdec::FileLoadManager::GetInstance().FindFile(SysVar::GAME_FONT_FILE_NAME);
	if (file == nullptr || file->GetStatus() != cdec::FileStatus::Available) {
		printf("�t�H���g�t�@�C���̓ǂݍ��݂Ɏ��s���܂������A���s���܂��B\n");
	} else {
		DWORD fn;
		if (AddFontMemResourceEx(reinterpret_cast<LPVOID>(const_cast<void*>(file->GetTopPointer())), file->GetSize(), nullptr, &fn) == 0) {
			printf("�t�H���g�̓ǂݍ��݂Ɏ��s���܂������A���s���܂��B\n");
		}
	}
	//�t�@�C���V�X�e���̃L���b�V���폜
	DeleteFileCash();
	return 0;
}

int InitFuncs::EndGame(int mode){
	switch (mode){
	case 0: //�ʏ�I��
		break;
	case 1: //�ُ�I��
		GPIUtil::AddErrorCount();
		break;
	case 2: //�s���I��
		GPIUtil::AddCheatCount();
		break;
	}
	//�Q�[���v���C���ԉ��Z
	GPIUtil::AddPlayTime(Counter::GetInstance().GetFrameFromStart(), Counter::GetInstance().GetTimeFromStartBySecond());

	SoundManager::GetInstance().End();
	ScreenManager::GetInstance().End();
	InitGraph(); //�摜�f�[�^������
	DxLib_End(); // DX���C�u�����I������
	int ret = SaveSaveData();
	if (ret < 0){
		printf("�Z�[�u�f�[�^��ۑ��ł��܂���ł����B(%d)\n", ret);
	}
	else{
		printf("�Z�[�u�f�[�^��ۑ����܂����B\n");
	}
#ifdef _DEBUG
	FreeConsole();
#endif
	return ret;
}

int InitFuncs::InitDxLibrary(){
	try{
		InitDatas::LoadInitFile();
		printf("�����f�[�^��ǂݍ��݂܂����B\n");
	}
	catch (InitDatas::init_datas_error& e){
		printf("�����f�[�^�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(%s)\n", e.what());
	}
	using namespace boost;
	try{
		//�^�C�g���o�[�ύX
		string wt("WirePlanet");
		if (SysVar::DEBUG_LEVEL > 0){
			wt += " ";
			wt += SysVar::VERSION_STRING;
		}
		SetWindowText(wt.c_str());
		SetWindowIconID(ICON_ID_1);

	//	DxSetAllocPrintFlag(true);

		unordered_map<string, string> init_datas = InitDatas::GetInitDatas("Init");
		int wmode = lexical_cast<int>(init_datas.at("WindowMode")); //�E�C���h�E���[�h
		int sw = lexical_cast<int>(init_datas.at("ScreenWidth")); //��ʕ�
		int sh = lexical_cast<int>(init_datas.at("ScreenHeight")); //��ʍ���
		int cbn = lexical_cast<int>(init_datas.at("ColorBitNum")); //�J���[�r�b�g��
		int dw = lexical_cast<int>(init_datas.at("DrawWidth")); //�`�敝
		int dh = lexical_cast<int>(init_datas.at("DrawHeight")); //�`�捂��
		bool bgp_flag = boost::lexical_cast<int>(InitDatas::GetInitDatas("System").at("BackGroundProgress")) ? true : false; //�o�b�N�O���E���h�����t���O
		int dmode; //�`�惂�[�h
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
		//������
//		SetWaitVSyncFlag(false);
		if (ChangeWindowMode(wmode) == 0){
			if (wmode == 0){ printf("�t���X�N���[�����[�h�ŋN�����܂��B\n"); }
			else if (wmode == 1){ printf("�E�C���h�E���[�h�ŋN�����܂��B\n"); }
		}
		else{
			printf("�E�C���h�E���[�h�̐ݒ�Ɏ��s���܂����B:ChangeWindowMode(%d)\n",wmode);
			return -1;
		}
		if (SetGraphMode(dw, dh, cbn) == 0){
			printf("�`��T�C�Y:%d,%d �J���[�r�b�g��:%d �ɐݒ肵�܂����B\n",sw,sh,cbn);
		}
		else{
			printf("��ʃ��[�h�̐ݒ�Ɏ��s���܂����B:SetGraphMode(%d,%d,%d)\n",sw,sh,cbn);
			return -1;
		}
		//��ʃT�C�Y���`��T�C�Y�ƈقȂ�����g��k������
		if (sw != dw || sh != dh){
			if (SetWindowSizeExtendRate((double)sw/dw,(double)sh/dh)==0){
				printf("��ʂ̊g�嗦��(%lf,%lf)�ɐݒ肵�܂����B\n", (double)sw / dw, (double)sh / dh);
			}
			else{
				printf("��ʂ̊g�嗦�̕ύX�Ɏ��s���܂����B\n");
			}
		}
		else{
			printf("��ʂ̊g��k���͍s���܂���ł����B\n");
		}
		if (DxLib_Init() == 0){
			printf("Dx���C�u���������������܂����B\n");
		}
		else{
			printf("Dx���C�u�����̏������Ɏ��s���܂����B:DxLib_Init()\n");
			return -1;
		}
		if (SetDrawScreen(DX_SCREEN_BACK) == 0){
		}
		else{
			printf("�`��Ώۉ�ʂ̐ݒ�Ɏ��s���܂����B:SetDrawScreen(DX_SCREEN_BACK)\n");
			return -1;
		}
		if (SetDrawMode(dmode) == 0){
			printf("�`�惂�[�h��%s�ɐݒ肵�܂����B\n", dmode == 0 ? "NEAREST" : (dmode == 1 ? "BILINEAR" : "INVALID_VALUE"));
		}
		else{
			printf("�`�惂�[�h�̐ݒ�Ɏ��s���܂����B:SetDrawMode(%s)\n",dmode==0 ? "NEAREST" : (dmode==1 ? "BILINEAR" : "INVALID_VALUE"));
			return -1;
		}
		//�L�[�ݒ�
		Key::GetInstance().SetDefaultCustomMap();
		SetJoypadInputToKeyInput(DX_INPUT_PAD1, PAD_INPUT_12, KEY_INPUT_SPACE);
		//���C�e�B���O�v�Z�͂��Ȃ�
		if (SetUseLighting(false)==0){
		}
		else{
			printf("���C�e�B���O�v�Z�̐ݒ�Ɏ��s���܂����B:SetDrawMode(%s)\n", "���C�e�B���O�v�Z:false");
			return -1;
		}
		//�o�b�N�O���E���h�����̐ݒ�
		if (SetAlwaysRunFlag(bgp_flag)){
			printf("�o�b�N�O���E���h�i�s�̐ݒ�Ɏ��s���܂����B\n");
			return -1;
		}
		if (bgp_flag){
			printf("�o�b�N�O���E���h�i�s��L���ɂ��܂����B\n");
		}
		else{
			printf("�o�b�N�O���E���h�i�s�𖳌��ɂ��܂����B\n");
		}
		return 0;
	}
	catch (InitDatas::init_datas_error& e){
		printf("�������f�[�^�̃Z�N�V�������s�����Ă��܂��B(%s)\n", e.what());
		return -1;
	}
	catch (out_of_range& e){
		printf("�����f�[�^���s�����Ă��܂��B(%s)\n", e.what());
		return -1;
	}
	catch (bad_lexical_cast& e){
		printf("\"%s\"�ɖ����ȃf�[�^���܂܂�Ă��܂��B(%s)\n", INI_FILE,e.what());
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
		printf("�t�@�C���̍ēǂݍ��݂Ɏ��s���܂����B(%d)\n",ret);
	}
	return ret;
}

int InitFuncs::PrepareManager(){
	//�X�N���[���}�l�[�W������
	ScreenManager& scrm = ScreenManager::GetInstance();
	scrm.ChangeScreenBufferNum(1);
	return 0;
}

