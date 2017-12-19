#pragma once

#include"DxLib.h"
#include"Key.h"
#include"Counter.h"
#include<string>
#include<memory>
#include"DataContainer.h"
//�Q�[�����

using namespace std;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual int Init(shared_ptr<DataContainer>); //������
	virtual int End(); //�I������

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool Initialized()const; //���������ꂽ��

	std::string GetID()const;

	//�������O�֐�
	void SetID(const std::string&);
protected:
	Key& key;
//	Counter& counter;
	shared_ptr<DataContainer> dc;

	bool initialized; //���������ꂽ��
private:
	std::string ID;
};

