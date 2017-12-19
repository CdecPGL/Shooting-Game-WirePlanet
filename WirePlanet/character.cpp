#include "character.h"
#include"CollisionDatas.h"
#include"CharacterManager.h"

#include"CreateEffectFuncs.h"
#include"Key.h"
#include"Planet.h"

using namespace std;

extern const int CHARACTER_DEFAULT_HIT_STOP_FRAME(2); //ヒットストップの長さ

//コンストラクタ
Character::Character(double m_friction,long m_HP,ObjectBelongingTo m_obt,long m_mt,const std::string& m_afn)
	:CharacterBase(m_friction,m_HP, m_obt,m_afn), state(NORMAL), pause_flag(false),
	ground_flag(false), matchless(false), matchless_time(m_mt), hit_stop_cnt(0),_mode(Mode::StandAlone),_muteki(false){
	time_from_attacked = matchless_time;
}

//デストラクタ
Character::~Character(){
}

void Character::ChangeDirection(Direction dir){ reverse = dir == LEFT ? true : false; }

//初期化
int Character::Init(){ return 0; }

bool Character::ReceiveMessage(const string& mes, double p){
	if (mes == "Muteki"){
		_muteki = true;
	}
	else{
		return CharacterBase::ReceiveMessage(mes, p);
	}
	return true;
}

//更新
void Character::Update(){
	switch (_mode){
	case Mode::StandAlone:
		UpdateInStandAloneMode();
		break;
	case Mode::Control:
		UpdateInControlMode();
		break;
	default:
		break;
	}
}

void Character::UpdateInStandAloneMode(){
//	if (Key::GetInstance().GetKeyPush(KEY_INPUT_0)){ CreateEffectFuncs::CreateScoreEffect(t, r, 9999, myplanet,GetColor(255,255,0)); }

	if (pause_flag){ return; }
	if (hit_stop_cnt > 0 && state==NORMAL){
		--hit_stop_cnt; return;
	}
	//共通処理
	CommonState();
	//状態ごとの処理
	switch (state)
	{
	case ATTACKED:
		AttackedState();
		break;
	case NORMAL:
		NormalState();
		break;
	case DIE:
		DieState();
		break;
	default:
		break;
	}
	//共通更新処理
	CommonUpdateProcess();
}

void Character::UpdateInControlMode(){
	//共通更新処理
	CommonUpdateProcess();
}

//接地フラグ変更
void Character::ChangeGroundFlag(bool m_gf){
	ground_flag = m_gf;
}

//一時停止フラグ変更
void Character::ChangePauseFlag(bool v){ pause_flag = v; }

//モードセット
void Character::SetMode(Mode m){ _mode = m; }
void Character::SetMode(int m){ _mode = (Mode)m; }

//衝突オブジェクト取得
bool Character::GetCollisionObject(CollisionDatas::CharacterCollisionObject* col_obj)const{
	col_obj->circle_objects.clear();
	if (hitarea_data.size() == 0){ return false; }
	col_obj->hp = p_HP;
	col_obj->r = r;
	if (myplanet){ col_obj->t = fmod((t + 360.0), 360.0); }
	else{ col_obj->t = t; }
	col_obj->position = position;
	col_obj->col_with_friend = col_with_friend;
	col_obj->char_attribute = _attribute;
	//無敵時間内だったら無敵属性付加
	//if (time_from_attacked < matchless_time){ col_obj->match_less = true; }
	for (HitAreaData* had : hitarea_data){
		CircleObject co;
		co.radius = had->radius;
		co.position.Set(had->x, had->y);
		col_obj->grotation = GetDrawRotation();
		col_obj->reverse = reverse;
		col_obj->belongto = GetBelongingTo();
//		col_obj->hit_stop = hit_stop_cnt > 0;
		co.attribute = had->attribute;
		//ヒットストップ中だったら攻撃属性を消す
		if (hit_stop_cnt > 0){
			co.attribute &= ~Col_Atrb_Atck;
		}
		//無敵時間ないだったらダメージ属性を消す
		if (time_from_attacked < matchless_time){
			co.attribute &= ~Col_Atrb_Dmge;
		}
		co.attack_power = had->additional_param[0] * _atk_pow_magnification;
		co.matchless_time = had->additional_param[1];
		col_obj->circle_objects.push_back(co);
	}
	if (col_obj->circle_objects.size() == 0){ return false; }
	return true;
}

//デフォルトの惑星衝突処理
void Character::ProcessCollisionWithPlanet(){
	bool p_gf = ground_flag;
	ground_flag = false;
	//惑星
	for (CollisionDataWithPlanet& c : col_data_p){
		r = c.r;
		t = c.t;
		vr = 0;
		ground_flag = true;
		if (c.damage){ ReceivedAttack(c.damage); }
	}
	col_data_p.clear();
}

//デフォルトのキャラ衝突処理
void Character::ProcessCollisionWithCharacter(){
	for (CollisionDataWithCharacter& c : col_data_c){
		//敵からダメージを受けたら
		if (c.receive_damage){
			matchless_time = c.matchless_time;
			hit_stop_cnt = CHARACTER_DEFAULT_HIT_STOP_FRAME;
			ReceivedAttack(c.receive_damage);
			_col_revers_flag = true;
		}
		//敵にダメージを与えたら
		if (c.give_damage){ hit_stop_cnt = CHARACTER_DEFAULT_HIT_STOP_FRAME; }
	}
	CharacterBase::ProcessCollisionWithCharacter();
}

//状態ごとの処理関数
void Character::NormalState(){

}

void Character::AttackedState(){
	if ((action->CheckCurrentActionTag(ANIM_TAG_CHAR_ATTACKED) == true && action->CheckActionPlaying() == false) || action->CheckCurrentActionTag(ANIM_TAG_CHAR_ATTACKED) == false){
		if (p_HP <= 0){ Died(); return; }
		else{ ReturnNormal(); }
	}
}

void Character::DieState(){
	CreateEffectFuncs::CreateEnemyTerminatedEffect(t, r, myplanet);
	CharacterManager::GetInstance().DeleteObject(GetCharID());
}

//共通処理
void Character::CommonState(){
	if (time_from_attacked < matchless_time){
		visible = ((matchless_time - time_from_attacked) % 2) ? true : false;
	}
	++time_from_attacked;
}

//状態変更関数
//攻撃を受けた
void Character::ReceivedAttack(int d){
	if (_muteki)return;
	time_from_attacked = 0;
	p_HP -= d;
	state = ATTACKED;
	/*if (action->StartActionByTag(ANIM_TAG_CHAR_ATTACKED,false)){
		action->StartActionByTag(ANIM_TAG_CHAR_IDOL);
	}*/
}
//通常復帰
void Character::ReturnNormal(){
//	action->StartActionByTag(ANIM_TAG_CHAR_IDOL);
	state = NORMAL;
}
//死んだ
void Character::Died(){
	state = DIE;
}
