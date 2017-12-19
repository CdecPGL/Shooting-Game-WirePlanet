#include"DxLib.h"

namespace InitFuncs{
	int InitGame(); //ゲーム初期化
	int EndGame(int=0); //ゲーム終了処理(0:通常終了 1:異常終了 2:不正検知で終了)
	int InitDxLibrary(); //DXライブラリの初期化。初期データ(InitDatas)の読み込みも行う。
	int InitFileSystem(int,int); //ファイルシステム初期化(初期化モード(0:開発データ 1:アーカイブデータ),音楽読み込みモード(0:まとめて 1:逐次))
	int InitFactory(); //ファクトリ初期化
	int PrepareManager(); //マネージャ準備
	int ClearObjectInManager(); //マネージャ内のオブジェクトを消去する
	int CleanManager(); //マネージャ内の不要なデータを削除する
	int ReloadScriptFile(); //ファイル再読込
	int LoadAnimationData(); //アニメーションファイルの読み込み
	int LoadSaveData(); //セーブデータ読み込み(0で成功 1:で初読み込み 0未満でエラー)
	int SaveSaveData(); //セーブデータ書き込み(0で成功 0未満でエラー)
	int DeleteFileCash();

}