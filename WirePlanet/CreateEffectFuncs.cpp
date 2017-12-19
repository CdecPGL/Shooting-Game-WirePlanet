#include "CreateEffectFuncs.h"
#include"EffectManager.h"
#include"EnemySpawnEffect.h"
#include"SoundManager.h"
#include"CharacterController.h"
#include"ScoreEffect.h"

using namespace std;

shared_ptr<Effect> CreateEffectFuncs::CreateEnemySpawnEffect(double t, double r/*, shared_ptr<Planet> p,shared_ptr<Character> c*/,const string& eiid, int w){
	shared_ptr<EnemySpawnEffect> ef = static_pointer_cast<EnemySpawnEffect>(EffectManager::GetInstance().CreateObject("EnemySpawnEffect"));
	if (ef == nullptr){ return nullptr; }
	ef->SetBelongingPlanet(CharacterController::GetInstance().GetBelongingPlanet());
	ef->SetTR(t, r);
	ef->SetWidth(w);
	/*ef->SetTarget(c);*/
	ef->SetSpawnEnemyInitID(eiid);
	if (ef->Init()){
		EffectManager::GetInstance().DeleteObject(ef);
		return nullptr;
	}
	SoundManager::GetInstance().PlaySoundByID("EnemySpawn0");
	return ef;
}

shared_ptr<Effect> CreateEffectFuncs::CreateScoreEffect(double t, double r, int d_s,shared_ptr<Planet> b_p,int col){
	shared_ptr<ScoreEffect> ef = static_pointer_cast<ScoreEffect>(EffectManager::GetInstance().CreateObject("ScoreEffect"));
	ef->SetBelongingPlanet(b_p);
	ef->SetTR(t, r);
	ef->SetDrawScore(d_s);
	ef->SetDrawColor(col);
	if (ef->Init()){ 
		EffectManager::GetInstance().DeleteObject(ef);
		return nullptr; 
	}
	return ef;
}
shared_ptr<Effect> CreateEffectFuncs::CreateScoreEffect(double t, double r, int d_s, shared_ptr<Planet> b_p){
	return CreateScoreEffect(t,r,d_s,b_p,GetColor(255,255,255));
}

shared_ptr<Effect> CreateEffectFuncs::CreateEnemyTerminatedEffect(double t_deg, double r, shared_ptr<Planet> p){
	shared_ptr<Effect> ef = EffectManager::GetInstance().CreateObject("NormalEffect","Die1");
	if (ef == nullptr){ return nullptr; }
	ef->SetBelongingPlanet(p);
	ef->SetTR(t_deg, r);
	if (ef->Init()){
		EffectManager::GetInstance().DeleteObject(ef);
		return nullptr; 
	}return ef;
}

shared_ptr<Effect> CreateEffectFuncs::CreateNormalEffect(const string& name,double t_deg, double r, shared_ptr<Planet> p){
	shared_ptr<Effect> ef = EffectManager::GetInstance().CreateObject("NormalEffect", name);
	if (ef == nullptr){ return nullptr; }
	ef->SetBelongingPlanet(p);
	ef->SetTR(t_deg, r);
	if (ef->Init()){
		EffectManager::GetInstance().DeleteObject(ef);
		return nullptr;
	}return ef;
}