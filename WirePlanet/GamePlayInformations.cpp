#include "GamePlayInformations.h"
//#include"SaveData.h"


GamePlayInformations::GamePlayInformations() :_points(new unsigned int(0)), _hight_score(new unsigned int(0))
, _play_frame(new unsigned long long(0)), _play_second(new unsigned long long(0)), _play_count(new unsigned int(0))
, _first_play_year(new unsigned int(0)), _first_play_month(new unsigned int(0)), _first_play_day(new unsigned int(0))
, _first_play_hour(new unsigned int(0)), _first_play_minuits(new unsigned int(0)), _first_play_second(new unsigned int(0))
, _defeated_enemy(new unsigned int(0)), _shooted_bullet_num(new unsigned int(0)), _game_over_count(new unsigned int(0))
, _continue_count(new unsigned int(0)), _normal_clear_count(new unsigned int(0)), _all_clear_count(new unsigned int(0))
, _cheat_count(new unsigned int(0)), _error_count(new unsigned int(0)), _made_clone_num(0u)
{
}


GamePlayInformations::~GamePlayInformations()
{
}

GamePlayInformations& GamePlayInformations::GetInstance(){
	static GamePlayInformations gpi;
	return gpi;
}

//int GamePlayInformations::SetSaveData(){
//	SaveData& sd = SaveData::GetInstance();
//	sd.SetUInteger("GamePoint",_points.get());
//	sd.SetUInteger("HightScore", _hight_score.get());
//	sd.SetUInteger("PlayCount", _play_count.get());
//	sd.SetUInteger("FirstPlayYear", _first_play_year.get());
//	sd.SetUInteger("FirstPlayMonth", _first_play_month.get());
//	sd.SetUInteger("FirstPlayDay", _first_play_day.get());
//	sd.SetUInteger("FirstPlayhour", _first_play_hour.get());
//	sd.SetUInteger("FirstPlayMinuits", _first_play_minuits.get());
//	sd.SetUInteger("FirstPlaySecond", _first_play_second.get());
//	sd.SetUInteger("DefeatedEnemy", _defeated_enemy.get());
//	sd.SetUInteger("ShootedBulletNum", _shooted_bullet_num.get());
//	sd.SetUInteger("GameOverCount", _game_over_count.get());
//	sd.SetUInteger("ContinueCount", _continue_count.get());
//	sd.SetUInteger("NormalClearCount", _normal_clear_count.get());
//	sd.SetUInteger("AllClearCount", _all_clear_count.get());
//	sd.SetUInteger("CheatCount", _cheat_count.get());
//
//	//起動フレーブ、秒は64bitなので上下32bitｎ分けて保存する
//	unsigned int h, l;
//	unsigned long long pf(_play_frame.get()), ps(_play_second.get());
//	h = (unsigned int)((pf >> 32) & 0xFFFFFFFF);
//	l = (unsigned int)(pf & 0xFFFFFFFF);
//	sd.SetUInteger("PlayFrameH", h);
//	sd.SetUInteger("PlayFrameL", l);
//	h = (unsigned int)((ps >> 32) & 0xFFFFFFFF);
//	l = (unsigned int)(ps & 0xFFFFFFFF);
//	sd.SetUInteger("PlaySecondH", h);
//	sd.SetUInteger("PlaySecondL", l);
//	return 0;
//}
//
//int GamePlayInformations::GetSaveData(){
//	SaveData& sd = SaveData::GetInstance();
//	_points.set(sd.GetUInteger("GamePoint"));
//	_hight_score.set(sd.GetUInteger("HightScore"));
//	_play_count.set(sd.GetUInteger("PlayCount"));
//	_first_play_year.set(sd.GetUInteger("FirstPlayYear"));
//	_first_play_month.set(sd.GetUInteger("FirstPlayMonth"));
//	_first_play_day.set(sd.GetUInteger("FirstPlayDay"));
//	_first_play_hour.set(sd.GetUInteger("FirstPlayhour"));
//	_first_play_minuits.set(sd.GetUInteger("FirstPlayMinuits"));
//	_first_play_second.set(sd.GetUInteger("FirstPlaySecond"));
//	_defeated_enemy.set(sd.GetUInteger("DefeatedEnemy"));
//	_shooted_bullet_num.set(sd.GetUInteger("ShootedBulletNum"));
//	_game_over_count.set(sd.GetUInteger("GameOverCount"));
//	_continue_count.set(sd.GetUInteger("ContinueCount"));
//	_normal_clear_count.set(sd.GetUInteger("NormalClearCount"));
//	_all_clear_count.set(sd.GetUInteger("AllClearCount"));
//	_cheat_count.set(sd.GetUInteger("CheatCount"));
//
//	//起動フレーブ、秒は64bitなので上下32bitｎ分けて保存する
//	unsigned int h, l;
//	unsigned long long pf(_play_frame.get()), ps(_play_second.get());
//	h = (unsigned int)((pf >> 32) & 0xFFFFFFFF);
//	l = (unsigned int)(pf & 0xFFFFFFFF);
//	sd.SetUInteger("PlayFrameH", h);
//	sd.SetUInteger("PlayFrameL", l);
//	h = (unsigned int)((ps >> 32) & 0xFFFFFFFF);
//	l = (unsigned int)(ps & 0xFFFFFFFF);
//	sd.SetUInteger("PlaySecondH", h);
//	sd.SetUInteger("PlaySecondL", l);
//}
