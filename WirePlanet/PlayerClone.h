#pragma once
#include "PlayerB.h"
class PlayerClone :
	public PlayerB
{
public:
	PlayerClone(long, long, const std::string&);
	~PlayerClone()override;
private:
	void DieState()override;
	//�p�����[�^�Z�b�g�֐�
	void SetParameterByLevel(int)override;
	virtual void NormalState()override;
};

