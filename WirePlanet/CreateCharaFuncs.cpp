#include"CreateCharaFuncs.h"
#include"CharacterManager.h"
#include"bullet.h"
#include"DropedEnergyBall.h"
#include<cmath>

using namespace std;

shared_ptr<CharacterBase> CreateCharaFuncs::FireBullet(const string& init_id, double dir, double speed, double atk_mag, const CharacterBase* shtr, double rv){
	return FireBulletAssignSuspend(init_id, dir, speed, atk_mag, shtr, -1, rv);
}
shared_ptr<CharacterBase> CreateCharaFuncs::FireBulletAssignSuspend(const string& init_id, double dir, double speed, double atk_mag, const CharacterBase* shtr,int sus, double rv){
	shared_ptr<Bullet> b = static_pointer_cast<Bullet>(CharacterManager::GetInstance().CreateAndResistObject(BULLET, init_id));
	b->SetAttackPowerMagnification(atk_mag);
	b->SetVelocity(speed * cos(dir / 180.0*DX_PI) + shtr->GetVH(), speed * sin(dir / 180.0*DX_PI) + shtr->GetVR());
	b->SetRotation(360.0 - dir);
	b->SetBelongingPlanet(shtr->GetBelongingPlanet());
	b->SetTR(shtr->GetT(), shtr->GetR());
	b->SetRatationVelocity(rv);
	b->SetObjectBelongingTo(shtr->GetBelongingTo());
	b->SetSuspendFrame(sus);
	b->SetTarget(shtr->GetTarget());
	return b;
}

shared_ptr<CharacterBase> CreateCharaFuncs::ThrowSword(double dir, double speed,double atk_mag, int thrower_id, int ini_pos_dis){
	shared_ptr<CharacterBase> shtr_sp = CharacterManager::GetInstance().GetSharedPointer(thrower_id);
	if (shtr_sp == nullptr){ return nullptr; }
	shared_ptr<Bullet> b = static_pointer_cast<Bullet>(CharacterManager::GetInstance().CreateAndResistObject(BULLET, "PlayerBSword"));
	if (b == nullptr){ return nullptr; }
	double r = shtr_sp->GetR();
	b->SetAttackPowerMagnification(atk_mag);
	b->SetVelocity(speed * cos(dir / 180.0*DX_PI), speed * sin(dir / 180.0*DX_PI));
	b->SetBelongingPlanet(shtr_sp->GetBelongingPlanet());
	b->SetTR(shtr_sp->GetT() + (double)ini_pos_dis * cos(dir / 180.0*DX_PI)* 180.0 / r / DX_PI, shtr_sp->GetR() + (double)ini_pos_dis * sin(dir / 180.0*DX_PI));
//	b->SetObjectBelongingTo(shtr_sp->GetBelongingTo());
	b->SetObjectBelongingTo(ObjectBelongingTo::Neutral);
	if (shtr_sp){
		b->SetTarget(shtr_sp);
	}
	return b;
}

//ドロップエネルギーボール作成(エネルギー量、生成元)
shared_ptr<CharacterBase> CreateCharaFuncs::CreateDropEnergyBall(int energy, const CharacterBase* crtr,bool gtf){
	shared_ptr<DropedEnergyBall> deb = static_pointer_cast<DropedEnergyBall>(CharacterManager::GetInstance().CreateAndResistObject(ITEM, "DropedEnergyBall"));
	deb->SetEnergy(energy);
	deb->SetTarget(crtr->GetTarget());
	deb->SetBelongingPlanet(crtr->GetBelongingPlanet());
	deb->SetTR(crtr->GetT(), crtr->GetR());
	deb->SetPolarVelocity(5, GetRand(360));
	if (gtf){ deb->GoTarget(); }
	return deb;
}

//ドロップエネルギーボール散布(エネルギー量、生成元)
void CreateCharaFuncs::SprayDropEnergyBall(int energy, const CharacterBase* crtr, bool gtf){
	int max_energy = 100 + energy/100;
	do{
		int crt_energy;
		if (energy > max_energy){
			crt_energy = max_energy;
			energy -= max_energy;
		}
		else{
			crt_energy = energy;
			energy = 0;
		}
		CreateDropEnergyBall(crt_energy, crtr,gtf);
	} while (energy>0);
}
