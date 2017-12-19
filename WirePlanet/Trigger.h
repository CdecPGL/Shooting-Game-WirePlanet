#pragma once

#include<vector>
#include<string>

class Game;

class Trigger
{
public:
	Trigger(int);
	virtual ~Trigger();

	int Init();
	int End();

	void StartEvent(); //登録されたイベントを開始
	void ResistEvent(const std::string&, int = 0); //イベント登録(イベントID、パラメータ)

	void SetTriggerName(const std::string&);

	int GetID()const;
	void SetGameScenePointer(Game*);
	const std::string& GetTriggerName()const;
	Game* GetGameScenePointer()const;
private:
	Game* game;
	const int ID;
	bool initialized;
	struct EventData{
		EventData(const std::string&, int);
		std::string event_id;
		int param;
	};
	std::vector<EventData> events;

	std::string trigger_name;
};
