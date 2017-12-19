#pragma once

#include"DxLib.h"
#include"Key.h"
#include"Counter.h"
#include<string>
#include<memory>
#include"DataContainer.h"
//ƒQ[ƒ€ê–Ê

using namespace std;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual int Init(shared_ptr<DataContainer>); //‰Šú‰»
	virtual int End(); //I—¹ˆ—

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool Initialized()const; //‰Šú‰»‚³‚ê‚½‚©

	std::string GetID()const;

	//‰Šú‰»‘OŠÖ”
	void SetID(const std::string&);
protected:
	Key& key;
//	Counter& counter;
	shared_ptr<DataContainer> dc;

	bool initialized; //‰Šú‰»‚³‚ê‚½‚©
private:
	std::string ID;
};

