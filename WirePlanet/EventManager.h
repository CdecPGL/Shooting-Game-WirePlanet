#pragma once

#undef CreateEvent

#include"Trigger.h"
#include<unordered_map>
#include<memory>

class Game;
class LuaStateHolder;

class EventManager
{
public:
	~EventManager();
	EventManager(const EventManager&) = delete;
	static EventManager& GetInstance();
	int Init();
	int Reset();
	int Process();
	int Reload();
	void Clean(); //使われていないイベント、トリガーを破棄
	void Clear(); //全てのイベント、トリガーを破棄
	void AllRecycle(); //全てrecycle

	void Update();

	std::shared_ptr<Trigger> CreateTrigger(const std::string&); //トリガー作成(トリガー名)
	int CreateEvent(const std::string&, int=0); //イベント作成(イベント名、パラメータ)

	int RecycleTrigger(int); //トリガーリサイクル(トリガーID)
	int RecycleEvent(int); //イベントリサイクル(イベントID)

	int GetEventCount()const;
	int GetTriggerCount()const;
	int GetUsingMemory()const;

	void SetGameScenePointer(Game*); //呼び出す地イベント、トリガーが全てリサイクルされます
private:
	Game* game;
	std::unique_ptr<LuaStateHolder> _event_state;
	std::unique_ptr<LuaStateHolder> _trigger_state;

	int id_counter_trg;
	EventManager();

	void process_trigger();

	std::unordered_map<int, std::shared_ptr<Trigger>> triggers;

	std::vector<std::shared_ptr<Trigger>> res_list_trg;
	std::vector<int> rcl_list_trg;

	void resist_trigger();
	void recycle_trigger();
	void recycle_event();

	std::vector<std::shared_ptr<Trigger>> unused_triggers;

	int InitLuaState();
};

