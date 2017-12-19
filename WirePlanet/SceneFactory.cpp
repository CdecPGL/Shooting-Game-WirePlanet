#include "SceneFactory.h"
#include"Title.h"
#include"Game.h"
#include"ViewPlayInformations.h"
//#include"Scene.h"

SceneFactory::SceneFactory()
{
	nameidtable.insert(map<string,int>::value_type("Title",0));
	nameidtable.insert(map<string, int>::value_type("Game", 1));
	nameidtable.insert(map<string, int>::value_type("ViewPlayInformations", 3));
}


SceneFactory::~SceneFactory()
{
}

SceneFactory& SceneFactory::GetInstance(){
	static SceneFactory sf;
	return sf;
}

int SceneFactory::Init(){
	return 0;
}

//Scene�̍쐬
shared_ptr<Scene> SceneFactory::CreateObject(const string& init_id){
	shared_ptr<Scene> scn;
	switch ((*nameidtable.find(init_id)).second){
	case 0:
		scn.reset(new Title);
		break;
	case 1:
		scn.reset(new Game);
		break;
	case 2:
		break;
	case 3:
		scn.reset(new ViewPlayInformations);
		break;
	default:
		break;
	}
	return scn;
}
