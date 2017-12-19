#include "GraphicObject.h"
#include"GraphicManager.h"
#include"GraphicController2D.h"

using namespace std;

GraphicObject::GraphicObject(): _ghandle(-1)
{
}


GraphicObject::~GraphicObject()
{
}

void GraphicObject::Update(){
	ProcessAutoMove();
	ApplyVelocity();
	ConvertPTW();
}

void GraphicObject::Draw(){
	gc2d::DrawRotaGraph(position.x, position.y, 1.0, 0.0, _ghandle, true);
}

int GraphicObject::SetGraph(const string& gn){
	_ghandle = GraphicManager::GetInstance().GetGraphHandle(gn);
	return _ghandle < 0 ? -1 : 0;
}
