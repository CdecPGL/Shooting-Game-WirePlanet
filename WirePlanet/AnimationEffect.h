#pragma once
#include "Effect.h"
#include"Animation.h"
class AnimationEffect :
	public Effect
{
public:
	//アニメファイル名
	AnimationEffect(const std::string&);
	~AnimationEffect();
	int Init()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<AnimLib::Animation> animation;
	std::string _anim_data_name;
};
