#include "AnimationData.h"

namespace AnimLib{

	using namespace std;

//	const int Ghandle_Buf_Size(1000); //画像読み込み時の画像ハンドルバッファサイズ
	const string AnimFile_Identification_String("AnimationFile_Cdec"); //アニメファイルV2以降識別文字列

	//アニメーションデータユニットコンストラクタ
	AnimationDataUnit::AnimationDataUnit(const string m_tag, int m_gh, int m_intrvl, bool m_rf, int m_size) :tag(m_tag), interval(m_intrvl), repeat_flag(m_rf), size_x(m_size),size_y(m_size){
		//printfDx(tag);
		int x, y;
		GetGraphSize(m_gh, &x, &y);
		int w = x / size_x;
		int h = y / size_y;
		num = w*h;
	};

	//AnimationDataUnitデストラクタ
	AnimationDataUnit::~AnimationDataUnit(){
		//画像の削除
		/*for (auto it = ghandle.begin(); it != ghandle.end(); ++it){
			DeleteGraph((*it));
		}*/
	}

	//アニメーションデータコンストラクタ
	AnimationData::AnimationData() :animfilename("empty"), num(0){}

	//AnimationDataデストラクタ
	AnimationData::~AnimationData(){
	}

	//読み込んだファイル名の確認
	bool AnimationData::CheckFileName(string fn)const{
		if (animfilename == fn){ return true; }
		else{ return false; }
	}

	//画像ハンドルをADUにセット
	void AnimationData::SetGhandlesToAnimDataUnit(){
		for (auto& adu : animationdataunit){
			adu->ghandle.clear();
			for (auto& fi : adu->frame_info){
				adu->ghandle.push_back(DerivationGraph(fi.pos_x,fi.pos_y,adu->size_x,adu->size_y,_ghandles[fi.graph_num] ));
			}
		}
	}
};
