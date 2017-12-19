#pragma once

#include<memory>
#include<string>
#include"Animation.h"

namespace AnimLib{
	class AnimationManager{
	public:
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager& operator = (const AnimationManager&) = delete;
		~AnimationManager();
		static AnimationManager& GetInstance();
		std::shared_ptr<Animation> CreateAnimation(const std::string&); //�A�j���[�V�����쐬

	private:
		AnimationManager();
	};
}