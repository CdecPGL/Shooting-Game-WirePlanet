#pragma once

#include"CreatorTemplate.h"
#include"character.h"
#include<unordered_map>

//弾クリエータ
class BulletCreator : public CreatorTemplate<CharacterBase>{
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	using InitFunc = std::shared_ptr<CharacterBase>(BulletCreator::*)(int, int,int, const std::string&);
	struct InitData{
		InitData(InitFunc,const std::string&,int,int,int);
		InitFunc init_func;
		const std::string action_file;
		int blend_mode;
		int blend_param;
		int col;
	};
	std::unordered_map<std::string,InitData> init_datas; //初期化データリスト
	std::unordered_map<std::string, InitFunc> init_func_list; //初期化関数対応リスト

	//初期化データ読み込み
	int LoadInitData();

	//初期化関数(コンストラクタに渡す変数)
	std::shared_ptr<CharacterBase> GetStraightBullet(int, int,int, const std::string&); //StraightBullet
	std::shared_ptr<CharacterBase> GetBoomerang(int, int,int, const std::string&); //Boomerang
	std::shared_ptr<CharacterBase> GetGuidedBullet(int, int, int, const std::string&); //GuidedBullet
};

