#include "Attack.h"
#include"EffectManager.h"
#include"CharacterManager.h"

using namespace std;

Attack::Attack()
{
}


Attack::~Attack()
{
}

int Attack::Init(){
	return 0;
}

void Attack::Update(){

}

int Attack::CreateEffect(int id, const string& init_id){
	shared_ptr<Object> ef = EffectManager::GetInstance().CreateObject("NormalEffect", init_id);
	if (ef == nullptr){ return -1; }
	objects.insert(ObjectList::value_type(id, ef));
	return 0;
}

int Attack::CreateBullet(int id, const string& init_id){
	shared_ptr<Object> ef = CharacterManager::GetInstance().CreateAndResistObject(BULLET, init_id);
	if (ef == nullptr){ return -1; }
	objects.insert(ObjectList::value_type(id, ef));
	return 0;
}
