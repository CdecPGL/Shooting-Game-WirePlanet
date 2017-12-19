#pragma once
#include "bullet.h"
class GuidedBullet :
	public Bullet
{
public:
	GuidedBullet(int,int,int,const std::string&);
	~GuidedBullet();

private:
	void move()override;

};

