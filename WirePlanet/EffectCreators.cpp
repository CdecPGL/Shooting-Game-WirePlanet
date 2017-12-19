#include "EffectCreators.h"
#include"EnemySpawnEffect.h"
#include"ScoreEffect.h"

using namespace std;

//EnemySpawnEffectCreator

shared_ptr<Effect> EnemySpawnEffectCreator::Create(const string&){
	return make_shared<EnemySpawnEffect>();
}

//ScoreEffectCreator

shared_ptr<Effect> ScoreEffectCreator::Create(const string&){
	return make_shared<ScoreEffect>();
}

