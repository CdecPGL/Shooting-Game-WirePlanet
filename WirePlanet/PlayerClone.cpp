#include "PlayerClone.h"

namespace{

}

PlayerClone::PlayerClone(long hp, long mt, const std::string& ad) :PlayerB(1,hp,0,mt,ad)
{
}


PlayerClone::~PlayerClone()
{
}

void PlayerClone::DieState(){
	Character::DieState();
}

void PlayerClone::SetParameterByLevel(int l){
	PlayerB::SetParameterByLevel(l);
	_shoot_interval *= 1;
	_shoot_power /= 1;
	if (_shoot_power < 1){ _shoot_power = 1; }
}

void PlayerClone::NormalState(){
	unsigned int in = 0;
	in |= PlayerInput::B;
	Player::SetInput(in);
	PlayerB::NormalState();
	--p_HP;
	if (p_HP <= 0){
		Died();
	}
}
