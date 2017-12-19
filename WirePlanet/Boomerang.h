#pragma once
#include "bullet.h"
class Boomerang :
	public Bullet
{
public:
	//コンストラクタ(加算ブレンドモード、ブレンドパラメータ、色16進数値、Actionファイル名)
	Boomerang(int, int,int, const std::string&);
	~Boomerang();
	int Init()override;
private:
	void move()override;
	void ProcessCollisionWithCharacter()override;
	void ProcessCollisionWithPlanet()override;
};

