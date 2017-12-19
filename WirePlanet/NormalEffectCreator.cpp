#include "NormalEffectCreator.h"

//#include"InitDatas.h"
#include"FileLoadUtility.h"
#include"SVLoader.h"
#include"boost\lexical_cast.hpp"
#include"AnimationEffect.h"

using namespace std;

const string EFCT_LIST_NAME("efct_list.csv");
const int LIST_ITEM_NUM(3); //リストの項目数

NormalEffectCreator::NormalEffectCreator()
{
}


NormalEffectCreator::~NormalEffectCreator()
{
}

int NormalEffectCreator::Init(){
	if (LoadFile()){
		printf("NormalEffectCreatorの初期化に失敗しました。NormalEffectリストの読み込みに失敗しました。\n");
		return -1;
	}
	CreatorTemplate<Effect>::Init();
	printf("NormalEffectCreatorが初期化されました。\n");
	return 0;
}

shared_ptr<Effect> NormalEffectCreator::Create(const string& init_id){
	auto it = init_datas.find(init_id);
	if (it == init_datas.end()){ return nullptr; }
	shared_ptr<Effect> out;
	switch ((*it).second.effect_kind)
	{
	case Animation_Effect:
		out = make_shared<AnimationEffect>((*it).second.file_name);
		return out;
		break;
	default:
		break;
	}
	return nullptr;
}

bool NormalEffectCreator::CheckInitIDExistence(const string& init_id)const{
	return !(init_datas.find(init_id)==init_datas.end());
}

int NormalEffectCreator::LoadFile(){
	try{
		//リストを取得
//		string list_pass = InitDatas::GetInitDatas("Resource").at("NormalEffectList");
		string list_pass = EFCT_LIST_NAME;
		//リストを読み込み
		CSVLoader scvl;
//		scvl.LoadCSV(list_pass);
		if (FileLoadUtil::LoadCSVFromFLM(&scvl, EFCT_LIST_NAME, true)){
			return -2;
		}
		//Directory取得
		/*const string dir = list_pass.substr(0, list_pass.find_last_of("\\")) + "\\";
		directory = dir;*/
		for (vector<string>& s : scvl){
			if (s.size() != LIST_ITEM_NUM){
				printf("リストの項目数が不正です。(%d/%d)\n", s.size(),LIST_ITEM_NUM);
				continue;
			}
			InitData inid;
			//エフェクトの種類取得
			inid.effect_kind = GetEffectKindEnumFromString(s[1]);
			switch (inid.effect_kind){
			case Animation_Effect:
				//アニメファイル名取得
				inid.file_name = s[2];
				break;
			default:
				printf("NormalEffectの種類が不正です。(%s)\n", s[1].c_str());
				continue;
				break;
			}
			init_datas.insert(unordered_map<string, InitData>::value_type(s[0],inid));
		}
		return 0;
	}
	catch (out_of_range& e){
		printf("初期データが不足しています。(%s)\n", e.what());
		return -1;
	}
	/*catch (InitDatas::init_datas_error& e){
		printf("初期データのセクションが不足しています。(%s)\n", e.what());
		return -1;
	}*/
}

//文字列からエフェクト種別列挙体取得
NormalEffectCreator::EffectKind NormalEffectCreator::GetEffectKindEnumFromString(const string& str){
	if (str == "AnimationEffect"){
		return Animation_Effect;
	}
	else{
		return Error;
	}
}
