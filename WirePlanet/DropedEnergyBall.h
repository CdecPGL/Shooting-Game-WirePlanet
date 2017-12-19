#pragma once

#include"Item.h"

class DropedEnergyBall : public Item
{
public:
	DropedEnergyBall();
	~DropedEnergyBall();

	void Update()override;
	void Draw()override;

	void SetEnergy(int);
	void SetSuspendTime(int);

	void GoTarget(); //ターゲットへ向かわせる
private:
	int _energy; //エネルギー量
	int _suspend_time; //持続時間
	int _size;
	enum ActionMode{ Stay, GoToTarget, MovingToTarget };
	ActionMode _mode;

	void ProcessCollisionWithCharacter()override;

	void Dispose(bool); //消滅(経験値加算するか)
};
