#include "Item.h"

using namespace std;

Item::Item(const string& af_id) :CharacterBase(1.0,1,ObjectBelongingTo::Neutral,af_id)
{
	_attribute = ITEM;
}


Item::~Item()
{
}

int Item::Init(){
	return 0;
}

void Item::Update(){

}

void Item::Draw(){

}

void Item::ProcessCollisionWithPlanet(){
	//òfêØ
	for (CollisionDataWithPlanet& c : col_data_p){
		r = c.r;
		vr = 0;
	}
	col_data_p.clear();
}
