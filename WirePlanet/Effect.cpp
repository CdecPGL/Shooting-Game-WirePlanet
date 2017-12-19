#include "Effect.h"
#include"EffectManager.h"


Effect::Effect() :Object(0.0,0.0),size_w(0)
{
}


Effect::~Effect()
{
}

void Effect::SetWidth(int w){ size_w = w; }

void Effect::DeleteMe(){
	EffectManager::GetInstance().DeleteObject(this);
}