#include "player.h"
#include "CharacterManager.h"
#include"CreateCharaFuncs.h"
#include"DebugWindow.h"
#include"LowStateIndivisualManager.h"
#include"SoundManager.h"

using namespace std;

const int LEVEL_RESISTANCE(200); //レベルの上がりに草

extern const int Blow_Rotation_Speed;

//コンストラクタ
Player::Player(double m_friction,long m_HP,double m_maxspeed,long m_mt,const string& m_ad)
	:Character(m_friction, m_HP, ObjectBelongingTo::Friend, m_mt, m_ad), maximum_speed(m_maxspeed), bullet_flag(false)/*, _total_exp(0)*/, _level(1), _input(0), _pinput(0), _pushed_input(0),_level_resistance(LEVEL_RESISTANCE)
,_total_exp_s(new int(0)){
	_attribute = PLAYER;
}

//デストラクタ
Player::~Player(){
}

//初期化
int Player::Init(){
	ns_vars.resize(10, 0);
	SetNormalStateLowStateFuncMap();
	SetParameterByLevel(1);
	return 0;
}

//上位状態関数
 void Player::NormalState(){
	 ns_ls_mgr->Update();
	 UniversalLowStateProc();
}

 void Player::DieState(){
	 Character::DieState();
 }

//描画
void Player::Draw(){
	Character::Draw();
	SetFormatDebugData(0, 0, GetColor(255, 255, 255), "x,y=%.1lf,%.1lf,ground_flag=%d", position.x, position.y, ground_flag);
	SetFormatDebugData(0,16,GetColor(255,255,255),"HP=%d/%d,r,vr=%.1lf,%.1lf,t,vh=%.lf,%.lf",HP,p_HP,r,vr,t,vh);
	SetFormatDebugData(0, 32, GetColor(255, 255, 255), "被弾からの経過時間/無敵時間=%d/%d", time_from_attacked, matchless_time);
	SetFormatDebugData(0, 48, GetColor(255, 255, 255), "hit_stop_cnt=%d,exp=%d,level=%d", hit_stop_cnt,/*_total_exp*/_total_exp_s.get(),_level);
}

//メッセージ受け取り
bool Player::ReceiveMessage(const string& mes, double p){
	if (mes == "AddExp"){
		AddExperiencePoint((int)p);
	}
	else{
		return Character::ReceiveMessage(mes, p);
	}
	return true;
}

//経験値取得
void Player::AddExperiencePoint(int exp){
//	_total_exp += exp;
	_total_exp_s.instance() += exp;
	_total_exp_s.synchronize();
	if (_total_exp_s.get() < 0){ _total_exp_s.set(0); }
	int nlevel = LevelCalculater(_total_exp_s.get(), _level_resistance);
	if (nlevel != _level){
		ChangeLevel(nlevel);
	}
}

int Player::GetExpDifferenceToNextLevel()const{
	return ExpCaluculater(_level + 1, _level_resistance) - ExpCaluculater(_level, _level_resistance);
}

int Player::GetExpRemainingToNextLevel()const{
	return ExpCaluculater(_level + 1, _level_resistance) - _total_exp_s.get();
}

//レベル取得
int Player::GetLevel()const{ return _level; }

//経験値取得
unsigned int Player::GetEXP()const{ return (unsigned)_total_exp_s.get(); }

//レベルに応じたパラメータセット
void Player::SetParameterByLevel(int l){

}

//レベル計算
int Player::LevelCalculater(int t_exp,int ul){
	return (int)sqrt(t_exp/ul + 1);
}

//経験値逆算機
int Player::ExpCaluculater(int l, int ul){
	return l*l*ul - 1;
}

//レベル変更処理
void Player::ChangeLevel(int nl){
	if (nl > _level){
		SoundManager::GetInstance().PlaySoundByID("PLevelUp");
	}
	_level = nl;
	SetParameterByLevel(nl);
}

void Player::SetInput(unsigned int in){
	_pinput = _input;
	_input = in;
	_pushed_input = ~_pinput & _input;
}

void Player::ClearInput(){
	_pinput = 0;
	_input = 0;
	_pushed_input = 0;
}

bool Player::ButtonPush(unsigned int b, PlayerInput::MultiInput mlt)const{
	if (mlt == PlayerInput::AND){
		return (_pushed_input & b) == b ? true : false;
	}
	else if (mlt == PlayerInput::OR){
		return _pushed_input & b ? true : false;
	}
	else{
		return false;
	}
}

bool Player::ButtonStatus(unsigned int b, PlayerInput::MultiInput mlt)const{
	if (mlt == PlayerInput::AND){
		return (_input & b) == b ? true : false;
	}
	else if (mlt == PlayerInput::OR){
		return _input & b ? true : false;
	}
	else{
		return false;
	}
}

//衝突
//void Player::Hit(){
//	if(collision_character.empty()==false && matchless == false){
//		std::vector<Character*>::iterator it;
//		for(it=collision_character.begin();it!=collision_character.end();it++){
//			//当たった相手の自分から見た向きを求める
//			double e_direction = atan2((*it)->GetPositionXY().y-position.y,(*it)->GetPositionXY().x-position.x);
//			//敵と反対向きに飛ばされる
//			vh = 5*cos(e_direction + DX_PI);
//			vr = 5*sin(e_direction + DX_PI);
//			matchless = true;
//			HP -= 10;
//			time_from_attacked = 0; //ダメージからの計k時間リセット
//		}
//	}
//}

//NormalState下位状態関数セット
void Player::SetNormalStateLowStateFuncMap(){
	
}

//下位状態共通処理
void Player::UniversalLowStateProc(){

}

