#include "LaserManager.h"

#include"ObjectHolderTemplate_WithoutID.h"
#include"GraphicManager.h"
#include"CollisionDatas.h"

using namespace std;

const string LaserGraphicFile("Laser64");
const string LaserColGraphicFile("LaserCol64");

LaserManager::LaserManager() :_laser_holder(make_unique<ObjectHolderTemplate_WithoutID<Laser>>()), _laser_ghandle(-1)
{
}


LaserManager::~LaserManager()
{
}

LaserManager& LaserManager::GetInstance(){
	static LaserManager lm;
	return lm;
}

int LaserManager::Init(){
	_laser_ghandle = GraphicManager::GetInstance().GetGraphHandle(LaserGraphicFile);
	_laser_col_ghandle = GraphicManager::GetInstance().GetGraphHandle(LaserColGraphicFile);
	return 0;
}

int LaserManager::Reset(){
	_laser_holder->clear();
	return 0;
}

shared_ptr<Laser> LaserManager::CreateLaser(const string& id){
	shared_ptr<Laser> nl = make_shared<Laser>();
	nl->SetGraphicHandle(_laser_ghandle,_laser_col_ghandle);
	if (nl->Init()){ return nullptr; }
	_laser_holder->resist_object(nl);
	return nl;
}

int LaserManager::DeleteLaser(Laser* ls){
	return _laser_holder->unresist_object(ls);
}

void LaserManager::Process(){
	_laser_holder->process_resist_list();
	_laser_holder->process_unresist_list();
}

void LaserManager::Update(){
	for (auto& it : _laser_holder->objects){
		it->Update();
	}
}

void LaserManager::Draw(){
	for (auto& it : _laser_holder->objects){
		it->Draw();
	}
}

vector<CollisionDatas::LaserCollisionObject> LaserManager::GetLaserCollionObjects()const{
	vector<CollisionDatas::LaserCollisionObject> out;
	for (auto& l : _laser_holder->objects){
		CollisionDatas::LaserCollisionObject lco;
		lco.belongto = l->GetBelongingTo();
		lco.position = l->GetPositionXY();
		double rota = (l->GetDrawRotation()) / 180.0*DX_PI;
		lco.direction.Set(cos(rota), sin(rota));
		lco.length = l->GetLength();
		lco.width = l->GetWidth();
		lco.power = 1;
		out.push_back(lco);
	}
	return move(out);
}
