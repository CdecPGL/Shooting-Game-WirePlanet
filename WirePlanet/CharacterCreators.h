#pragma once

#include"CreatorTemplate.h"
#include<unordered_map>
#include"BulletCreator.h"

//プレイヤークリエータ
class PlayerCreator : public CreatorTemplate < CharacterBase > {
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	struct InitData{
		//HP, 摩擦, 最大移動速度,無敵時間, アクションファイル
		InitData(int, double, double, int, const std::string&);
		int HP;
		double friction;
		double max_speed;
		int matchless_time;
		std::string action_file;
	};
	std::unordered_map<std::string, InitData> init_datas;
	int LoadInitData();
};

//敵クリエータ
class EnemyCreator : public CreatorTemplate < CharacterBase > {
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	struct InitData{
		//HP,摩擦,不適時間,ドロップエナジー,ムーバID,アクションファイル
		InitData(int, double, int, int, const std::string&, const std::string&);
		int HP;
		double friction;
		int matchless_time;
		int drop_energy;
		std::string mover_id;
		std::string action_file;
	};
	std::unordered_map<std::string, InitData> init_datas;
	int LoadInitData();
};

//アイテムクリエーター
class ItemCreator : public CreatorTemplate < CharacterBase > {
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	struct InitData{
		//アクションファイル
		InitData(const std::string&);
		std::string action_file;
	};
	std::unordered_map<std::string, InitData> init_datas;
	int LoadInitData();
};
