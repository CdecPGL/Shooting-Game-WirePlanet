#pragma once

#include"CreatorTemplate.h"
#include<unordered_map>
#include"BulletCreator.h"

//�v���C���[�N���G�[�^
class PlayerCreator : public CreatorTemplate < CharacterBase > {
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	struct InitData{
		//HP, ���C, �ő�ړ����x,���G����, �A�N�V�����t�@�C��
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

//�G�N���G�[�^
class EnemyCreator : public CreatorTemplate < CharacterBase > {
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	struct InitData{
		//HP,���C,�s�K����,�h���b�v�G�i�W�[,���[�oID,�A�N�V�����t�@�C��
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

//�A�C�e���N���G�[�^�[
class ItemCreator : public CreatorTemplate < CharacterBase > {
public:
	int Init()override;
	std::shared_ptr<CharacterBase> Create(const std::string&);
	bool CheckInitIDExistence(const std::string&)const override;
private:
	struct InitData{
		//�A�N�V�����t�@�C��
		InitData(const std::string&);
		std::string action_file;
	};
	std::unordered_map<std::string, InitData> init_datas;
	int LoadInitData();
};
