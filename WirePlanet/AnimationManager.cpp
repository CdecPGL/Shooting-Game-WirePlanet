#include"AnimationManager.h"

using namespace std;

namespace AnimLib{
	AnimationManager::AnimationManager(){
	}

	AnimationManager::~AnimationManager(){}

	AnimationManager& AnimationManager::GetInstance(){
		static AnimationManager am;
		return am;
	}

	shared_ptr<Animation> AnimationManager::CreateAnimation(const string& anim_dat_name){
		return nullptr;
	}
}

