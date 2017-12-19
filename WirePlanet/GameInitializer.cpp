#include "GameInitializer.h"

#include"LuaStateHolder.h"
#include"Game.h"
#include"LuaStateCreator.h"
#include"luabind/luabind.hpp"
#include"EventManager.h"

using namespace std;

namespace{
	const string GAME_INIT_LUA_STATE("game_init_def.lua");
}

GameInitializer::GameInitializer() :_l_state(make_unique<LuaStateHolder>())
{
	printf("GameInitializerのインスタンスが作成されました。\n");
}

GameInitializer::~GameInitializer()
{
	printf("GameInitializerのインスタンスが破棄されました。\n");
}

GameInitializer& GameInitializer::GetInstance(){
	static GameInitializer gi;
	return gi;
}

int GameInitializer::Init(){
	if (initLuaStateHolder() < 0){
		printf("GameInitializerの初期化に失敗しました。(-1)\n");
		return -1;
	}
	printf("GameInitializerが初期化されました。\n");
	return 0;
}

int GameInitializer::Reload(){
	int error = _l_state->Reload(LuaStateCreator::CreateGameInitializerState());
	if (error < 0){
		printf("GameInitializerの再読み込みに失敗しました。(%d)\n", error);
		return error;
	}
	printf("GameInitializerの再読み込みに成功しました。\n");
	return 0;
}

int GameInitializer::operator()(Game* g, const string& iniid, const string& eve, int evep){
	if (luabind::call_function<bool>(_l_state->GetLuaState(), "CheckInitializerExistence", iniid) == false){
		printf("GameInitializer ゲームシーンの初期化に失敗しました。指定された初期化IDが存在しません。(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
		return -1;
	}
	if (luabind::call_function<int>(_l_state->GetLuaState(), iniid.c_str(), g) < 0){
		printf("GameInitializer ゲームシーンの初期化に失敗しました。初期化の実行に失敗しました。(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
		return -2;
	}
	if (eve != "NULL"){
		if (EventManager::GetInstance().CreateEvent(eve, evep) < 0){
			printf("GameInitializer ゲーム開始イベントの実行に失敗しました。(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
		}
	}
	printf("GameInitializer ゲームシーンを初期化しました。(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
	return 0;
}

int GameInitializer::initLuaStateHolder(){
	_l_state->SetLuaState(LuaStateCreator::CreateGameInitializerState());
	_l_state->SetLuaFilePass(GAME_INIT_LUA_STATE);
	if (_l_state->Init() < 0){
		return -1;
	}
	return 0;
}