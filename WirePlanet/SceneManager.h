#pragma once

//シーンマネージャー
//2014/04/28 Version1.0 RPG3mより移植
//2014/05/08 Version2.0 シーンの変更処理変更。変更時に現在のシーンと次のシーンをどうするのかの指定方法を変更。
//2014/06/14 Version2.1 シーンの保持、登録、登録解除をObjectHolderTemplate_WithIDに置き換え
//2014/06/25 Version2.2 シーンのProcess()をUpdate()とDraw()に分離

#include<map>
#include<memory>
#include<string>
#include"DataContainer.h"
#include<unordered_map>
#include<tuple>
#include"Scene.h"

template<typename T,class C>
class ObjectHolderTemplate_WithID;
class SceneChangeEffect;

//シーン変更時動作指定列挙体
//現在のシーンをどうするか(END:終了処理を行うが破棄はしない INTERRUPT:終了処理を行わない DISPOSE:終了処理を行い破棄する)
enum CSCrntSceneProc{END,INTERRUPT,DISPOSE};
//次のシーンをどうするか(INIT:初期化する RESTART:初期化しない(未初期化だったら初期化する))
enum CSNextSceneProc{INIT,RESTART};

//シングルトン

class SceneManager
{
public:
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	static SceneManager& GetInstance();
	int Init(); //初期化
	int End(); //終了処理
	int Process(); //管理処理(-1でエラー 1でゲーム終了)
	void Clean(); //未参照のシーンを削除
	void Clear(); //全削除

	void Update(); //シーン更新
	void Draw(); //シーン描画

	int EndGameRequest(); //ゲーム終了リクエスト

	std::shared_ptr<Scene> CreateObject(const std::string&,const std::string& ="NULL"); //シーン作成(初期化id,任意のid)
	int DeleteObject(const std::string&); //シーン削除(id)
	bool CheckObjectResisted(const std::string&)const; //指定IDのオブジェクトが存在してるか
	std::shared_ptr<Scene> FindObject(const std::string&)const; //指定IDのオブジェクトを取得
	int GetObjectCount()const; //登録されているオブジェクト数を取得
	std::string GetNewID()const; //重複しないIDを取得

	int ChangeSceneRequest(const string&,CSCrntSceneProc = DISPOSE,CSNextSceneProc = INIT); //IDでシーンの変更依頼(id,現在のシーンをどうするか、変更シーンをどうするか)(-1の時は失敗で変更されない。)
	std::shared_ptr<Scene> CreateAndChangeScene(const std::string&, const std::string& = "NULL", CSCrntSceneProc = DISPOSE, CSNextSceneProc = INIT); //シーンを作成して変更(初期化ID、任意のID,現在のシーンをどうするか、変更シーンをどうするか)
	shared_ptr<Scene> GetCurrentScene()const; //現在のシーンのポインタを取得
	std::string GetCurrentSceneID()const; //現在のシーンのIDを取得
private:
	bool initialized; //初期化されたか

	bool _game_end_reuest;

	std::unique_ptr<ObjectHolderTemplate_WithID<std::string, Scene>> scene_holder; //シーンホルダー

	SceneManager();
	std::shared_ptr<Scene> current_scene;
	struct Request{
		Request(const std::string&,CSCrntSceneProc,CSNextSceneProc);
		std::string ID;
		CSCrntSceneProc csp; //現在のシーンをどうするか
		CSNextSceneProc nsp; //次のシーンをどうするか
	};
	Request request; //シーン変更リクエスト(ID,現在のシーンを中断するか、変更シーンを再開するか)("None"でなし)

	void ChangeScene(); //シーン変更処理

	shared_ptr<DataContainer> datacontainer; //シーングローバルデータコンテナ

//	std::unique_ptr<SceneChangeEffect> _scn_cng_efct;

	//グローバル変数
//	shared_ptr<Counter> counter;
};

