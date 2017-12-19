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

	void GoTarget(); //�^�[�Q�b�g�֌����킹��
private:
	int _energy; //�G�l���M�[��
	int _suspend_time; //��������
	int _size;
	enum ActionMode{ Stay, GoToTarget, MovingToTarget };
	ActionMode _mode;

	void ProcessCollisionWithCharacter()override;

	void Dispose(bool); //����(�o���l���Z���邩)
};
