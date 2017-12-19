#pragma once

#include<map>
#include<memory>
#include"Scene.h"
//シングルトン

class Scene;

class SceneFactory
{
public:
	~SceneFactory();
	SceneFactory(const SceneFactory&) = delete;
	int Init();
	static SceneFactory& GetInstance();
	std::shared_ptr<Scene> CreateObject(const std::string&); //新しいシーンの作成(前回作成されたものは消去される)(失敗したら無効なポインタを)
	bool CheckInitDataExistence(const std::string&)const;
	void ClearInitData();
private:
	SceneFactory();
	bool initialized; //初期化フラグ
	map<string, int> nameidtable; //場面名と番号の対応表
};

