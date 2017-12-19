#include "GameScreenEffect.h"
#include"Vector2D.h"
#include"GraphicController2D.h"
#include"GraphicManager.h"
#include"SoundManager.h"

using namespace std;

GameScreenEffect::GameScreenEffect() : _current_effect_info({ "NULL", -1, "NULL" }), inc_cnt(0)
{
}

GameScreenEffect::~GameScreenEffect()
{
}

int GameScreenEffect::Init(){
	_id_num_map.insert(make_pair("Alert", 0));
	_id_num_map.insert(make_pair("FullScreenGraph", 1));
	_draw_area_size = gc2d::GetDrawAreaSize();
	return 0;
}

void GameScreenEffect::Update(){
	switch (_current_effect_info.num){
	case 0:
		++inc_cnt;
		break;
	case 1:
		if (inc_cnt == 0){
			if (GraphicManager::GetInstance().IsGraphExist(_current_effect_info.param) == false){
				printf("GameScreenEffect::Update FullScreenGraph->éwíËÇ≥ÇÍÇΩâÊëúÇ™ë∂ç›ÇµÇ‹ÇπÇÒÅB(%s)\n", _current_effect_info.param.c_str());
				StopEffect();
			}
			else{
				inc_cnt = 1;
			}
		}
		break;
	default:
		break;
	}
}

void GameScreenEffect::Draw(){
	Vector2D<int> draw_area_size = gc2d::GetDrawAreaSize();
	switch (_current_effect_info.num){
	case 0:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)((1 - cos(inc_cnt / 30.0)*150.0)));
		DrawFillBox(0, 0, draw_area_size.x, draw_area_size.y, GetColor(255, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case 1:
		DrawExtendGraph(0, 0, _draw_area_size.x, _draw_area_size.y, GraphicManager::GetInstance().GetGraphHandle(_current_effect_info.param), true);
		break;
	default:
		break;
	}
}

int GameScreenEffect::StartEffect(const string& eid, const string& param){
	if (eid == "Alert"){ SoundManager::GetInstance().PlaySoundByID("Warning"); }
	auto it = _id_num_map.find(eid);
	if (it == _id_num_map.end()){ return -1; }
	_current_effect_info = { eid, (*it).second, param };
	inc_cnt = 0;
	return 0;
}

void GameScreenEffect::StopEffect(){
	_current_effect_info = { "NULL", -1, "NULL" };
}

bool GameScreenEffect::IsEffectPlaying()const{
	return _current_effect_info.num < 0 ? false : true;
}

string GameScreenEffect::GetCurrentEffectName()const{
	return _current_effect_info.name;
}