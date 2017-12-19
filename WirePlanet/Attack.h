#pragma once

#include "object.h"
#include<unordered_map>

class Attack :
	public Object
{
public:
	Attack();
	virtual ~Attack()override;

	int Init()override;

	void Update()override;

	//�G�t�F�N�g�쐬
	int CreateEffect(int, const std::string&);
	//�e�쐬
	int CreateBullet(int, const std::string&);
	//�I�u�W�F�N�g����֐�
private:
	using ObjectList = std::unordered_multimap < int, std::weak_ptr<Object> > ;
	ObjectList objects;
};
