#include "CharacterFactory.h"
#include"CharacterCreators.h"

using namespace std;

CharacterFactory::CharacterFactory() :initialized(false)
{
	printf("CharacterFactory�̃C���X�^���X���쐬����܂����B\n");
}


CharacterFactory::~CharacterFactory()
{
	printf("CharacterFactory�̃C���X�^���X���j������܂����B\n");
}

CharacterFactory& CharacterFactory::GetInstance(){
	static CharacterFactory cf;
	return cf;
}

int CharacterFactory::Init(){
	//�N���G�[�^�o�^
	creators.insert(creator_map::value_type(PLAYER, make_unique<PlayerCreator>()));
	creators.insert(creator_map::value_type(ENEMY, make_unique<EnemyCreator>()));
	creators.insert(creator_map::value_type(BULLET, make_unique<BulletCreator>()));
	creators.insert(creator_map::value_type(ITEM, make_unique<ItemCreator>()));
	//�N���G�[�^������
	for (auto it = creators.begin(); it != creators.end(); ++it){
		(*it).second->Init();
	}
	initialized = true;
	printf("CharacterFactory������������܂����B\n");
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
