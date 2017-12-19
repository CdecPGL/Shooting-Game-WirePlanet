#pragma once
#include "bullet.h"
class Boomerang :
	public Bullet
{
public:
	//�R���X�g���N�^(���Z�u�����h���[�h�A�u�����h�p�����[�^�A�F16�i���l�AAction�t�@�C����)
	Boomerang(int, int,int, const std::string&);
	~Boomerang();
	int Init()override;
private:
	void move()override;
	void ProcessCollisionWithCharacter()override;
	void ProcessCollisionWithPlanet()override;
};

