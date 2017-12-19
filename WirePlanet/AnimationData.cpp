#include "AnimationData.h"

namespace AnimLib{

	using namespace std;

//	const int Ghandle_Buf_Size(1000); //�摜�ǂݍ��ݎ��̉摜�n���h���o�b�t�@�T�C�Y
	const string AnimFile_Identification_String("AnimationFile_Cdec"); //�A�j���t�@�C��V2�ȍ~���ʕ�����

	//�A�j���[�V�����f�[�^���j�b�g�R���X�g���N�^
	AnimationDataUnit::AnimationDataUnit(const string m_tag, int m_gh, int m_intrvl, bool m_rf, int m_size) :tag(m_tag), interval(m_intrvl), repeat_flag(m_rf), size_x(m_size),size_y(m_size){
		//printfDx(tag);
		int x, y;
		GetGraphSize(m_gh, &x, &y);
		int w = x / size_x;
		int h = y / size_y;
		num = w*h;
	};

	//AnimationDataUnit�f�X�g���N�^
	AnimationDataUnit::~AnimationDataUnit(){
		//�摜�̍폜
		/*for (auto it = ghandle.begin(); it != ghandle.end(); ++it){
			DeleteGraph((*it));
		}*/
	}

	//�A�j���[�V�����f�[�^�R���X�g���N�^
	AnimationData::AnimationData() :animfilename("empty"), num(0){}

	//AnimationData�f�X�g���N�^
	AnimationData::~AnimationData(){
	}

	//�ǂݍ��񂾃t�@�C�����̊m�F
	bool AnimationData::CheckFileName(string fn)const{
		if (animfilename == fn){ return true; }
		else{ return false; }
	}

	//�摜�n���h����ADU�ɃZ�b�g
	void AnimationData::SetGhandlesToAnimDataUnit(){
		for (auto& adu : animationdataunit){
			adu->ghandle.clear();
			for (auto& fi : adu->frame_info){
				adu->ghandle.push_back(DerivationGraph(fi.pos_x,fi.pos_y,adu->size_x,adu->size_y,_ghandles[fi.graph_num] ));
			}
		}
	}
};
