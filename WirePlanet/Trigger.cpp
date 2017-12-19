#include "Trigger.h"
#include"EventManager.h"
#include"LuaStateHolder.h"
#include"luabind/luabind.hpp"
#include"Game.h"

#undef CreateEvent

using namespace std;

Trigger::Trigger(int id) :initialized(false), ID(id), game(nullptr)
{
}

Trigger::~Trigger()
{
}

int Trigger::Init(){
	if (initialized){ return -1; }
	if (game == nullptr){ return -2; }
	initialized = true;
	return 0;
}

int Trigger::End(){
	if (!initialized){ return -1; }
	events.clear();
	initialized = false;
	game = nullptr;
	return 0;
}

void Trigger::ResistEvent(const string& e_id, int param){
	events.push_back(EventData(e_id, param));
}

void Trigger::StartEvent(){
	for (EventData& ed : events){
		EventManager::GetInstance().CreateEvent(ed.event_id, ed.param);
	}
}

int Trigger::GetID()const{ return ID; }
const string& Trigger::GetTriggerName()const{ return trigger_name; }
Game* Trigger::GetGameScenePointer()const{ return game; }
void Trigger::SetTriggerName(const string& tn){ trigger_name = tn; }
void Trigger::SetGameScenePointer(Game* g){ game = g; }

Trigger::EventData::EventData(const string& e_id, int p) :event_id(e_id), param(p){}