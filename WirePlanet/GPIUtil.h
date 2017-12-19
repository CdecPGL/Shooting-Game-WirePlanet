#pragma once
namespace GPIUtil
{
	void AddGamePoint(unsigned int);
	void UseGamePoint(unsigned int);
	void SetHighScore(unsigned int);
	void AddPlayTime(unsigned long long, unsigned long long); //(ÉtÉåÅ[ÉÄÅAïb)
	void AddPlayCount();
	void SetFirstPlayDate(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
	void AddDefeatedEnemyCount(unsigned int);
	void AddShootedBulletCount(unsigned int);
	void AddMadeCloneCount(unsigned int);
	void AddGameOverCount();
	void AddContinueCount();
	void AddNormalClearCount();
	void AddAllClearCount();
	void AddCheatCount();
	void AddErrorCount();

	unsigned int GetGamePoint();
	unsigned int GetHightScore();
	unsigned long long GetPlayFrame();
	unsigned long long GetPlaySecond();
	unsigned int GetPlayCount();
	unsigned int GetDefeatedEnemy();
	unsigned int GetShootedBullet();
	unsigned int GetMadeClone();
	unsigned int GetGameOverCount();
	unsigned int GetContinueCount();
	unsigned int GetNormalClearCount();
	unsigned int GetAllClearCount();
	unsigned int GetCheatCount();
	unsigned int GetErrorCount();
	struct DateTime{
		unsigned int year;
		unsigned int month;
		unsigned int day;
		unsigned int hour;
		unsigned int minuits;
		unsigned int second;
	};
	DateTime GetFirstPlayDateTime();
};

