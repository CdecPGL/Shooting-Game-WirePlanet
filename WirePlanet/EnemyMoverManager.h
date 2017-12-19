#pragma once

#include<string>
#include<memory>
#include"boost/thread/mutex.hpp"

//�V���O���g��

class LuaStateHolder;
struct lua_State;
class Enemy;

class EnemyMoverManager
{
public:
	~EnemyMoverManager();
	EnemyMoverManager(const EnemyMoverManager&) = delete;
	static EnemyMoverManager& GetInstance();
	int CreateMover(const std::string&, Enemy*); //���[�o�[�쐬(���[�o�[��,�K�p�Ώ�)
	int CreateMoverTS(const std::string&, Enemy*); //���[�o�[�쐬�B�X���b�h�Z�[�t��(���[�o�[��,�K�p�Ώ�)
	int DeleteMover(int); //���[�o�[�폜
	int ExcuteMover(int); //���[�o�[���s(���[�o�[ID)
	int Init();
	int Reset();
	void Clean();
	void Clear();
	int Reload();
	//Lua���g�p���Ă��郁������KB�ŕԂ�
	int GetUsingMemory()const;
	int GetCount()const;
private:
	mutable boost::mutex _lua_mtx;
	bool initialized;
	EnemyMoverManager();
	std::unique_ptr<LuaStateHolder> _lua_state_holder;
};
