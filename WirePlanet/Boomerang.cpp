#include "Boomerang.h"

using namespace std;

const double RETURN_POWER(0.2);
const double ROTATION_SPEED(50.0);

Boomerang::Boomerang(int m_bm, int m_bp,int m_c, const string& af) :Bullet(m_bm, m_bp,m_c, af)
{
}


Boomerang::~Boomerang()
{
}

int Boomerang::Init(){
	Bullet::Init();
	col_with_friend = true;
	return 0;
}

void Boomerang::move(){
	if (target){
		double t_t = GetTargetTFromMe();
		double t_r = GetTargetRFromMe();
		double t_dir = atan2(t_r, t_t);
		vh += RETURN_POWER * cos(t_dir);
		vr += RETURN_POWER * sin(t_dir);
	}
	rotation += (reverse ? -1 : 1)*ROTATION_SPEED;
	_inc_cntr=0; //カウンタをリセットすることで自然消滅を防ぐ
}

void Boomerang::ProcessCollisionWithPlanet(){
	//惑星
	for (CollisionDataWithPlanet& c : col_data_p){
		r = c.r;
		if (vr < 0){ vr = 0; }
	}
	col_data_p.clear();
}

void Boomerang::ProcessCollisionWithCharacter(){
	col_data_c.clear();
}
