#pragma once

#include<string>
#include"lua.hpp"
#include<vector>
#include<mutex>

class LuaStateHolder
{
public:
	LuaStateHolder();
	~LuaStateHolder();

	void SetLuaState(lua_State*);
	void SetLuaFilePass(const std::string&);

	int Init();

	int Reload(lua_State*);

//	lua_State* GetCoroutineState(const std::string&); //保持しているステートから新しいステートを作成して渡す
//	lua_State* GetFunctionState(const std::string&); //保持しているステートのポインタを渡す
	lua_State* GetLuaState()const; //保持しているステートのポインタを渡す
	int GetUsingMemory()const;

	void MutexLockLuaState();
	void MutexUnlockLuaState();

private:
	bool initialized;
	std::vector<std::string> lua_file_pass;
	lua_State* L;
	static std::recursive_mutex _ls_mtx;

	int LoadLuaState();
};

