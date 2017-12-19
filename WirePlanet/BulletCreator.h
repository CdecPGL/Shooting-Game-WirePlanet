#pragma once

#include"CreatorTemplate.h"
#include"character.h"
#include<unordered_map>

//�e�N���G�[�^
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
	std::unordered_map<std::string,InitData> init_datas; //�������f�[�^���X�g
	std::unordered_map<std::string, InitFunc> init_func_list; //�������֐��Ή����X�g

	//�������f�[�^�ǂݍ���
	int LoadInitData();

	//�������֐�(�R���X�g���N�^�ɓn���ϐ�)
	std::shared_ptr<CharacterBase> GetStraightBullet(int, int,int, const std::string&); //StraightBullet
	std::shared_ptr<CharacterBase> GetBoomerang(int, int,int, const std::string&); //Boomerang
	std::shared_ptr<CharacterBase> GetGuidedBullet(int, int, int, const std::string&); //GuidedBullet
};

