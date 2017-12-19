#pragma once

#include"Effect.h"

class SingleGraphEffect : public Effect
{
public:
	SingleGraphEffect();
	~SingleGraphEffect();
	int Init()override;
	void Update()override;
	void Draw()override;
};
