#include "AnimationEffect.h"
#include"GraphicController2D.h"

using namespace std;
using namespace AnimLib;

AnimationEffect::AnimationEffect(const string& afn) :animation(make_unique<Animation>()), _anim_data_name(afn)
{
}


AnimationEffect::~AnimationEffect()
{
}

int AnimationEffect::Init(){
	if (animation == nullptr){ return -1; }
	//アニメデータセット
	int res = animation->SetAnimationData(_anim_data_name);
	if (res<0){
		printf("AnimationEffect::Init アニメデータのセットに失敗しました。(reason:%d,name:%s)\n", res, _anim_data_name.c_str());
		return -2;
	}
	//初めのアニメを再生する
	vector<string> tags;
	animation->GetAnimationTagList(&tags);
	if (tags.size() == 0){ return -1; }
	animation->StartAnimationByTag(tags[0]);
	return 0;
}

void AnimationEffect::Update(){
	animation->Update();
	if (animation->CheckAnimationPlaying() == false){
		DeleteMe();
	}
}

void AnimationEffect::Draw(){
	double d_angle = GetDrawRotation();
	int ghandle = animation->GetCurrentGraph();
	gc2d::DrawRotaGraph((int)position.x, (int)position.y, 1.0, d_angle / 180 * DX_PI, ghandle, 1, false);
}