#include "Animation.h"
#include <stdio.h>

//AnimationClass

namespace AnimLib{

	//�R���X�g���N�^
	Animation::Animation():frame(-1),counter(0),current_anim(0),_roop_flag(false),_playing(false){}

	//�R���X�g���N�^(ActionData�̎Q�Ƃ𓾂�)
	Animation::Animation(const std::string& filename) : frame(-1), counter(0), current_anim(0), _roop_flag(false), _playing(false),animationdata(AnimationDataAdmin::GetInstace().GetAnimationDataPointer(filename)){

	}

	//�R���X�g���N�^(ActionData�̎Q�Ƃ𓾂�)
	Animation::Animation(shared_ptr<const AnimationData> ad) : frame(-1), counter(0), current_anim(0), _roop_flag(false), _playing(false),animationdata(ad){

	}

	//�f�X�g���N�^
	Animation::~Animation(void){

	}

	//�A�j���[�V�����f�[�^�̃Z�b�g
	int Animation::AttachAnimationData(const string& adn){
		auto nad = AnimationDataAdmin::GetInstace().GetAnimationDataPointer(adn);
		if (nad == nullptr){ return -1; }
		animationdata = nad;
		return 0;
	}

	//�A�j���[�V�����f�[�^�̃Z�b�g
	int Animation::AttachAnimationData(shared_ptr<const AnimationData> ad){
		animationdata = ad;
		return 0;
	}

	//�X�V
	void Animation::Update() {
		if (animationdata == nullptr || animationdata->num == 0){ return; } //�A�N�V�������Ȃ�������X�L�b�v
		if (_playing){
			++counter;
			//�t���[���̉��Z(���݂̃A�N�V�������A�j���Ȃ����J�E���^�[���s�\���Ȃ���Z���Ȃ��B�A�j���[�V�����J�n(frame==-1)�Ȃ�i�߂�)
			if ((animationdata->animationdataunit.at(current_anim)->interval != 0 && counter >= animationdata->animationdataunit.at(current_anim)->interval) || frame==-1){
				frame++;
				counter = 0;
				//�t���[�����A�j���[�V���������𒴂�����
				if (frame >= animationdata->animationdataunit.at(current_anim)->num){
					if (_roop_flag == false){ //�J��Ԃ��Ȃ���������
						--frame; //�ŏI�t���[���ɂ��ăA�j���[�V�����I��
						_playing = false;
					}
					else{ //�J��Ԃ����肾������
						frame = 0; //0�ɂ��čĂуA�j���[�V����
					}
				}
			}
			/*else{
				counter++;
				}*/
		}
	}

	//���݂̉摜�̎擾
	int Animation::GetCurrentGraph() const{
		if (animationdata==nullptr || animationdata->num == 0){ return -1; }
		//frame��-1��������-3��Ԃ�
		if (frame == -1){ return -3; }
		return animationdata->animationdataunit.at(current_anim)->ghandle[frame];
		return -2;
	}

	//���݂̉摜�̃T�C�Y���擾
	void Animation::GetCurrentGraphSize(int* w,int* h){
		if (animationdata==nullptr||animationdata->num == 0){ return; }
		if (w){ *w = animationdata->animationdataunit.at(current_anim)->size_x; }
		if (h){ *h = animationdata->animationdataunit.at(current_anim)->size_y; }
	}

	//���݂̃t���[�������擾
	int Animation::GetCurrentFrame() const{
		if (animationdata==nullptr||animationdata->num == 0){ return 0; }
		return frame;
	}

	//���݂̃A�N�V�����̃^�O�ƈ������r
	bool Animation::CheckCurrentAnimationTag(const std::string& str) const{
		if (animationdata==nullptr||animationdata->num == 0){ return false; }
		if (str == animationdata->animationdataunit.at(current_anim)->tag){ return true; }
		else{ return false; }
	}

	//���݂̃A�N�V�����̃^�O���擾
	std::string Animation::GetCurrentAnimationTag() const{
		if (animationdata==nullptr||animationdata->num == 0){ return "none"; }
		return animationdata->animationdataunit.at(current_anim)->tag;
	}

	//�^�O��p���ĔC�ӂ̃A�N�V�������X�^�[�g
	int Animation::StartAnimationByTag(const std::string& m_tag){
		if (animationdata==nullptr||animationdata->num == 0){ return -1; } //�A�j���[�V�������Ȃ�������X�L�b�v
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
		if (animationdata == nullptr || animationdata->num == 0){ return -1; } //�A�j���[�V�������Ȃ�������X�L�b�v
		for (int i = 0; i < animationdata->num; i++){
			if (m_tag == animationdata->animationdataunit.at(i)->tag){ current_anim = i; frame = -1; }
		}
		_roop_flag = r_f;
		_playing = true;
		return 0;
	}

	//�A�N�V��������R�}�i�߂�
	int Animation::ProgressAnimation(){
		if (animationdata==nullptr||animationdata->num == 0){ return -1; } //�A�j���[�V�������Ȃ�������X�L�b�v
		if (_playing){
			frame++;
			counter = 0;
			//�t���[�����A�j���[�V���������𒴂�����
			if (frame >= animationdata->animationdataunit.at(current_anim)->num){
				if (_roop_flag == false){ //�J��Ԃ��Ȃ���������
					--frame; //�ŏI�t���[���ɂ��ăA�j���[�V�����I��
					_playing = false;
				}
				else{ //�J��Ԃ����肾������
					frame = 0; //0�ɂ��čĂуA�j���[�V����
				}
			}
		}
		return 0;
	}

	//�`��
	void Animation::Draw(double x, double y, double angle,double ext,bool rf, char c){
		if (animationdata == nullptr || animationdata->num == 0){ return; } //�A�j���[�V�������Ȃ�������X�L�b�v
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

	//�T�C�Y���w�肵�ĕ`��
	void Animation::FreeSizeDraw(double x, double y, double angle, int d_size, char c){
		if (animationdata == nullptr || animationdata->num == 0){ return; } //�A�j���[�V�������Ȃ�������X�L�b�v
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

	//�c���g�嗦���w�肵�ĕ`��
	void Animation::FreeXYExtDraw(double x, double y, double angle, double ext_x,double ext_y, char c){
		if (animationdata == nullptr || animationdata->num == 0){ return; } //�A�j���[�V�������Ȃ�������X�L�b�v
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

	//�A�j���[�V�����f�[�^���Z�b�g
	int Animation::SetAnimationData(const string& af){
		return AttachAnimationData(af);
	}

	//�A�j���[�V�����Đ������m�F
	bool Animation::CheckAnimationPlaying()const{
		return _playing;
	}

	//�A�j���[�V�����̃^�O���X�g�擾
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