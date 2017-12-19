#pragma once
#include "CharacterBase.h"
#include"Action.h"

class Item :
	public CharacterBase
{
public:
	Item(const std::string&);
	virtual ~Item();
	int Init()override;
	void Update()override;
	void Draw()override;
protected:
	void ProcessCollisionWithPlanet()override;

private:
	std::shared_ptr<Action> _action;
};
