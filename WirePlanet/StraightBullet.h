#pragma once
#include "bullet.h"
class StraightBullet :
	public Bullet
{
public:
	//コンストラクタ(ブレンドモード、ブレンドパラメータ,色16進数値,アクションファイル名)
	StraightBullet(int, int, int, const std::string&);
	~StraightBullet();
private:
	void move()override;
};
