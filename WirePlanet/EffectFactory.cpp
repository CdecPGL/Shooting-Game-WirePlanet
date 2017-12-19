#include "EffectFactory.h"
#include"EffectCreators.h"

using namespace std;

EffectFactory::EffectFactory() :initialized(false)
{
	printf("EffectFactoryのインスタンスが作成されました。\n");
}


EffectFactory::~EffectFactory()
{
	printf("EffectFactoryのインスタンスが破棄されました。\n");
}

EffectFactory& EffectFactory::GetInstance(){
	static EffectFactory cf;
	return cf;
}

int EffectFactory::Init(){
	//クリエータ登録
	creators.insert(creator_map::value_type("EnemySpawnEffect", make_unique<EnemySpawnEffectCreator>()));
	creators.insert(creator_map::value_type("NormalEffect", make_unique<NormalEffectCreator>()));
	creators.insert(creator_map::value_type("ScoreEffect", make_unique<ScoreEffectCreator>()));
	//クリエーター初期化
	for (auto it = creators.begin(); it != creators.end(); ++it){
		(*it).second->Init();
	}
	initialized = true;
	printf("EffectFactoryが初期化されました。\n");
	return 0;
}

shared_ptr<Effect> EffectFactory::CreateObject(const string& kind,const string& init_id){
	auto it = creators.find(kind);
	if (it == creators.end()){ return nullptr; }
	return (*it).second->Create(init_id);
}

bool EffectFactory::CheckInitDataExistence(const string& kind,const string& init_id)const{
	auto it = creators.find(kind);
	return it == creators.end() ? false : (*it).second->CheckInitIDExistence(init_id);
}

void EffectFactory::ClearInitDataList(){
	creators.clear();
}
