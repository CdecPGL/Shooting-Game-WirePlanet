#include "Animation.h"
#include <stdio.h>

//AnimationClass

namespace AnimLib{

	//コンストラクタ
	Animation::Animation():frame(-1),counter(0),current_anim(0),_roop_flag(false),_playing(false){}

	//コンストラクタ(ActionDataの参照を得る)
	Animation::Animation(const std::string& filename) : frame(-1), counter(0), current_anim(0), _roop_flag(false), _playing(false),animationdata(AnimationDataAdmin::GetInstace().GetAnimationDataPointer(filename)){

	}

	//コンストラクタ(ActionDataの参照を得る)
	Animation::Animation(shared_ptr<const AnimationData> ad) : frame(-1), counter(0), current_anim(0), _roop_flag(false), _playing(false),animationdata(ad){

	}

	//デストラクタ
	Animation::~Animation(void){

	}

	//アニメーションデータのセット
	int Animation::AttachAnimationData(const string& adn){
		auto nad = AnimationDataAdmin::GetInstace().GetAnimationDataPointer(adn);
		if (nad == nullptr){ return -1; }
		animationdata = nad;
		return 0;
	}

	//アニメーションデータのセット
	int Animation::AttachAnimationData(shared_ptr<const AnimationData> ad){
		animationdata = ad;
		return 0;
	}

	//更新
	void Animation::Update() {
		if (animationdata == nullptr || animationdata->num == 0){ return; } //アクションがなかったらスキップ
		if (_playing){
			++counter;
			//フレームの加算(現在のアクションがアニメなしかカウンターが不十分なら加算しない。アニメーション開始(frame==-1)なら進める)
			if ((animationdata->animationdataunit.at(current_anim)->interval != 0 && counter >= animationdata->animationdataunit.at(current_anim)->interval) || frame==-1){
				frame++;
				counter = 0;
				//フレームがアニメーション枚数を超えたら
				if (frame >= animationdata->animationdataunit.at(current_anim)->num){
					if (_roop_flag == false){ //繰り返しなしだったら
						--frame; //最終フレームにしてアニメーション終了
						_playing = false;
					}
					else{ //繰り返しありだったら
						frame = 0; //0にして再びアニメーション
					}
				}
			}
			/*else{
				counter++;
				}*/
		}
	}

	//現在の画像の取得
	int Animation::GetCurrentGraph() const{
		if (animationdata==nullptr || animationdata->num == 0){ return -1; }
		//frameが-1だったら-3を返す
		if (frame == -1){ return -3; }
		return animationdata->animationdataunit.at(current_anim)->ghandle[frame];
		return -2;
	}

	//現在の画像のサイズを取得
	void Animation::GetCurrentGraphSize(int* w,int* h){
		if (animationdata==nullptr||animationdata->num == 0){ return; }
		if (w){ *w = animationdata->animationdataunit.at(current_anim)->size_x; }
		if (h){ *h = animationdata->animationdataunit.at(current_anim)->size_y; }
	}

	//現在のフレーム数を取得
	int Animation::GetCurrentFrame() const{
		if (animationdata==nullptr||animationdata->num == 0){ return 0; }
		return frame;
	}

	//現在のアクションのタグと引数を比較
	bool Animation::CheckCurrentAnimationTag(const std::string& str) const{
		if (animationdata==nullptr||animationdata->num == 0){ return false; }
		if (str == animationdata->animationdataunit.at(current_anim)->tag){ return true; }
		else{ return false; }
	}

	//現在のアクションのタグを取得
	std::string Animation::GetCurrentAnimationTag() const{
		if (animationdata==nullptr||animationdata->num == 0){ return "none"; }
		return animationdata->animationdataunit.at(current_anim)->tag;
	}

	//タグを用いて任意のアクションをスタート
	int Animation::StartAnimationByTag(const std::string& m_tag){
		if (animationdata==nullptr||animationdata->num == 0){ return -1; } //アニメーションがなかったらスキップ
		for (int i = 0; i<animationdata->num; i++){
			if (m_tag == animationdata->animationdataunit.at(i)->tag){ 
				current_anim = i; 
				frame = -1;
				_roop_flag = animationdata->animationdataunit.at(i)->repeat_flag;
			}
		}
		_playing = true;
		return 0;
	}
	int Animation::StartAnimationByTag(const std::string& m_tag,bool r_f){
		if (animationdata == nullptr || animationdata->num == 0){ return -1; } //アニメーションがなかったらスキップ
		for (int i = 0; i < animationdata->num; i++){
			if (m_tag == animationdata->animationdataunit.at(i)->tag){ current_anim = i; frame = -1; }
		}
		_roop_flag = r_f;
		_playing = true;
		return 0;
	}

	//アクションを一コマ進める
	int Animation::ProgressAnimation(){
		if (animationdata==nullptr||animationdata->num == 0){ return -1; } //アニメーションがなかったらスキップ
		if (_playing){
			frame++;
			counter = 0;
			//フレームがアニメーション枚数を超えたら
			if (frame >= animationdata->animationdataunit.at(current_anim)->num){
				if (_roop_flag == false){ //繰り返しなしだったら
					--frame; //最終フレームにしてアニメーション終了
					_playing = false;
				}
				else{ //繰り返しありだったら
					frame = 0; //0にして再びアニメーション
				}
			}
		}
		return 0;
	}

	//描画
	void Animation::Draw(double x, double y, double angle,double ext,bool rf, char c){
		if (animationdata == nullptr || animationdata->num == 0){ return; } //アニメーションがなかったらスキップ
		if (c == 0){
			DrawRotaGraph((int)x, (int)y, 1, angle, GetCurrentGraph(), true);
		}
		else{
			int size_x,size_y;
			GetCurrentGraphSize(&size_x, &size_y);
			int cx(size_x / 2), cy(size_y / 2);
			if ((c & 1) == 1){ cx = 0; }
			if ((c & 2) == 2){ cx = size_x; }
			if ((c & 4) == 4){ cy = 0; }
			if ((c & 8) == 8){ cy = size_y; }
			if ((c & 3) == 3){ cx = size_x / 2; }
			if ((c & 12) == 12){ cy = size_y / 2; }
			DrawRotaGraph2((int)x, (int)y, (int)cx, (int)cy, ext, angle, GetCurrentGraph(), true, rf);
		}
	}

	//サイズを指定して描画
	void Animation::FreeSizeDraw(double x, double y, double angle, int d_size, char c){
		if (animationdata == nullptr || animationdata->num == 0){ return; } //アニメーションがなかったらスキップ
		int size_x, size_y;
	GetCurrentGraphSize(&size_x, &size_y);
		double ExtRate(d_size / size_x);
		if (c == 0){
			DrawRotaGraph((int)x, (int)y, ExtRate, angle, GetCurrentGraph(), true);
		}
		else{
			int cx(size_x / 2), cy(size_y / 2);
			if ((c & 1) == 1){ cx = 0; }
			if ((c & 2) == 2){ cx = size_x; }
			if ((c & 4) == 4){ cy = 0; }
			if ((c & 8) == 8){ cy = size_y; }
			if ((c & 3) == 3){ cx = size_x / 2; }
			if ((c & 12) == 12){ cy = size_y / 2; }
			DrawRotaGraph2((int)x, (int)y, (int)cx, (int)cy, ExtRate, angle, GetCurrentGraph(), true);
		}
	}

	//縦横拡大率を指定して描画
	void Animation::FreeXYExtDraw(double x, double y, double angle, double ext_x,double ext_y, char c){
		if (animationdata == nullptr || animationdata->num == 0){ return; } //アニメーションがなかったらスキップ
		int size_x, size_y;
		GetCurrentGraphSize(&size_x, &size_y);
		int cx(size_x / 2), cy(size_y / 2);
		if ((c & 1) == 1){ cx = 0; }
		if ((c & 2) == 2){ cx = size_x; }
		if ((c & 4) == 4){ cy = 0; }
		if ((c & 8) == 8){ cy = size_y; }
		if ((c & 3) == 3){ cx = size_x / 2; }
		if ((c & 12) == 12){ cy = size_y / 2; }
			DrawRotaGraph3((int)x, (int)y, (int)cx, (int)cy, ext_x,ext_y, angle, GetCurrentGraph(), true);
	}

	//アニメーションデータをセット
	int Animation::SetAnimationData(const string& af){
		return AttachAnimationData(af);
	}

	//アニメーション再生中か確認
	bool Animation::CheckAnimationPlaying()const{
		return _playing;
	}

	//アニメーションのタグリスト取得
	void Animation::GetAnimationTagList(vector<string>* v)const{
		if (v == nullptr){ return; }
		for (const unique_ptr<AnimationDataUnit>& a : animationdata->animationdataunit){
			v->push_back(a->tag);
		}
	}
	vector<string> Animation::GetAnimationTagList()const{
		vector<string> v;
		for (const unique_ptr<AnimationDataUnit>& a : animationdata->animationdataunit){
			v.push_back(a->tag);
		}
		return move(v);
	}

	void Animation::Reset()
	{
		_playing = false;
		counter = 0;
		frame = -1;
		current_anim = 0;
		_roop_flag = false;
	}

	void Animation::StopAnimation()
	{
		_playing = false;
	}


};