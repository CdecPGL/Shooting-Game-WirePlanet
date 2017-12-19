#pragma once

#include"CharacterManager.h"
#include<unordered_map>

class Planet;

class CharacterController
{
public:
	~CharacterController();
	CharacterController(const CharacterController&) = delete;
	static CharacterController& GetInstance();

	int Reset();
	int Process();

	void Clear();
	void CleanEnemy();

	std::shared_ptr<CharacterBase> CreateAndResistEnemy(CharaAtrribtute,const std::string&);
	std::shared_ptr<CharacterBase> ResistEnemy(std::shared_ptr<CharacterBase>);
	std::shared_ptr<CharacterBase> CreatePlayer(CharaAtrribtute, const std::string&, const std::string&);
	int ResistPlayer(std::shared_ptr<CharacterBase>, const std::string&); //�v���C���[�o�^(�|�C���^�A�v���C���[��)(�L�����Ǘ��ɃI���ނ���ꍇ�͎����œo�^���������)
	int UnresistPlayer(const std::string&); //�v���C���[�o�^����(�v���C���[��)

	int ChangePlayer(const std::string&);

	void SetBelongingPlanet(std::shared_ptr<Planet>);
	std::shared_ptr<Planet> GetBelongingPlanet()const;

	int GetEnemyCount()const;
	
private:
	int inc_cnt;
	CharacterController();
	std::vector<std::weak_ptr<CharacterBase>> enemies;
	std::unordered_map<std::string, std::shared_ptr<CharacterBase>> players;
	std::string using_player_id;

	std::shared_ptr<Planet> belonging_planet;

	void setPlayerToEnemy(std::shared_ptr<CharacterBase>);
};

