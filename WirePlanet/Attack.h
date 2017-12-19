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

	//エフェクト作成
	int CreateEffect(int, const std::string&);
	//弾作成
	int CreateBullet(int, const std::string&);
	//オブジェクト操作関数
private:
	using ObjectList = std::unordered_multimap < int, std::weak_ptr<Object> > ;
	ObjectList objects;
};
