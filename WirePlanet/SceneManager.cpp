#include "SceneManager.h"
#include"define.h"
#include"SceneFactory.h"
#ifdef DEBUG_DRAW
#include<ctime>
#include"DebugWindow.h"
#endif
#include"boost/lexical_cast.hpp"
#include"ObjectHolderTemplate_WithID.h"

using namespace std;

SceneManager::Request::Request(const string& id, CSCrntSceneProc m_csp, CSNextSceneProc m_nsp) :ID(id), csp(m_csp),nsp(m_nsp){}

SceneManager::SceneManager() : request("None", END, INIT), datacontainer(new DataContainer), initialized(false), scene_holder(make_unique<ObjectHolderTemplate_WithID<string, Scene>>()), _game_end_reuest(false)
{
	printf("SceneManagerのインスタンスが作成されました。\n");
}


SceneManager::~SceneManager()
{
	printf("SceneManagerのインスタンスが破棄されました。\n");
}

SceneManager& SceneManager::GetInstance(){
	static SceneManager sm;
	return sm;
}

//初期化
int SceneManager::Init(){
	//dataholder->SetCounter(counter);
	initialized = true;
	_game_end_reuest = false;
	printf("SceneManagerが初期化されました。\n");
	return 0;
}

//終了処理
int SceneManager::End(){
	request.ID = "None";
	int result = 0;
	if (current_scene){
		result = current_scene->End();
		current_scene.reset();
	}
	datacontainer->clear(); //データコンテナの消去
	scene_holder->clear();
	initialized = false;
	printf("SceneManagerの終了処理が実行されました。\n");
	return result;
}

//管理処理
int SceneManager::Process(){
	scene_holder->process_resist_list();
	scene_holder->process_unresist_list();
	if (_game_end_reuest){
		for (auto it : scene_holder->objects){
			if (it.second->Initialized() == false){ it.second->End(); }
		}
		return 1;
	}
	ChangeScene();
	return 0;
}

int SceneManager::EndGameRequest(){
	_game_end_reuest = true;
	return 0;
}

//シーン処理
void SceneManager::Update(){
	if (initialized == false || current_scene == nullptr){ return; }
#ifdef DEBUG_DRAW
	time_t t = clock();
#endif
	current_scene->Update();
#ifdef DEBUG_DRAW
	t = clock()-t;
	DebugWindow::GetInstance().AddExecutionTime("ScnMgr::Update", t / (CLOCKS_PER_SEC / 1000));
#endif
}

//シーン描画
void SceneManager::Draw(){
	if (initialized == false || current_scene == nullptr){ return; }
#ifdef DEBUG_DRAW
	time_t t = clock();
#endif
	current_scene->Draw();
#ifdef DEBUG_DRAW
	t = clock() - t;
	DebugWindow::GetInstance().AddExecutionTime("ScnMgr::Draw", t / (CLOCKS_PER_SEC / 1000));
#endif
}

//シーン作成
shared_ptr<Scene> SceneManager::CreateObject(const string& init_id, const string& id){
	auto ns = SceneFactory::GetInstance().CreateObject(init_id);
	if (ns){
		ns->SetID(id == "NULL" ? GetNewID() : id);
		scene_holder->resist_object(ns, id == "NULL" ? GetNewID():id);
	}
	return ns;
}

//シーン破棄
int SceneManager::DeleteObject(const string& id){
	auto it = scene_holder->objects.find(id);
	if (it == scene_holder->objects.end()){ return -1; }
	scene_holder->unresist_object((*it).first);
	return 0;
}

//シーン変更依頼
int SceneManager::ChangeSceneRequest(const string& id, CSCrntSceneProc csp, CSNextSceneProc nsp){
	if (CheckObjectResisted(id)){
		request = Request(id, csp, nsp);
	}
	else{ //シーンリストになかったら登録リストも確認するS
		auto it = scene_holder->res_list.find(id);
		if (it == scene_holder->res_list.end()){ return -1; }
		request = Request(id, csp, nsp);
	}
	return 0;
}

//現在のシーンのポインタを取得
shared_ptr<Scene> SceneManager::GetCurrentScene()const{ return current_scene; }

//シーン変更処理
void SceneManager::ChangeScene(){
	if (request.ID == "None"){ return; }
	//現在のシーンを終了又は中断
	if (current_scene){
		int error = 0;
		if (request.csp == END || request.csp == DISPOSE ){
			error = current_scene->End();
			if (request.csp == DISPOSE){
				DeleteObject(current_scene->GetID());
			}
		} //現在のシーンの終了
		if (error != 0){
			printf("SceneManager::ChangeScene %sの終了に失敗しました(%d)\n", typeid(*(current_scene.get())).name(), error);
			request.ID = "None";
			current_scene.reset();
			return;
		}
	}
	//新しいシーンのセット
	current_scene = FindObject(request.ID);
	int error = 0;
	if (request.nsp == INIT){ error = current_scene->Init(datacontainer); }
	else if (request.nsp == RESTART){
		if (!current_scene->Initialized()){ error = current_scene->Init(datacontainer); }
	}
	if (error){
		printf("SceneManager::ChangeScene %sの初期化に失敗しました(%d)\n", typeid(*(current_scene.get())).name(), error);
		current_scene.reset();
	}
	else{
		printf("シーンを切り替えました:%s\n", typeid(*(current_scene.get())).name());
	}
	request.ID = "None";
	return;
}

bool SceneManager::CheckObjectResisted(const string& id)const{
	return scene_holder->is_exist(id);
}

shared_ptr<Scene> SceneManager::FindObject(const string& id)const{
	return scene_holder->find_object(id);
}

int SceneManager::GetObjectCount()const{
	return scene_holder->size();
}

string SceneManager::GetNewID()const{
	static int id_cnt = 0;
	string nid;
	do{
		nid = "Scene_";
		nid += boost::lexical_cast<string>(id_cnt);
		++id_cnt;
	} while (CheckObjectResisted(nid));
	return nid;
}

shared_ptr<Scene> SceneManager::CreateAndChangeScene(const string& init_id,const string& id,CSCrntSceneProc csp,CSNextSceneProc nsp){
	string nid = (id == "NULL" ? GetNewID() : id);
	auto ns = CreateObject(init_id,nid);
	if (ns){
		ChangeSceneRequest(nid,csp,nsp);
	}
	return ns;
}