#pragma once

#include "DxLib.h"
#include<memory>
#include<string>
#include<unordered_map>
#include"Vector2D.h"

class CharacterBase;
class PlayerMeter;

class InformationArea
{
public:
	InformationArea();
	~InformationArea();
	int Init();
	void Update();
	void Draw();

	void SetPoint(unsigned long long int);

	void SetLeftAreaWidth(int);
	void SetRightAreaWidth(int);
	int ResistPlayer(const std::string&,std::shared_ptr<CharacterBase>,const std::string& ="NULL"); //�v���C���[�̓o�^(�v���C���[ID�A�v���C���[�|�C���^[�A�A�C�R���摜])
	int UnesistPlayer(const std::string&); //�v���C���[�̓o�^����(�v���C���[ID)

	int ChangePlayer(const std::string&); //�v���C���[���
private:
	Vector2D<int> _draw_area_size;

	int left_area_width; //���G���A�̕�
	int right_area_width; //�E�G���A�̕�

	struct PlayerInfo{
		PlayerInfo();
		PlayerInfo(std::shared_ptr<CharacterBase>);
		std::unique_ptr<PlayerMeter> meter;
		std::shared_ptr<CharacterBase> player;
		void Update();
	};
	std::unordered_map<std::string, std::unique_ptr<PlayerInfo>> _players; //�v���C���[���X�g
	std::pair<std::string,PlayerInfo*> usg_player; //�g�p���̃v���[���[

	std::list<PlayerInfo*> _player_proc_list; //�v���C���[�������X�g(�擪�����ݎg���Ă���v���C���[)



	void set_player_meter_position();

	unsigned long long int _point;
	unsigned int _hight_score;

	int _default_fh;
};

