#pragma once

#include"DxLib.h"
#include"Key.h"
#include"Counter.h"
#include<string>
#include<memory>
#include"DataContainer.h"
//ゲーム場面

using namespace std;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual int Init(shared_ptr<DataContainer>); //初期化
	virtual int End(); //終了処理

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool Initialized()const; //初期化されたか

	std::string GetID()const;

	//初期化前関数
	void SetID(const std::string&);
protected:
	Key& key;
//	Counter& counter;
	shared_ptr<DataContainer> dc;

	bool initialized; //初期化されたか
private:
	std::string ID;
};

