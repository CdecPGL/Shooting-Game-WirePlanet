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

//	lua_State* GetCoroutineState(const std::string&); //�ێ����Ă���X�e�[�g����V�����X�e�[�g���쐬���ēn��
//	lua_State* GetFunctionState(const std::string&); //�ێ����Ă���X�e�[�g�̃|�C���^��n��
	lua_State* GetLuaState()const; //�ێ����Ă���X�e�[�g�̃|�C���^��n��
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

