#pragma once
#include"object.h"
class Effect : public Object
{
public:
	//�R���X�g���N�^(�s�N�Z���T�C�Y��)
	Effect();
	virtual ~Effect();
	virtual int Init() = 0;
	virtual void Update()override = 0;
	virtual void Draw()override = 0;
	//�������O�ɌĂ�
	void SetWidth(int);

protected:
	int size_w; //�s�N�Z���T�C�Y��(0�Ō����喔�͎�������)

	void DeleteMe(); //�������폜
};
