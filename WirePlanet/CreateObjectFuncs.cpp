#include "CreateObjectFuncs.h"

#include"GraphicObject.h"

using namespace std;

shared_ptr<Object> CreateObjFuncs::CreateGraphicObject(const string& gn){
	std::shared_ptr<GraphicObject> go = make_shared<GraphicObject>();
	if (go->SetGraph(gn)){
		return nullptr;
	}
	return go;
}