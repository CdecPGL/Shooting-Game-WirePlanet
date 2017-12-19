#include "CharacterFactory.h"
#include"CharacterCreators.h"

using namespace std;

CharacterFactory::CharacterFactory() :initialized(false)
{
	printf("CharacterFactoryのインスタンスが作成されました。\n");
}


CharacterFactory::~CharacterFactory()
{
	printf("CharacterFactoryのインスタンスが破棄されました。\n");
}

CharacterFactory& CharacterFactory::GetInstance(){
	static CharacterFactory cf;
	return cf;
}

int CharacterFactory::Init(){
	//クリエータ登録
	creators.insert(creator_map::value_type(PLAYER, make_unique<PlayerCreator>()));
	creators.insert(creator_map::value_type(ENEMY, make_unique<EnemyCreator>()));
	creators.insert(creator_map::value_type(BULLET, make_unique<BulletCreator>()));
	creators.insert(creator_map::value_type(ITEM, make_unique<ItemCreator>()));
	//クリエータ初期化
	for (auto it = creators.begin(); it != creators.end(); ++it){
		(*it).second->Init();
	}
	initialized = true;
	printf("CharacterFactoryが初期化されました。\n");
	return 0;
}

shared_ptr<CharacterBase> CharacterFactory::CreateObject(CharaAtrribtute ca, const string& init_id){
	auto it = creators.find(ca);
	if (it == creators.end()){ return nullptr; }
	return (*it).second->Create(init_id);
}

bool CharacterFactory::CheckInitDataExistence(CharaAtrribtute ca,const string& init_id)const{
	auto it = creators.find(ca);
	if (it == creators.end()){ return false; }
	return (*it).second->CheckInitIDExistence(init_id);
}

void CharacterFactory::ClearInitDataList(){
	creators.clear();
}
