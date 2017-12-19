#pragma once

namespace GameCounter
{
	namespace LevelCounter{
		void ResetLevelCount();
		void ApplyLevelCountToGPI();
		void AddBaseScore(unsigned int);
		void AddDefeatedEnemy(unsigned int);
		void AddShootedBullet(unsigned int);
		void AddExplodedEnemy(unsigned int);
		void AddReceivedDamage(unsigned int);
		void AddMadeClone(unsigned int);
		unsigned int GetBaseScore();
		unsigned int GetDefeatedEnemy();
		unsigned int GetShootedBullet();
		unsigned int GetExplodedEnemy();
		unsigned int GetReceivedDamage();
		unsigned int GetMadeClone();
	}
	/*void ResetGameCount();
	void ApplyGameCountToGPI();
	void AddBaseScore(unsigned int);
	void AddDefeatedEnemy(unsigned int);
	void AddShootedBullet(unsigned int);
	void AddExplodedEnemy(unsigned int);
	void AddReceivedDamage(unsigned int);
	unsigned int GetBaseScore();
	unsigned int GetDefeatedEnemy();
	unsigned int GetShootedBullet();
	unsigned int GetExplodedEnemy();
	unsigned int GetReceivedDamage();
	void SetTotalScore(unsigned int);*/
};

