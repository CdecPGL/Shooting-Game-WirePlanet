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

	void StartEvent(); //�o�^���ꂽ�C�x���g���J�n
	void ResistEvent(const std::string&, int = 0); //�C�x���g�o�^(�C�x���gID�A�p�����[�^)

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
