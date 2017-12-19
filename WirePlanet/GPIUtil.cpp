#include "GPIUtil.h"
#include"GamePlayInformations.h"

void GPIUtil::AddGamePoint(unsigned int p){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	gpi._points.instance() += p;
	gpi._points.synchronize();
}

void GPIUtil::UseGamePoint(unsigned int p){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	if (gpi._points.get() < p){ gpi._points.set(0); }
	else{
		gpi._points.instance() = p;
		gpi._points.synchronize();
	}
}

void GPIUtil::SetHighScore(unsigned int hs){
	GamePlayInformations::GetInstance()._hight_score.set(hs);
}

void GPIUtil::AddPlayTime(unsigned long long pf, unsigned long long ps){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	gpi._play_frame.instance() += pf;
	gpi._play_frame.synchronize();
	gpi._play_second.instance() += ps;
	gpi._play_second.synchronize();
}

void GPIUtil::AddPlayCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._play_count.instance();
	gpi._play_count.synchronize();
}

void GPIUtil::SetFirstPlayDate(unsigned int y, unsigned int m, unsigned int d, unsigned int h, unsigned int min, unsigned int s){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	gpi._first_play_year.set(y);
	gpi._first_play_month.set(m);
	gpi._first_play_day.set(d);
	gpi._first_play_hour.set(h);
	gpi._first_play_minuits.set(min);
	gpi._first_play_second.set(s);
}

void GPIUtil::AddDefeatedEnemyCount(unsigned int num){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	gpi._defeated_enemy.instance() += num;
	gpi._defeated_enemy.synchronize();
}

void GPIUtil::AddShootedBulletCount(unsigned int num){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	gpi._shooted_bullet_num.instance() += num;
	gpi._shooted_bullet_num.synchronize();
}

void GPIUtil::AddMadeCloneCount(unsigned int num){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	gpi._made_clone_num.instance() += num;
	gpi._made_clone_num.synchronize();
}

void GPIUtil::AddGameOverCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._game_over_count.instance();
	gpi._game_over_count.synchronize();
}

void GPIUtil::AddContinueCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._continue_count.instance();
	gpi._continue_count.synchronize();
}

void GPIUtil::AddNormalClearCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._normal_clear_count.instance();
	gpi._normal_clear_count.synchronize();
}

void GPIUtil::AddAllClearCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._all_clear_count.instance();
	gpi._all_clear_count.synchronize();
}

void GPIUtil::AddCheatCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._cheat_count.instance();
	gpi._cheat_count.synchronize();
}

void GPIUtil::AddErrorCount(){
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	++gpi._error_count.instance();
	gpi._error_count.synchronize();
}

unsigned int GPIUtil::GetGamePoint(){
	return GamePlayInformations::GetInstance()._points.get();
}

unsigned int GPIUtil::GetHightScore(){
	return GamePlayInformations::GetInstance()._hight_score.get();
}

unsigned long long GPIUtil::GetPlayFrame(){
	return GamePlayInformations::GetInstance()._play_frame.get();
}

unsigned long long GPIUtil::GetPlaySecond(){
	return GamePlayInformations::GetInstance()._play_second.get();
}

unsigned int GPIUtil::GetPlayCount(){
	return GamePlayInformations::GetInstance()._play_count.get();
}

unsigned int GPIUtil::GetDefeatedEnemy(){
	return GamePlayInformations::GetInstance()._defeated_enemy.get();
}

unsigned int GPIUtil::GetShootedBullet(){
	return GamePlayInformations::GetInstance()._shooted_bullet_num.get();
}

unsigned int GPIUtil::GetMadeClone(){
	return GamePlayInformations::GetInstance()._made_clone_num.get();
}

unsigned int GPIUtil::GetGameOverCount(){
	return GamePlayInformations::GetInstance()._game_over_count.get();
}

unsigned int GPIUtil::GetContinueCount(){
	return GamePlayInformations::GetInstance()._continue_count.get();
}

unsigned int GPIUtil::GetNormalClearCount(){
	return GamePlayInformations::GetInstance()._normal_clear_count.get();
}

unsigned int GPIUtil::GetAllClearCount(){
	return GamePlayInformations::GetInstance()._all_clear_count.get();
}

unsigned int GPIUtil::GetCheatCount(){
	return GamePlayInformations::GetInstance()._cheat_count.get();
}

unsigned int GPIUtil::GetErrorCount(){
	return GamePlayInformations::GetInstance()._error_count.get();
}

GPIUtil::DateTime GPIUtil::GetFirstPlayDateTime(){
	DateTime dt;
	GamePlayInformations& gpi = GamePlayInformations::GetInstance();
	dt.year = gpi._first_play_year.get();
	dt.month = gpi._first_play_month.get();
	dt.day = gpi._first_play_day.get();
	dt.hour = gpi._first_play_hour.get();
	dt.minuits = gpi._first_play_minuits.get();
	dt.second = gpi._first_play_second.get();
	return dt;
}
