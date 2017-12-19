#include "EffectManager.h"
#include<algorithm>
#include"EffectFactory.h"
#include"ObjectHolderTemplate_WithoutID.h"

#define CHECK_INITAILIZED_INT if(!initialized)return -1;
#define CHECK_INITAILIZED_VOID if(!initialized)return;
#define CHECK_INITAILIZED_POINTER if(!initialized)return nullptr;

using namespace std;

EffectManager::EffectManager() :initialized(false), effect_holder(make_unique<ObjectHolderTemplate_WithoutID<Effect>>())
{
	printf("EffectManagerのインスタンスが作成されました。\n");
}


EffectManager::~EffectManager()
{
	printf("EffectManagerのインスタンスが破棄されました。\n");
}

EffectManager& EffectManager::GetInstance(){
	static EffectManager em;
	return em;
}

int EffectManager::Init(){
	printf("EffectManagerが初期化されました。\n");
	initialized = true;
	return 0;
}

int EffectManager::End(){
	CHECK_INITAILIZED_INT
		printf("EffectManagerの終了処理が実行されました。\n");
	initialized = false;
	return 0;
}

int EffectManager::Reset(){
	CHECK_INITAILIZED_INT
		Clear();
	return 0;
}

int EffectManager::Process(){
	CHECK_INITAILIZED_INT
	effect_holder->process_resist_list();
	effect_holder->process_unresist_list();
	return 0;
}

void EffectManager::Clean(){
	CHECK_INITAILIZED_VOID
	effect_holder->clean();
}

void EffectManager::Clear(){
	CHECK_INITAILIZED_VOID
	effect_holder->clear();
}

void EffectManager::Update(){
	CHECK_INITAILIZED_VOID
	for (shared_ptr<Effect>& obj : effect_holder->objects){
		obj->Update();
	}
}

void EffectManager::Draw(){
	CHECK_INITAILIZED_VOID
	for (shared_ptr<Effect>& obj : effect_holder->objects){
		obj->Draw();
	}
}

shared_ptr<Effect> EffectManager::CreateObject(const std::string& kind,const string& init_id){
	CHECK_INITAILIZED_POINTER
	shared_ptr<Effect> nchara = EffectFactory::GetInstance().CreateObject(kind,init_id);
	if (nchara){
		effect_holder->resist_object(nchara);
	}
	else{
		printf("EffectManager:キャラクタの作成に失敗しました。:kind=%s init_id=%s\n",kind.c_str(),init_id.c_str());
	}
	return nchara;
}

int EffectManager::DeleteObject(weak_ptr<Effect> chara){
	CHECK_INITAILIZED_INT
	return effect_holder->unresist_object(chara);
}
int EffectManager::DeleteObject(Effect* chara){
	CHECK_INITAILIZED_INT
	return effect_holder->unresist_object(chara);
}

int EffectManager::GetObjectCount()const{ return effect_holder->size(); }

