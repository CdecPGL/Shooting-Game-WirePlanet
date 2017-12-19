#include "EffectFactory.h"
#include"EffectCreators.h"

using namespace std;

EffectFactory::EffectFactory() :initialized(false)
{
	printf("EffectFactory�̃C���X�^���X���쐬����܂����B\n");
}


EffectFactory::~EffectFactory()
{
	printf("EffectFactory�̃C���X�^���X���j������܂����B\n");
}

EffectFactory& EffectFactory::GetInstance(){
	static EffectFactory cf;
	return cf;
}

int EffectFactory::Init(){
	//�N���G�[�^�o�^
	creators.insert(creator_map::value_type("EnemySpawnEffect", make_unique<EnemySpawnEffectCreator>()));
	creators.insert(creator_map::value_type("NormalEffect", make_unique<NormalEffectCreator>()));
	creators.insert(creator_map::value_type("ScoreEffect", make_unique<ScoreEffectCreator>()));
	//�N���G�[�^�[������
	for (auto it = creators.begin(); it != creators.end(); ++it){
		(*it).second->Init();
	}
	initialized = true;
	printf("EffectFactory������������܂����B\n");
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
