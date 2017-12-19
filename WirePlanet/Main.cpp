//�t�@�C���̃C���N���[�h

#include"define.h"
#include "DxLib.h"
#include "Key.h"
#include"SceneManager.h"
#include<exception>
#include"InitFuncs.h"
#include"GraphicController2D.h"
#include"ScreenManager.h"
#include"secure_exception.h"
#include"VSYNCManager.h"
#include"SystemVariable.h"

#ifdef DEBUG_DRAW
#include"DebugWindow.h"
#endif

//���색�C�u�����̎g�p
//�v���W�F�N�g�̐ݒ�̒ǉ��̈ˑ����C�u�����Ɏg�����C�u������ǉ����C���N���[�h����B
//lua���g���Ƃ��͉�����lua.dll���v���W�F�N�g�̃f�B���N�g���ɃR�s�[����

void MyTerminate(){
	abort();
}

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	std::set_terminate(MyTerminate);
#ifdef _DEBUG
	::AllocConsole();               // �R�}���h�v�����v�g���\�������
	freopen("CON", "r", stdin);  // �W�����͂����蓖�Ă�
	freopen("CON", "w", stdout);  // �W���o�͂����蓖�Ă�
	printf("�f�o�b�O���[�h\n");
#else
	FILE* fp_out = freopen("game_log.txt", "w", stdout);
#endif
	//�Q�[��������
	int err;
	if (err = InitFuncs::InitGame()){
		printf("�������Ɏ��s���܂����B(%d)\n",err);
		MessageBox(GetMainWindowHandle(), "�������Ɏ��s���܂����B�A�v���P�[�V�������I�����܂��B", "�������G���[", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
		InitFuncs::EndGame();
		DxLib_End();
		return -1;
	}
#ifdef DEBUG_DRAW
	DebugWindow& dw = DebugWindow::GetInstance();
#endif
	Counter& cntr = Counter::GetInstance();

	//���������Ǘ�
	VSYNCManager& vm = VSYNCManager::GetInstance();
	//�J�����̏�����
	gc2d::Init();
	//�O���[�o���ϐ��̐錾
	SceneManager& sm = SceneManager::GetInstance();
	sm.Init();
	sm.CreateAndChangeScene("Title");
	ScreenManager& scrm = ScreenManager::GetInstance();

	// while( ����ʂ�\��ʂɔ��f, ���b�Z�[�W����, ��ʃN���A )
	while( vm.ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 && Key::GetInstance().Update()==0 ){
		//�����Ɏ�ȏ�����`��
		try{
			cntr.StartMeasurePR();

			sm.Update();
//			gc2d::UpdateCamera();
			sm.Draw();
			if (sm.Process()){ break; }
			cntr.Update();
#ifdef DEBUG_DRAW
			if (SysVar::DEBUG_LEVEL >= 1){
				dw.Update();
				dw.Draw();
			}
#endif
			scrm.DrawScreen();
		}
		catch (secure_obj::cheat_detected& e){
			printf("�s�������o����܂����B: %s\n", e.what());
			MessageBox(GetMainWindowHandle(), "�`�[�g�̓_���_��", "�G���[", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION | MB_APPLMODAL);
			InitFuncs::EndGame(2);
			return -3;
		}
		catch (std::exception& e){
			printf("�\�����ʗ�O���������܂����B: %s\n", e.what());
			MessageBox(GetMainWindowHandle(), "�G���[���������܂����B�A�v���P�[�V�������I�����܂��B", "�G���[", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
			InitFuncs::EndGame(1);
#ifdef _DEBUG
			throw;
#endif
			return -1;
		}
		catch (...){
			printf("�����s���̗�O���������܂���\n");
			MessageBox(GetMainWindowHandle(), "�G���[���������܂����B�A�v���P�[�V�������I�����܂��B", "�G���[", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
			InitFuncs::EndGame(1);
#ifdef _DEBUG
			throw;
#endif
			return -2;
		}
	}
	printf("�Q�[�����[�v��ʏ�I�����܂��B\n");
	InitFuncs::EndGame();
	return 0;
}  