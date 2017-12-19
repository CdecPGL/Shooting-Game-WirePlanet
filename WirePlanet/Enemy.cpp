#include "Enemy.h"
#include"EnemyMoverManager.h"
#include"CreateCharaFuncs.h"
#include"CharacterManager.h"
#include"Planet.h"
#include"LaserManager.h"
#include"EnemyMoverManager.h"
#include"SoundManager.h"
#include"CharacterManager.h"
#include"CreateEffectFuncs.h"
#include"GameCounter.h"

using namespace std;

namespace{
	const double LANDED_DROP_ENERGY_MAGNIFICATION(7); //上陸時に敵が落とすエネルギーの倍率
	const int EXPLOSION_SUSPEND(180); //上陸から爆発までの時間
	const int ALERT_TIME(90); //警告をする時間
	const int EXPLOSION_POWER(20000);
}

//コンストラクタ(依存)
Enemy::Enemy(int m_hp, double m_f, int m_mt, int m_de, const string& m_mvr, const string& m_ad)
	:Character(m_f,m_hp,ObjectBelongingTo::Enemy,m_mt,m_ad),distance_toplayer(0),_mvr_id(-1),mvr_name(m_mvr),mvr_state(0),_land_flag(false),_drop_energy(m_de),emm(EnemyMoverManager::GetInstance()), _mvr_crt_obj_cnt(0),_explosion_cntr(0){
	action->StartActionByTag("idol");
	_attribute = ENEMY;
}

Enemy::~Enemy(void){
}

int Enemy::Init(){
//	printf("|e-0|");
	Character::Init();
//	printf("|e-1|");
	_mvr_id = emm.CreateMover(mvr_name,this);
//	printf("|e-2|");
//	printf("Enemy is inited.(%d)\n", _mvr_id);
	if (_mvr_id < 0){
		printf("Enemy::Init EnemyMoverの作成に失敗しました。(%d,%s)\n", _mvr_id, mvr_name.c_str());
	}
	return 0;
}

//衝突処理
void Enemy::ProcessCollisionWithCharacter(){
	Character::ProcessCollisionWithCharacter();
	if (ground_flag == false){ 
		hit_stop_cnt = 0;
	} //空中ではヒットストップを無効化する
}

//ダメージを受けた
void Enemy::ReceivedAttack(int d){
	if (_muteki)return;
	if (ground_flag == false){
		hit_stop_cnt = 0;
		time_from_attacked = 0;
		p_HP -= d;
		if (p_HP <= 0){ Died(); }
	}
	else{
		Character::ReceivedAttack(d);
	}
}

//更新
void Enemy::NormalState(){
	move();
}

//動く
void Enemy::move(){
	if (mvr_state < 2){
		if (_mvr_id >= 0){
			if (emm.ExcuteMover(_mvr_id)){
				mvr_state = 2;
			}
		}
	}
	if (_land_flag){
		if (_explosion_cntr <= 0){
			if (myplanet){
				myplanet->AddForce(t, EXPLOSION_POWER);
			}
			CreateEffectFuncs::CreateNormalEffect("Explosion", t, r, myplanet);
			if (_mvr_id >= 0){ emm.DeleteMover(_mvr_id); }
			_mvr_id = -1;
			GameCounter::LevelCounter::AddExplodedEnemy(1);
			endAllMCObject();
			CharacterManager::GetInstance().DeleteObject(GetCharID());
			SoundManager::GetInstance().PlaySoundByID("Explosion");
		}
		if (_explosion_cntr < ALERT_TIME){
			if (_explosion_cntr % 4 == 0){
				_col_revers_flag = true;
				SoundManager::GetInstance().PlaySoundByID("ExplosionAlert");
			}
		}
		--_explosion_cntr;
	}
}

void Enemy::DieState(){
	CreateCharaFuncs::SprayDropEnergyBall((int)(_drop_energy * (_land_flag ? LANDED_DROP_ENERGY_MAGNIFICATION : 1)), this, _land_flag ? false : true);
	GameCounter::LevelCounter::AddBaseScore(_drop_energy * (_land_flag ? LANDED_DROP_ENERGY_MAGNIFICATION : 1));
	GameCounter::LevelCounter::AddDefeatedEnemy(1);
	if (_mvr_id >= 0){ emm.DeleteMover(_mvr_id); }
	_mvr_id = -1;
	endAllMCObject();
	Character::DieState();
	SoundManager::GetInstance().PlaySoundByID("EnemyDead");
}

//ムーバ用
int Enemy::StartReverseAction(const string& tag){
	printf("アクションの逆再生は未実装です。");
	return -1;
}
bool Enemy::CheckActionPlaying()const{
	return action->CheckActionPlaying();
}
bool Enemy::GetGroundFrag()const{ return ground_flag; }
void Enemy::AddDefaultGravity(){ Gravity(); }
double Enemy::GetTargetDistanceT()const{
	double dis_deg = abs(GetTargetT() - t);
	return dis_deg > 180.0 ? 360.0 - dis_deg : dis_deg;
}
double Enemy::GetTargetDistanceR()const{
	return abs(GetTargetR() - r);
}
void Enemy::ProcessFriction(){
	if (ground_flag){ Friction(); }
}
void Enemy::AddForceToPlanet(double t_deg,double f){
	if (myplanet){ myplanet->AddForce(t_deg,f); }
}

int Enemy::GetBelongingPlanetRadius()const{
	return myplanet ? (int)myplanet->GetRadius() : -1;
}

void Enemy::Landed(){
	_land_flag = true;
	_explosion_cntr = EXPLOSION_SUSPEND;
}

double Enemy::GetHPPercentage()const{
	return (double)p_HP / (double)HP * 100.0;
}

//ムーバー作成オブジェクト系関数
void Enemy::endAllMCObject(){
	for (auto& mco : _mvr_created_objects){
		if (mco.second.first == false || mco.second.second.expired() == true){ continue; }
		mco.second.second.lock()->End();
	}
	_mvr_created_objects.clear();
}

int Enemy::setMvrCrtObj(weak_ptr<Object> obj,bool df){
	if (obj.expired()){ return -1; }
	_mvr_created_objects.insert(make_pair(_mvr_crt_obj_cnt, make_pair(df,obj)));
	return _mvr_crt_obj_cnt++;
}

Object* Enemy::GetMCObject(int oid){
	auto it = _mvr_created_objects.find(oid);
	if (it == _mvr_created_objects.end()){ return nullptr; }
	if ((*it).second.second.expired()){
		_mvr_created_objects.erase(it);
		return nullptr;
	}
	return (*it).second.second.lock().get();
}

int Enemy::EndMCObject(int oid){
	auto it = _mvr_created_objects.find(oid);
	if (it == _mvr_created_objects.end()){ return -1; }
	if ((*it).second.second.expired()){
		_mvr_created_objects.erase(it);
		return -1;
	}
	if ((*it).second.second.lock()->End() < 0){ return -2; }
	_mvr_created_objects.erase(it);
	return 0;
}

int Enemy::FireBullet(const string& iid, double dir, double spd, double atk){
	return setMvrCrtObj(CreateCharaFuncs::FireBullet(iid, dir, spd, atk, this),false);
}
int Enemy::FireRotatingBullet(const string& iid, double dir, double spd, double atk, double rv){
	return setMvrCrtObj(CreateCharaFuncs::FireBullet(iid, dir, spd, atk, this, rv),false);
}
int Enemy::FireLaser(int w, int s, int d, int m_r, int g, int b, double dir_deg){
	shared_ptr<Laser> ls = LaserManager::GetInstance().CreateLaser("NULL");
	ls->SetBelongingPlanet(myplanet);
	ls->SetTR(t, r);
	ls->SetWidth(w);
	ls->SetSuspension(s);
	ls->SetDuration(d);
	ls->SetColor(m_r, g, b);
	ls->SetDirection(dir_deg);
	ls->SetObjectBelongingTo(_obj_belonging_to);
	return setMvrCrtObj(ls,true);
}
int Enemy::CreateCharacter(const string& name){
	shared_ptr<CharacterBase> c = CharacterManager::GetInstance().CreateAndResistObject(CharaAtrribtute::ENEMY,name);
	c->SetBelongingPlanet(myplanet);
	c->SetTR(t, r);
	c->SetTarget(target);
	return setMvrCrtObj(c, false);
}

