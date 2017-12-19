#include "EventManager.h"
#include"LuaStateCreator.h"
#include"LuaStateHolder.h"
#include"InitDatas.h"
#include"LuaStateHolder.h"
#include"Game.h"

#include"FileLoadUtility.h"

#undef CreateEvent

using namespace std;

const string COROUTINE_MANAGER_FILE("coroutine_manager.lua");
const string EVENT_DEF_FILE("event_def.lua");
const string TRIGGER_DEF_FILE("trigger_def.lua");
;
EventManager::EventManager() : id_counter_trg(0), game(nullptr)
, _event_state(make_unique<LuaStateHolder>()), _trigger_state(make_unique<LuaStateHolder>())
{
	printf("EventManagerのインスタンスが作成されました。\n");
}


EventManager::~EventManager()
{
	printf("EventManagerのインスタンスが破棄されました。\n");
}

int EventManager::Init(){
	int error=0;
	error = InitLuaState();
	if (error){
		printf("EventManagerの初期化に失敗しました。LuaStateの初期化に失敗しました。(%d)\n",error);
		return error;
	}
	printf("EventManagerが初期化されました。\n");
	return 0;
}

int EventManager::Reset(){
	Clear();
	if (_event_state->GetLuaState()){ luabind::call_function<void>(_event_state->GetLuaState(), "Initialize"); }
	return 0;
}

EventManager& EventManager::GetInstance(){
	static EventManager em;
	return em;
}

int EventManager::InitLuaState(){
	string trigger_pass, event_pass;
	try{
		trigger_pass = TRIGGER_DEF_FILE;
		event_pass = EVENT_DEF_FILE;
	}
	catch (InitDatas::init_datas_error& e){
		printf("初期データのセクションが不足しています。(%s)\n", e.what());
		return -2;
	}
	catch (out_of_range& e){
		printf("初期データが不足しています。(%s)\n", e.what());
	}
	//Trigger
	_trigger_state->SetLuaState(LuaStateCreator::CreateTriggerState());
	_trigger_state->SetLuaFilePass(trigger_pass);
	if (_trigger_state->Init()){ return -1; }
	//Event	
	_event_state->SetLuaState(LuaStateCreator::CreateEventState());
	_event_state->SetLuaFilePass(COROUTINE_MANAGER_FILE);
	_event_state->SetLuaFilePass(event_pass);
	if (_event_state->Init()){ return -2; }
	return 0;
}

int EventManager::Process(){
	//トリガー処理
	process_trigger();
	//トリガーの登録、リサイクル
	resist_trigger();
	recycle_trigger();
	return 0;
}

int EventManager::Reload(){
	int res;
	int ret = 0;
	//Trigger
	res = _trigger_state->Reload(LuaStateCreator::CreateTriggerState());
	if (res){
		printf("EventManager::Reload Trigger定義Luaスクリプトの再読み込みに失敗しました。(%d)\n", res);
		ret = -1;
	}
	else{
		printf("EventManager::Reload Trigger定義Luaスクリプトが再読み込みされました。\n");
	}
	//Event	
	res = _event_state->Reload(LuaStateCreator::CreateEventState());
	if (res){
		printf("EventManager::Reload Event定義Luaスクリプトの再読み込みに失敗しました。(%d)\n", res);
		ret -= -2;
	}
	else{
		printf("EventManager::Reload Event定義Luaスクリプトが再読み込みされました。\n");
	}
	return ret;
}

void EventManager::Clean(){
	unused_triggers.clear();
	lua_gc(_event_state->GetLuaState(), LUA_GCCOLLECT,0);
	lua_gc(_trigger_state->GetLuaState(), LUA_GCCOLLECT, 0);
}

void EventManager::Clear(){
	res_list_trg.clear();
	rcl_list_trg.clear();
	triggers.clear();
	recycle_event();
	Clean();
	id_counter_trg = 0;
}

void EventManager::AllRecycle(){
	res_list_trg.clear();
	rcl_list_trg.clear();
	for (auto it = triggers.begin();it!=triggers.end();++it){
		if (it == triggers.end()){ continue; }
		if ((*it).second->End()){
			triggers.erase(it);
			continue;
		}
		unused_triggers.push_back((*it).second);
	}
	triggers.clear();
}

void EventManager::Update(){
	//イベントのアップデート
	try{
//		printf("EveMgr::Upt pre_lock\n");
		_event_state->MutexLockLuaState();
//		printf("EveMgr::Upt locked\n");
		luabind::call_function<void>(_event_state->GetLuaState(), "Process");
//		printf("EveMgr::Upt ProcessEnded\n");
		_event_state->MutexUnlockLuaState();
//		printf("EveMgr::Upt unlocked\n");
	}
	catch (luabind::error& e){
		printf("EventManager::Update Lua関数\"Process\"の呼び出しでエラーが発生しました。(%s)\n",e.what());
	}
}

int EventManager::GetEventCount()const{ return luabind::call_function<int>(_event_state->GetLuaState(),"GetCount"); }
int EventManager::GetTriggerCount()const{ return triggers.size(); }

int EventManager::GetUsingMemory()const{
	return _event_state->GetUsingMemory() + _trigger_state->GetUsingMemory();
}

int EventManager::CreateEvent(const string& evt_name, int param){
	int id = luabind::call_function<int>(_event_state->GetLuaState(), "CreateEvent", evt_name, game,param);
	int s = GetEventCount();
	if (id < 0){
		printf("EventManager::CreateEvent 存在しないイベントが指定されました。(%d,%s)\n", id,evt_name.c_str());
		return id;
	}
	return id;
}

shared_ptr<Trigger> EventManager::CreateTrigger(const string& trg_name){
	//トリガー存在確認
	if (luabind::call_function<bool>(_trigger_state->GetLuaState(),"CheckTriggerNameExistence", trg_name.c_str()) == false){
		printf("EventManager::CreateTrigger 存在しないトリガーが指定されました。(%s)\n", trg_name.c_str());
		return nullptr;
	}
	std::shared_ptr<Trigger> trg;
	if (unused_triggers.size() > 0){
		trg = *(unused_triggers.end() - 1);
		unused_triggers.pop_back();
	}
	else{
		trg = make_shared<Trigger>(id_counter_trg);
		++id_counter_trg;
	}
	trg->SetTriggerName(trg_name);
	trg->SetGameScenePointer(game);
	if (trg->Init()){
		printf("In EventManager::CreateTrigger Triggerの作成に失敗しました。(name=%s)\n", trg_name.c_str());
		return nullptr;
	}
	res_list_trg.push_back(trg);
	return trg;
}

int EventManager::RecycleEvent(int evt_id){
	return luabind::call_function<int>(_event_state->GetLuaState(), "RecycleEvent",evt_id);
}

int EventManager::RecycleTrigger(int trg_id){
	if (triggers.find(trg_id) == triggers.end()){
		return -1;
	}
	else{
		rcl_list_trg.push_back(trg_id);
		return 0;
	}
}

void EventManager::process_trigger(){
	//トリガーチェック
	for (auto it = triggers.begin(); it != triggers.end(); ++it){
		int res = luabind::call_function<int>(_trigger_state->GetLuaState(),"ExcuteTrigger", (*it).second->GetTriggerName(), (*it).second->GetGameScenePointer());
		if (res < 0){ //エラー
			printf("トリガーの実行でエラーが発生しました。(%d,%s)\n", res,(*it).second->GetTriggerName().c_str());
			RecycleTrigger((*it).second->GetID());
		}
		else if (res == 1){ //条件一致
			(*it).second->StartEvent();
			RecycleTrigger((*it).second->GetID());
		}
	}
}

void EventManager::resist_trigger(){
	for (auto it = res_list_trg.begin(); it != res_list_trg.end(); ++it){
		triggers.insert(unordered_map<int, shared_ptr<Trigger>>::value_type((*it)->GetID(), (*it)));
	}
	res_list_trg.clear();
}

void EventManager::recycle_trigger(){
	for (int trg_id : rcl_list_trg){
		auto it = triggers.find(trg_id);
		if (it == triggers.end()){ continue; }
		if ((*it).second->End()){
			triggers.erase(it);
			continue;
		}
		unused_triggers.push_back((*it).second);
		triggers.erase(it);
	}
	rcl_list_trg.clear();
}

void EventManager::recycle_event(){
	luabind::call_function<int>(_event_state->GetLuaState(), "InitEveMgr");
}

void EventManager::SetGameScenePointer(Game* g){
	AllRecycle();
	game = g;
}

