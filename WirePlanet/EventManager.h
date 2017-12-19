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
	void Clean(); //�g���Ă��Ȃ��C�x���g�A�g���K�[��j��
	void Clear(); //�S�ẴC�x���g�A�g���K�[��j��
	void AllRecycle(); //�S��recycle

	void Update();

	std::shared_ptr<Trigger> CreateTrigger(const std::string&); //�g���K�[�쐬(�g���K�[��)
	int CreateEvent(const std::string&, int=0); //�C�x���g�쐬(�C�x���g���A�p�����[�^)

	int RecycleTrigger(int); //�g���K�[���T�C�N��(�g���K�[ID)
	int RecycleEvent(int); //�C�x���g���T�C�N��(�C�x���gID)

	int GetEventCount()const;
	int GetTriggerCount()const;
	int GetUsingMemory()const;

	void SetGameScenePointer(Game*); //�Ăяo���n�C�x���g�A�g���K�[���S�ă��T�C�N������܂�
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

