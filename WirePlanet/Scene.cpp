#include "Scene.h"

using namespace std;

Scene::Scene() :key(Key::GetInstance()), initialized(false)
{
}


Scene::~Scene()
{
}

int Scene::Init(shared_ptr<DataContainer> m_dc){
	dc = m_dc;
	initialized = true;
	return 0;
}

int Scene::End(){
	initialized = false;
	return 0;
}

void Scene::SetID(const std::string& id){
	ID = id;
}

string Scene::GetID()const{ return ID; }

bool Scene::Initialized()const{ return initialized; }
