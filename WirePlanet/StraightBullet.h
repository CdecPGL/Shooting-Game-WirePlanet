#pragma once
#include "bullet.h"
class StraightBullet :
	public Bullet
{
public:
	//�R���X�g���N�^(�u�����h���[�h�A�u�����h�p�����[�^,�F16�i���l,�A�N�V�����t�@�C����)
	StraightBullet(int, int, int, const std::string&);
	~StraightBullet();
private:
	void move()override;
};
