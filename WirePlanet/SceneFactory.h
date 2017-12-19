#pragma once

#include<map>
#include<memory>
#include"Scene.h"
//�V���O���g��

class Scene;

class SceneFactory
{
public:
	~SceneFactory();
	SceneFactory(const SceneFactory&) = delete;
	int Init();
	static SceneFactory& GetInstance();
	std::shared_ptr<Scene> CreateObject(const std::string&); //�V�����V�[���̍쐬(�O��쐬���ꂽ���̂͏��������)(���s�����疳���ȃ|�C���^��)
	bool CheckInitDataExistence(const std::string&)const;
	void ClearInitData();
private:
	SceneFactory();
	bool initialized; //�������t���O
	map<string, int> nameidtable; //��ʖ��Ɣԍ��̑Ή��\
};

