#include "bullet.h"
#include"CharacterManager.h"
#include"Planet.h"
#include"CreateEffectFuncs.h"
#include"SoundManager.h"

using namespace std;

namespace{
	const double BulletPowerRate(15.0); //’e‚ª˜f¯‚ÉÕ“Ë‚µ‚½Žž‰Á‚¦‚é—Í‚Ì”{—¦
	const int SUSPEND_TIME(600); //’e‚ªŽc‚éŽžŠÔ
}

//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Bullet::Bullet(int m_bm,int m_bp,int m_c,const string& m_ad)
	:CharacterBase(0,1,ObjectBelongingTo::Neutral,m_ad),blend_mode(m_bm),blend_param(m_bp),_inc_cntr(0),_suspend_frame(SUSPEND_TIME){
	Convert16ColToRGBColor(m_c, col, col + 1, col + 2);
	_attribute = BULLET;
}

//ˆÊ’u‚Ì‹êS
void Bullet::Update(){
	move();
	CommonUpdateProcess();
	++_inc_cntr;
	if (_inc_cntr >= _suspend_frame){
		CharacterManager::GetInstance().DeleteObject(GetCharID());
		CreateEffectFuncs::CreateNormalEffect("BulletDisappear", t, r, myplanet);
	}
}

//•`‰æ
void Bullet::Draw(){
	if (blend_mode != DX_BLENDMODE_NOBLEND){
		SetDrawBlendMode(blend_mode, blend_param);
	}
	if (col[0] < 0xFF || col[1] < 0xFF || col[2] < 0xFF){
		SetDrawBright(col[0], col[1], col[2]);
	}
	CharacterBase::Draw();
	if (col[0] < 0xFF || col[1] < 0xFF || col[2] < 0xFF){
		SetDrawBright(255, 255, 255);
	}
	if (blend_mode != DX_BLENDMODE_NOBLEND){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

//Õ“Ëˆ—
void Bullet::ProcessCollisionWithPlanet(){
	//˜f¯
	for (CollisionDataWithPlanet& c : col_data_p){
		if(myplanet)myplanet->AddForce(c.t, m * BulletPowerRate);
		CharacterManager::GetInstance().DeleteObject(GetCharID());
		CreateEffectFuncs::CreateNormalEffect("BulletDisappear", t, r, myplanet);
	}
	col_data_p.clear();
}
void Bullet::ProcessCollisionWithCharacter(){
	//ƒLƒƒƒ‰
	for (CollisionDataWithCharacter& c : col_data_c){
		CharacterManager::GetInstance().DeleteObject(GetCharID());
		CreateEffectFuncs::CreateNormalEffect("BulletDisappear",t, r, myplanet);
		SoundManager::GetInstance().PlaySoundByID("BulletHit");
		break;
	}
	col_data_c.clear();
}

//ƒZƒbƒg
void Bullet::SetVelocity(double mw, double mvr){
	vh = mw; vr = mvr;
}
void Bullet::SetSuspendFrame(int sf){
	_suspend_frame = sf;
	if (_suspend_frame < 0){
		_suspend_frame = SUSPEND_TIME;
	}
}

//
int Bullet::Convert16ColToRGBColor(int col16, int* r, int* g, int* b){
	if (col16 < 0 || 0xFFFFFF < col16){
		return -1; 
		*r = 0;
		*g = 0;
		*b = 0;
	}
	*r = (col16 >> 16) & 0xFF;
	*g = (col16 >> 8) & 0xFF;
	*b = col16 & 0xFF;
	return 0;
}
