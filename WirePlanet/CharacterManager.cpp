#include "CharacterManager.h"
#include"CharacterFactory.h"
#include"Planet.h"
#include"boost/utility.hpp"
#include"CollisionDetecter.h"
#include"ObjectHolderTemplate_WithID.h"
#include"MatrixMap.h"
#include"LaserManager.h"

#ifdef DEBUG_DRAW
#include<ctime>
#include"DebugWindow.h"
#endif

#define CHECK_INITAILIZED_INT if(!initialized)return -1;
#define CHECK_INITAILIZED_VOID if(!initialized)return;
#define CHECK_INITAILIZED_POINTER if(!initialized)return nullptr;
#define CHECK_INITAILIZED_BOOL if(!initialized)return false;

using namespace std;

const int CHARACTER_DISPOSE_DISTANCE_FROM_PLANET_CENTER(800);

CharacterManager::CharacterManager() :initialized(false), chara_holder(make_unique<ObjectHolderTemplate_WithID<int, CharacterBase>>()), id_counter(0), _collision_map(make_unique<MatrixMap<CharaAtrribtute, bool>>()), _col_det_cnt(0)
{
	printf("CharacterManagerのインスタンスが作成されました。\n");
}


CharacterManager::~CharacterManager()
{
	printf("CharacterManagerのインスタンスが破棄されました。\n");
}

CharacterManager& CharacterManager::GetInstance(){
	static CharacterManager cm;
	return cm;
}

int CharacterManager::Init(){
	SetCollisionMap();
	printf("CharacterManagerが初期化されました。\n");
	initialized = true;
	return 0;
}

int CharacterManager::End(){
	CHECK_INITAILIZED_INT
	printf("CharacterManagerの終了処理が実行されました。\n");
	initialized = false;
	return 0;
}

int CharacterManager::Reset(){
	CHECK_INITAILIZED_INT
	Clear();
	players.clear();
	enemies.clear();
	return 0;
}

int CharacterManager::Process(){
	CHECK_INITAILIZED_INT
	chara_holder->process_resist_list();
	chara_holder->process_unresist_list();
	return 0;
}

void CharacterManager::Clean(){
	CHECK_INITAILIZED_VOID
	chara_holder->clean();
}

void CharacterManager::Clear(){
	CHECK_INITAILIZED_VOID
	chara_holder->clear();
	id_counter = 0;
}

void CharacterManager::Update(){
	CHECK_INITAILIZED_VOID
	chara_holder->do_all(&CharacterBase::Update);
}

void CharacterManager::Draw(){
	CHECK_INITAILIZED_VOID
	chara_holder->do_all(&CharacterBase::Draw);
}

shared_ptr<CharacterBase> CharacterManager::CreateObject(CharaAtrribtute ca, const string& init_id)const{
	CHECK_INITAILIZED_POINTER
//		printf("|cm1|");
		shared_ptr<CharacterBase> nchara = CharacterFactory::GetInstance().CreateObject(ca, init_id);
//		printf("|cm2|");
	if (nchara==nullptr){	
		printf("CharacterManager:キャラクタの作成に失敗しました。:ca=%d,init_id=%s\n", ca, init_id.c_str());
		return nullptr;
	}
//	printf("|cm3|");
	if (nchara->Init()){
		printf("CharacterManager:キャラクタの初期化に失敗しました。:ca=%d,init_id=%s\n", ca, init_id.c_str());
		return nullptr;
	}
//	printf("|cm4|");
	return nchara;
}
shared_ptr<CharacterBase> CharacterManager::CreateObject(CharaAtrribtute ca)const{
	return CreateObject(ca, "NULL");
}

shared_ptr<CharacterBase> CharacterManager::CreateAndResistObject(CharaAtrribtute ca, const string& init_id){
	CHECK_INITAILIZED_POINTER
	shared_ptr<CharacterBase> nchara = CreateObject(ca, init_id);
	if (nchara == nullptr){ return nullptr; }
	if (nchara->Init()){
		printf("CharacterManager:キャラクタの初期化に失敗しました。:ca=%d,init_id=%s\n", ca, init_id.c_str());
		return nullptr;
	}
	if (chara_holder->resist_object(nchara,id_counter)){ return nullptr; }
	nchara->SetCharID(id_counter);
	++id_counter;
	return nchara;
}
shared_ptr<CharacterBase> CharacterManager::CreateAndResistObject(CharaAtrribtute ca){
	return CreateAndResistObject(ca, "NULL");
}

int CharacterManager::ResistObject(shared_ptr<CharacterBase> chara){
	int id = chara->GetCharID();
	if (id < 0){ chara->SetCharID(id_counter); id = id_counter++; }
	return chara_holder->resist_object(chara, id);
}

int CharacterManager::DeleteObject(int id){
	CHECK_INITAILIZED_INT
	return chara_holder->unresist_object(id);
}

bool CharacterManager::CheckObjectResisted(int id)const{
	CHECK_INITAILIZED_BOOL
	return chara_holder->is_exist(id);
}

int CharacterManager::GetObjectCount()const{ return chara_holder->size(); }

shared_ptr<CharacterBase> CharacterManager::GetSharedPointer(int id)const{
	return chara_holder->find_object(id);
}

int CharacterManager::GetCollisionDetectedCount()const{ return _col_det_cnt; }

void CharacterManager::SetCollisionMap(){
	_collision_map->SetMap(PLAYER, PLAYER, false);
	_collision_map->SetMap(PLAYER, ENEMY, true);
	_collision_map->SetMap(PLAYER, BULLET, true);
	_collision_map->SetMap(PLAYER, ITEM, true);
	_collision_map->SetMap(ENEMY, ENEMY, false);
	_collision_map->SetMap(ENEMY, BULLET, true);
	_collision_map->SetMap(ENEMY, ITEM, false);
	_collision_map->SetMap(BULLET, BULLET, false);
	_collision_map->SetMap(BULLET, ITEM, false);
	_collision_map->SetMap(ITEM, ITEM, false);
}

void CharacterManager::CollisionDetect(){
#ifdef DEBUG_DRAW
	time_t time = clock();
#endif
	using namespace CollisionFuncs;
	//レーザー衝突オブジェクト取得
	vector<CollisionDatas::LaserCollisionObject> lsr_col_obj = move(LaserManager::GetInstance().GetLaserCollionObjects());
	_col_det_cnt = 0;
	for (auto it0 = chara_holder->objects.begin(); it0 != chara_holder->objects.end(); ++it0){
		//キャラが惑星と指定以上の距離離れたら破棄する
		if ((*it0).second->GetR() > CHARACTER_DISPOSE_DISTANCE_FROM_PLANET_CENTER){
			DeleteObject((*it0).second->GetCharID());
			continue;
		}
		CollisionDatas::CharacterCollisionObject c0co;
		shared_ptr<Planet> p = (*it0).second->GetBelongingPlanet();
		if ((*it0).second->GetCollisionObject(&c0co) == false){ continue; } //衝突オブジェクトがなかったらスキップ
		//惑星とキャラ
		if (p){
			//惑星と衝突していたら
			CollisionDataWithPlanet cdwp;
			if (CollisionDetectsCharsPlanet(c0co, p, &cdwp)){
				(*it0).second->AddCollisionData(cdwp);
			}
		}
		//キャラ同士
		if ((*it0).second->GetBelongingTo() == ObjectBelongingTo::None){ continue; }
		for (auto it1 = boost::next(it0); it1 != chara_holder->objects.end(); ++it1){
			//衝突マップ参照
			if (_collision_map->CheckMap((*it0).second->GetCharaAttribute(), (*it1).second->GetCharaAttribute()) == false){ continue; }
			//敵同士、味方同士だったら判定しない(味方との判定が有効だったらする)
//			if ((*it0).second->GetHostilityFlag() == (*it1).second->GetHostilityFlag() && (*it0).second->GetCollisionWithFriendFlag() == false && (*it1).second->GetCollisionWithFriendFlag() == false){ continue; }

			if ((*it1).second->GetBelongingTo() == ObjectBelongingTo::None){ continue; }
			if ((*it0).second->GetBelongingTo() == (*it1).second->GetBelongingTo()){ continue; }
			CollisionDatas::CharacterCollisionObject c1co;
			if ((*it1).second->GetCollisionObject(&c1co) == false){ continue; }
			CollisionDataWithCharacter cdwc0, cdwc1;
			if (CollisionDetectsCharsChar(c0co,&cdwc0 , c1co,&cdwc1 )){
				cdwc0.col_chara_id = (*it1).second->GetCharID();
				(*it0).second->AddCollisionData(cdwc0);
				cdwc1.col_chara_id = (*it0).second->GetCharID();
				(*it1).second->AddCollisionData(cdwc1);
			}
			++_col_det_cnt;
		}
		//レーザーとキャラ
		for (auto& lco : lsr_col_obj){
			if ((c0co.belongto == lco.belongto && c0co.belongto != ObjectBelongingTo::Neutral && lco.belongto != ObjectBelongingTo::Neutral) || (c0co.belongto == ObjectBelongingTo::None || lco.belongto == ObjectBelongingTo::None)){ continue; }
			CollisionDataWithCharacter cdwc;
			if (CollisionDetectsCharsLaser(c0co, lco, &cdwc)){
				cdwc.col_chara_id = -1;
				cdwc.char_attribute = CharaAtrribtute::BULLET;
				(*it0).second->AddCollisionData(cdwc);
			}
		}
	}
	//衝突処理を行う
	for (unordered_map<int, shared_ptr<CharacterBase>>::value_type& c : chara_holder->objects){
		c.second->ProcessCollision();
	}
#ifdef DEBUG_DRAW
	time = (int)(clock() - time);
	DebugWindow::GetInstance().AddExecutionTime("ChrMgr::ColDet", time / (CLOCKS_PER_SEC / 1000));
#endif
}
