//ファイルのインクルード

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

//自作ライブラリの使用
//プロジェクトの設定の追加の依存ライブラリに使うライブラリを追加しインクルードする。
//luaを使うときは加えてlua.dllをプロジェクトのディレクトリにコピーする

void MyTerminate(){
	abort();
}

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	std::set_terminate(MyTerminate);
#ifdef _DEBUG
	::AllocConsole();               // コマンドプロンプトが表示される
	freopen("CON", "r", stdin);  // 標準入力を割り当てる
	freopen("CON", "w", stdout);  // 標準出力を割り当てる
	printf("デバッグモード\n");
#else
	FILE* fp_out = freopen("game_log.txt", "w", stdout);
#endif
	//ゲーム初期化
	int err;
	if (err = InitFuncs::InitGame()){
		printf("初期化に失敗しました。(%d)\n",err);
		MessageBox(GetMainWindowHandle(), "初期化に失敗しました。アプリケーションを終了します。", "初期化エラー", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
		InitFuncs::EndGame();
		DxLib_End();
		return -1;
	}
#ifdef DEBUG_DRAW
	DebugWindow& dw = DebugWindow::GetInstance();
#endif
	Counter& cntr = Counter::GetInstance();

	//垂直同期管理
	VSYNCManager& vm = VSYNCManager::GetInstance();
	//カメラの初期化
	gc2d::Init();
	//グローバル変数の宣言
	SceneManager& sm = SceneManager::GetInstance();
	sm.Init();
	sm.CreateAndChangeScene("Title");
	ScreenManager& scrm = ScreenManager::GetInstance();

	// while( 裏画面を表画面に反映, メッセージ処理, 画面クリア )
	while( vm.ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 && Key::GetInstance().Update()==0 ){
		//ここに主な処理を描く
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
			printf("不正が検出されました。: %s\n", e.what());
			MessageBox(GetMainWindowHandle(), "チートはダメダメ", "エラー", MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION | MB_APPLMODAL);
			InitFuncs::EndGame(2);
			return -3;
		}
		catch (std::exception& e){
			printf("予期せぬ例外が発生しました。: %s\n", e.what());
			MessageBox(GetMainWindowHandle(), "エラーが発生しました。アプリケーションを終了します。", "エラー", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
			InitFuncs::EndGame(1);
#ifdef _DEBUG
			throw;
#endif
			return -1;
		}
		catch (...){
			printf("原因不明の例外が発生しました\n");
			MessageBox(GetMainWindowHandle(), "エラーが発生しました。アプリケーションを終了します。", "エラー", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
			InitFuncs::EndGame(1);
#ifdef _DEBUG
			throw;
#endif
			return -2;
		}
	}
	printf("ゲームループを通常終了します。\n");
	InitFuncs::EndGame();
	return 0;
}  