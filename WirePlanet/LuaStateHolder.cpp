#include "LuaStateHolder.h"
#include"FileLoadUtility.h"

using namespace std;

recursive_mutex LuaStateHolder::_ls_mtx;

void LuaStateHolder::MutexLockLuaState(){
//	_ls_mtx.lock();
}

void LuaStateHolder::MutexUnlockLuaState(){
//	_ls_mtx.unlock();
}

LuaStateHolder::LuaStateHolder() :L(nullptr), initialized(false)
{
}


LuaStateHolder::~LuaStateHolder()
{
	if (L){
		lua_close(L);
	}
}

void LuaStateHolder::SetLuaState(lua_State* l){
	if (L){
		lua_close(L);
	}
	L = l; 
}
void LuaStateHolder::SetLuaFilePass(const string& lfp){ lua_file_pass.push_back(lfp); }

int LuaStateHolder::Init(){
	return LoadLuaState();
}

int LuaStateHolder::Reload(lua_State* nl){
	if (L){ lua_close(L); }
	L = nl;
	return LoadLuaState();
}

int LuaStateHolder::LoadLuaState(){
	if (L){
		for (auto& file_name : lua_file_pass){
			if (FileLoadUtil::LoadLuaScriptFromFLM(L, file_name.c_str(), true) || lua_pcall(L, 0, LUA_MULTRET, 0)){
				const char* err = lua_tostring(L, -1);
				printf("LuaStateHolderの初期化に失敗しました。:Luaファイル\"%s\"の読み込みに失敗しました。(final_file:%s)\n%s\n", file_name.c_str(), (lua_file_pass.end() - 1)->c_str(), err);
				return -1;
			}
		}
		/*if (lua_pcall(L,0,LUA_MULTRET,0)){
			const char* err = lua_tostring(L, -1);
			printf("LuaStateHolderの初期化に失敗しました。:Luaステートの実行に失敗しました。(final_file:%s)\n%s\n", (lua_file_pass.end()-1)->c_str(), err);
			return -1;
		}*/
		initialized = true;
		printf("LuaStateHolderが初期化されました。:%s\n", (lua_file_pass.end() - 1)->c_str());
		return 0;
	}
	else{
		printf("LuaStateHolderの初期化に失敗しました。:LuaStateがセットされていません。\n");
		return -1;
	}
}

//lua_State* LuaStateHolder::GetCoroutineState(const string& co_name){
//	if (!initialized){ 
//		return nullptr; 
//		printf("LuaStateHolder::GetCoroutineState 初期化される前に呼び出されました。\n");
//	}
//	lua_settop(L, 0);
//	lua_getglobal(L, co_name.c_str());
//	//指定された関数が存在しなかったらエラー
//	if (lua_type(L, 1) != LUA_TFUNCTION){
//		lua_settop(L, 0);
//		printf("LuaStateHolder::GetCoroutineState 指定された関数は存在しません。:%s\n", co_name.c_str());
//		return nullptr;
//	}
//	lua_settop(L, 0);
//	return lua_newthread(L);
//}

//lua_State* LuaStateHolder::GetFunctionState(const string& func_name){
//	if (!initialized){
//		return nullptr;
//		printf("LuaStateHolder::GetFunctionState 初期化される前に呼び出されました。\n");
//	}
//	lua_getglobal(L, func_name.c_str());
//	//指定された関数が存在しなかったらエラー
//	if (lua_type(L, 1) != LUA_TFUNCTION){
//		lua_settop(L, 0);
//		printf("LuaStateHolder::GetFunctionState 指定された関数は存在しません。:%s\n", func_name.c_str());
//		return nullptr;
//	}
//	lua_settop(L, 0);
//	return L;
//}

lua_State* LuaStateHolder::GetLuaState()const{ 
	return L; 
}

int LuaStateHolder::GetUsingMemory()const{
	if (L == nullptr){ return 0; }
	return lua_gc(L, LUA_GCCOUNT, 0);
}
