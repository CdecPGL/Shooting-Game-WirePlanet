#include "CharacterBase.h"
#include"GraphicController2D.h"
#include"Key.h"
#include"Planet.h"
#include"CharacterManager.h"

using namespace std;

CharacterBase::CharacterBase(double m_friction,int m_hp, ObjectBelongingTo m_obt, const std::string& m_afn)
	:Object(1.0, m_friction, m_obt), _current_ghandle(-1), HP(m_hp), p_HP(m_hp),
	reverse(false), action(make_shared<Action>(ActionDataAdmin::GetInstance().GetActionData(m_afn)))
	, char_ID(-1), id_settable(true), col_with_friend(false),_atk_pow_magnification(1.0),_bright_flag(false),_revers_draw_gh(-1),_col_revers_flag(false){
}


CharacterBase::~CharacterBase()
{
	if (_revers_draw_gh >= 0){
		DeleteGraph(_revers_draw_gh);
	}
}

void CharacterBase::Draw(){
	double d_angle = GetDrawRotation();
	if (visible){
		gc2d::DrawRotaGraphF((float)position.x, (float)position.y, 1.0, d_angle / 180 * DX_PI, _current_ghandle, 1, reverse);
		if (_bright_flag){
			SetDrawBlendMode(DX_BLENDMODE_ADD_X4, 255);
			gc2d::DrawRotaGraphF((float)position.x, (float)position.y, 1.0, d_angle / 180 * DX_PI, _current_ghandle, 1, reverse);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			_bright_flag = false;
		}
		if (_col_revers_flag){
			int w, h;
			GetGraphSize(_current_ghandle, &w, &h);
			if (_revers_draw_gh < 0){
				_revers_draw_gh = MakeGraph(w, h, true);
			}
			GraphFilterBlt(_current_ghandle, _revers_draw_gh, DX_GRAPH_FILTER_INVERT);
			gc2d::DrawRotaGraphF((float)position.x, (float)position.y, 1.0, d_angle / 180 * DX_PI, _revers_draw_gh, 1, reverse);
			_col_revers_flag = false;
		}
	}

	//@デバッグ
	/*static bool draw_debug;
	if (Key::GetInstance().GetKeyPush(KEY_INPUT_2)){ draw_debug = !draw_debug; }
	if (draw_debug){
		DrawHitArea();
	}*/
}

void CharacterBase::DrawHitArea(){
	double d_angle = GetDrawRotation();
	int rev = reverse ? -1 : 1; //画像が反転しているときは衝突判定の相対x座標も反転させるようにする
	for (auto hit = hitarea_data.begin(); hit != hitarea_data.end(); hit++){
		int r(0), g(0), b(0);
		if ((*hit)->attribute & Col_Atrb_Atck){ r = 255; }
		if ((*hit)->attribute & Col_Atrb_Trrn){ g = 255; }
		if ((*hit)->attribute & Col_Atrb_Dmge){ b = 255; }
		int col = GetColor(r, g, b);
		gc2d::DrawCircleF((int)(position.x - rev * (*hit)->x * sin(d_angle / 180 * DX_PI - DX_PI / 2) - (*hit)->y * cos(d_angle / 180 * DX_PI - DX_PI / 2)), (int)(position.y + rev * (*hit)->x * cos(d_angle / 180 * DX_PI - DX_PI / 2) - (*hit)->y * sin(d_angle / 180.0 * DX_PI - DX_PI / 2)), (*hit)->radius, col, 0);
	}
}

//衝突オブジェクト取得
bool CharacterBase::GetCollisionObject(CollisionDatas::CharacterCollisionObject* col_obj)const{
	col_obj->circle_objects.clear();
	if (hitarea_data.size() == 0){ return false; }
	col_obj->hp = p_HP;
	col_obj->r = r;
	if (myplanet){ col_obj->t = fmod((t + 360.0), 360.0); }
	else{col_obj->t = t;}
	col_obj->position = position;
	col_obj->col_with_friend = col_with_friend;
	col_obj->char_attribute = _attribute;
	for (HitAreaData* had : hitarea_data){
		CircleObject co;
		co.radius = had->radius;
		co.position.Set(had->x, had->y);
		col_obj->grotation = GetDrawRotation();
		col_obj->reverse = reverse;
		col_obj->belongto = GetBelongingTo();
		co.attribute = had->attribute;
		co.attack_power = (int)(had->additional_param[0] * _atk_pow_magnification);
		co.matchless_time = had->additional_param[1];
		col_obj->circle_objects.push_back(co);
	}
	if (col_obj->circle_objects.size() == 0){ return false; }
	return true;
}

//摩擦処理
void CharacterBase::Friction(){
	double g_friction = 1.0;
	if (myplanet){
		g_friction = myplanet->GetFriction();
	}
	if (vh != 0){
		vh += -(vh / fabs(vh)) * friction * g_friction * 0.1;
		if (fabs(vh) <= friction * g_friction * 0.1 / 2){ vh = 0; }
	}
}

void CharacterBase::ProcessCollision(){
	ProcessCollisionWithCharacter();
	ProcessCollisionWithPlanet();
}

void CharacterBase::ProcessCollisionWithCharacter(){
	col_data_c.clear();
}

void CharacterBase::ProcessCollisionWithPlanet(){
	col_data_p.clear();
}

//void CharacterBase::ResistToManager(){
//	id_settable = true;
//	int id = CharacterManager::GetInstance().ResistObject(this);
//	SetCharID(id);
//}
//
//void CharacterBase::UnresistFromManager(){
//	CharacterManager::GetInstance().DeleteObject(GetCharID());
//}

//状態のコピー
void CharacterBase::CopyStatus(const CharacterBase& c){
	t = c.t;
	r = c.r;
	ConvertPTW();
	reverse = c.reverse;
}

int CharacterBase::GetGHandle()const{ return _current_ghandle; };
bool CharacterBase::GetReverseFlag()const{ return reverse; }
bool CharacterBase::GetCollisionWithFriendFlag()const{ return col_with_friend; }
int CharacterBase::GetHP()const{ return p_HP; };
int CharacterBase::GetMaxHP()const{ return HP; }
CharaAtrribtute CharacterBase::GetCharaAttribute()const{ return _attribute; }

void CharacterBase::SetAttackPowerMagnification(double apm){ _atk_pow_magnification = apm; }
void CharacterBase::SetReverse(bool rf){ reverse = rf; }

int CharacterBase::StartAction(const string& tag){
	return action->StartActionByTag(tag.c_str());
}

int CharacterBase::GetActionSize()const{
	return action->GetGraphSize();
}

//ID
void CharacterBase::SetCharID(int id){
	if (id_settable == false){ return; }
	char_ID = id;
	id_settable = false;
}
int CharacterBase::GetCharID()const{ return char_ID; }

void CharacterBase::UpdateHitAreaData(){
	action->SetCurrentHitAreaToVector(&hitarea_data);
}

void CharacterBase::UpdateCurrentGHandle(){
	_current_ghandle = action->GetCurrentGraph();
}

void CharacterBase::CommonUpdateProcess(){
	//速度を適用
	ApplyVelocity();
	//xy座標を求める
	ConvertPTW();
	//アクション更新
	action->Update();
	//画像更新
	UpdateCurrentGHandle();
	//当たり判定更新
	UpdateHitAreaData();
	//自動移動処理
	ProcessAutoMove();
}
