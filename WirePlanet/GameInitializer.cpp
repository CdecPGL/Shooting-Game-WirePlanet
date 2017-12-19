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
	printf("GameInitializer�̃C���X�^���X���쐬����܂����B\n");
}

GameInitializer::~GameInitializer()
{
	printf("GameInitializer�̃C���X�^���X���j������܂����B\n");
}

GameInitializer& GameInitializer::GetInstance(){
	static GameInitializer gi;
	return gi;
}

int GameInitializer::Init(){
	if (initLuaStateHolder() < 0){
		printf("GameInitializer�̏������Ɏ��s���܂����B(-1)\n");
		return -1;
	}
	printf("GameInitializer������������܂����B\n");
	return 0;
}

int GameInitializer::Reload(){
	int error = _l_state->Reload(LuaStateCreator::CreateGameInitializerState());
	if (error < 0){
		printf("GameInitializer�̍ēǂݍ��݂Ɏ��s���܂����B(%d)\n", error);
		return error;
	}
	printf("GameInitializer�̍ēǂݍ��݂ɐ������܂����B\n");
	return 0;
}

int GameInitializer::operator()(Game* g, const string& iniid, const string& eve, int evep){
	if (luabind::call_function<bool>(_l_state->GetLuaState(), "CheckInitializerExistence", iniid) == false){
		printf("GameInitializer �Q�[���V�[���̏������Ɏ��s���܂����B�w�肳�ꂽ������ID�����݂��܂���B(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
		return -1;
	}
	if (luabind::call_function<int>(_l_state->GetLuaState(), iniid.c_str(), g) < 0){
		printf("GameInitializer �Q�[���V�[���̏������Ɏ��s���܂����B�������̎��s�Ɏ��s���܂����B(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
		return -2;
	}
	if (eve != "NULL"){
		if (EventManager::GetInstance().CreateEvent(eve, evep) < 0){
			printf("GameInitializer �Q�[���J�n�C�x���g�̎��s�Ɏ��s���܂����B(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
		}
	}
	printf("GameInitializer �Q�[���V�[�������������܂����B(ID:%s,eve:%s,eve_param:%d)\n", iniid.c_str(), eve.c_str(), evep);
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