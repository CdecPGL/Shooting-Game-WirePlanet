#include "EnemyMoverManager.h"
#include"LuaStateCreator.h"
#include"LuaStateHolder.h"
#include"luabind/luabind.hpp"
#include"Enemy.h"

const std::string COROUTINE_MANAGER_FILE("coroutine_manager.lua");
const std::string EnemyMoverFileName("EnemyMover.lua");

using namespace std;

EnemyMoverManager::EnemyMoverManager() :_lua_state_holder(make_unique<LuaStateHolder>()), initialized(false)
{
	printf("EnemyMoverManagerのインスタンスが作成されました。\n");
}


EnemyMoverManager::~EnemyMoverManager()
{
	printf("EnemyMoverManagerのインスタンスが破棄されました。\n");
}

EnemyMoverManager& EnemyMoverManager::GetInstance(){
	static EnemyMoverManager emm;
	return emm;
}

int EnemyMoverManager::Init(){
	boost::mutex::scoped_lock m_sl(_lua_mtx);
	_lua_state_holder->SetLuaState(LuaStateCreator::CreateEnemyMoverState());
	_lua_state_holder->SetLuaFilePass(COROUTINE_MANAGER_FILE);
	_lua_state_holder->SetLuaFilePass(EnemyMoverFileName);
	int res = _lua_state_holder->Init();
	if (res){
		printf("EnemyMoverManagerの初期化に失敗しました。:LuaStateの作成に失敗しました。\n");
		return -1;
	}
	else{
		initialized = true;
		printf("EnemyMoverManagerのが初期化されました。\n");
		return 0;
	}
}

int EnemyMoverManager::Reset(){
	Clear();
	return 0;
}

int EnemyMoverManager::Reload(){
	int res = _lua_state_holder->Reload(LuaStateCreator::CreateEnemyMoverState());
	if (res){
		printf("EnemyMoverManager::Reload Luaスクリプトの再読み込みに失敗しました。(%d)\n",res);
		return -1;
	}
	else{
		printf("EnemyMoverManager::Reload Luaスクリプトが再読み込みされました。\n");
		return 0;
	}
}

int EnemyMoverManager::CreateMover(const string& mn,Enemy* e){
//	printf("emm::cm pre_lock\n");
	_lua_state_holder->MutexLockLuaState();
//	printf("emm::cm locked\n");
	int ret = luabind::call_function<int>(_lua_state_holder->GetLuaState(), "CreateMover", mn, e);
//	printf("emm::cm created\n");
	_lua_state_holder->MutexUnlockLuaState();
//	printf("emm::cm unlocked\n");
	return ret;
}

int EnemyMoverManager::CreateMoverTS(const string& mn, Enemy* e){
	boost::mutex::scoped_lock m_sl(_lua_mtx);
	return luabind::call_function<int>(_lua_state_holder->GetLuaState(), "CreateMover", mn, e);
}

int EnemyMoverManager::DeleteMover(int id){
	return luabind::call_function<int>(_lua_state_holder->GetLuaState(), "DeleteMover", id);
}

int EnemyMoverManager::ExcuteMover(int id){
	boost::mutex::scoped_lock m_sl(_lua_mtx);
	return luabind::call_function<int>(_lua_state_holder->GetLuaState(), "ExcuteMover", id);
}

int EnemyMoverManager::GetUsingMemory()const{
	boost::mutex::scoped_lock m_sl(_lua_mtx);
	return _lua_state_holder->GetUsingMemory();
}

int EnemyMoverManager::GetCount()const{
	boost::mutex::scoped_lock m_sl(_lua_mtx);
	return luabind::call_function<int>(_lua_state_holder->GetLuaState(), "GetMoverCount");
}

void EnemyMoverManager::Clean(){
	lua_gc(_lua_state_holder->GetLuaState(), LUA_GCCOLLECT, 0);
}

void EnemyMoverManager::Clear(){
	luabind::call_function<void>(_lua_state_holder->GetLuaState(), "InitEneMvrMgr");
	Clean();
}
