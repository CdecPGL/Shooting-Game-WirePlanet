#pragma once

#include<string>
#include<memory>
#include"boost/thread/mutex.hpp"

//シングルトン

class LuaStateHolder;
struct lua_State;
class Enemy;

class EnemyMoverManager
{
public:
	~EnemyMoverManager();
	EnemyMoverManager(const EnemyMoverManager&) = delete;
	static EnemyMoverManager& GetInstance();
	int CreateMover(const std::string&, Enemy*); //ムーバー作成(ムーバー名,適用対象)
	int CreateMoverTS(const std::string&, Enemy*); //ムーバー作成。スレッドセーフ版(ムーバー名,適用対象)
	int DeleteMover(int); //ムーバー削除
	int ExcuteMover(int); //ムーバー実行(ムーバーID)
	int Init();
	int Reset();
	void Clean();
	void Clear();
	int Reload();
	//Luaが使用しているメモリをKBで返す
	int GetUsingMemory()const;
	int GetCount()const;
private:
	mutable boost::mutex _lua_mtx;
	bool initialized;
	EnemyMoverManager();
	std::unique_ptr<LuaStateHolder> _lua_state_holder;
};
