#include "DropedEnergyBall.h"
#include"CharacterManager.h"
#include"GraphicController2D.h"
#include"SoundManager.h"

using namespace std;

const string ACTION_ID("DropedEnergyBall");
const int MINIMUM_SIZE(4);

DropedEnergyBall::DropedEnergyBall() :Item(ACTION_ID), _size(0), _suspend_time(300), _mode(Stay)
{
	action->StartActionByTag("idol");
}


DropedEnergyBall::~DropedEnergyBall()
{
}

void DropedEnergyBall::Update(){
	switch (_mode)
	{
	case DropedEnergyBall::Stay:
		if (_suspend_time <= 0){
			Dispose(false);
		}
		else if (_suspend_time < 60){
			visible = (_suspend_time % 2) ? true : false;
		}

		vh -= (vh >= 0 ? 1 : -1) * 0.2;
		vr -= (vr >= 0 ? 1 : -1) * 0.2;

		--_suspend_time;
		break;
	case DropedEnergyBall::GoToTarget:
		if (target == nullptr){ _mode = Stay; break; }
		_obj_belonging_to = ObjectBelongingTo::None;
		StartHermiteMove(Vector2D<double>(target->GetT(),target->GetR()),Vector2D<double>(vh/r/DX_PI*180.0,vr) * 60,Vector2D<double>(0.0,0.0),30);
		vh = 0; vr = 0;
		_mode = MovingToTarget;
		ProcessAutoMove();
		break;
	case DropedEnergyBall::MovingToTarget:
		if (IsAutoMoving() == false){
			Dispose(true);
		}
		ProcessAutoMove();
		break;
	default:
		break;
	}
	action->Update();
	UpdateHitAreaData();
	UpdateCurrentGHandle();
	ApplyVelocity();
	ConvertPTW();
}

void DropedEnergyBall::Draw(){
	double d_angle = GetDrawRotation();
	if (visible){
		gc2d::DrawRotaGraph((int)position.x, (int)position.y, (double)_size/16.0, d_angle / 180 * DX_PI, _current_ghandle, 1, reverse);
	}
}

void DropedEnergyBall::SetEnergy(int e){
	_energy = e;
	_size = MINIMUM_SIZE + (int)(sqrt(e) * (16.0 - MINIMUM_SIZE) / 16.0);
};
void DropedEnergyBall::SetSuspendTime(int s){ _suspend_time = s; };

void DropedEnergyBall::GoTarget(){
	_mode = GoToTarget;
}

void DropedEnergyBall::ProcessCollisionWithCharacter(){
	for (CollisionDataWithCharacter& c : col_data_c){
		if (c.char_attribute == PLAYER){
			Dispose(true);
			break;
		}
	}
	col_data_c.clear();
}

void DropedEnergyBall::Dispose(bool ef){
	CharacterManager::GetInstance().DeleteObject(GetCharID());
	if (ef){
		if (target){
			target->ReceiveMessage("AddExp", _energy);
			SoundManager::GetInstance().PlaySoundByID("GetEneBall");
		}
	}
}
