#include "CharacterController.h"
#include<algorithm>

const int ENEMY_CLEARN_INTERVAL(60);

using namespace std;

CharacterController::CharacterController() :using_player_id("NULL"), inc_cnt(0)
{
}

CharacterController::~CharacterController()
{
}

CharacterController& CharacterController::GetInstance(){
	static CharacterController cc;
	return cc;
}

int CharacterController::Reset(){
	inc_cnt = 0;
	Clear();
	belonging_planet = nullptr;
	return 0;
}

int CharacterController::Process(){
	if (inc_cnt%ENEMY_CLEARN_INTERVAL == 0){ CleanEnemy(); }
	++inc_cnt;
	return 0;
}

void CharacterController::Clear(){
	enemies.clear();
	players.clear();
	using_player_id = "NULL";
}

void CharacterController::CleanEnemy(){
	enemies.erase(remove_if(enemies.begin(), enemies.end(), [](std::weak_ptr<CharacterBase>& e)->bool{return e.expired(); }), enemies.end());
	enemies.erase(remove_if(enemies.begin(), enemies.end(), [](std::weak_ptr<CharacterBase>& e)->bool{return !CharacterManager::GetInstance().CheckObjectResisted(e.lock()->GetCharID()); }), enemies.end());
}

std::shared_ptr<CharacterBase> CharacterController::CreateAndResistEnemy(CharaAtrribtute ca, const string& inid){
	std::shared_ptr<CharacterBase> e = CharacterManager::GetInstance().CreateAndResistObject(ca, inid);
	if (e){
		if (belonging_planet){ e->SetBelongingPlanet(belonging_planet); }
		enemies.push_back(e);
		auto pit = players.find(using_player_id);
		if (pit == players.end()){ return e; }
		e->SetTarget(pit->second);
//		e->Init();
		return e;
	}
	else{
		return nullptr;
	}
}

std::shared_ptr<CharacterBase> CharacterController::ResistEnemy(std::shared_ptr<CharacterBase> e){
	if (e){
		CharacterManager::GetInstance().ResistObject(e);
		if (belonging_planet){ e->SetBelongingPlanet(belonging_planet); }
		enemies.push_back(e);
		auto pit = players.find(using_player_id);
		if (pit == players.end()){ return e; }
		e->SetTarget(pit->second);
//		e->Init();
		return e;
	}
	else{
		return nullptr;
	}
}

std::shared_ptr<CharacterBase> CharacterController::CreatePlayer(CharaAtrribtute ca, const string& inid, const string& id){
	//IDが重複していたらそれを返す
	auto it = players.find(id);
	if (it != players.end()){ return it->second; }
	std::shared_ptr<CharacterBase> p = CharacterManager::GetInstance().CreateObject(ca, inid);
	if (p){
		if (belonging_planet){ p->SetBelongingPlanet(belonging_planet); }
		players.insert(unordered_map<string, shared_ptr<CharacterBase>>::value_type(id, p));
//		p->Init();
		return p;
	}
	else{
		return nullptr;
	}
}

int CharacterController::ResistPlayer(shared_ptr<CharacterBase> plr, const string& p_id){
	//IDが重複していたらエラー
	if (players.find(p_id) != players.end())return -1;
	players.insert(unordered_map<string, shared_ptr<CharacterBase>>::value_type(p_id, plr));
	return 0;
}

int CharacterController::UnresistPlayer(const string& p_id){
	if (p_id == using_player_id){
		using_player_id = "NULL";
		setPlayerToEnemy(nullptr);
	}
	auto it = players.find(p_id);
	if (it == players.end()){ return -1; }
	players.erase(it);
	return 0;
}

int CharacterController::ChangePlayer(const string& id){
	auto rit = players.find(id);
	if (rit == players.end()){ return -1; }
	auto it = players.find(using_player_id);
	if (it != players.end()){
		CharacterManager::GetInstance().DeleteObject(it->second->GetCharID());
	}
	CharacterManager::GetInstance().ResistObject(rit->second);
	setPlayerToEnemy(rit->second);
	using_player_id = id;
	return 0;
}

void CharacterController::setPlayerToEnemy(shared_ptr<CharacterBase> p){
	CleanEnemy();
	for (weak_ptr<CharacterBase>& e : enemies){
		e.lock()->SetTarget(p);
	}
}

void CharacterController::SetBelongingPlanet(shared_ptr<Planet> p){ belonging_planet = p; }
std::shared_ptr<Planet> CharacterController::GetBelongingPlanet()const{ return belonging_planet; }
int CharacterController::GetEnemyCount()const{ return enemies.size(); }