#pragma once

#include"Vector2D.h"
#include<memory>
#include"DxLib.h"

class CharacterBase;
class Player;

class PlayerMeter
{
public:
	int Init();
	void Update();
	void Draw();
	PlayerMeter();
	~PlayerMeter();
	int SetPlayer(std::shared_ptr<CharacterBase>);
	void SetPosition(const Vector2D<int>&);
	int SetRountGauge(const std::string&); //�~�`�Q�[�W�̉摜�Z�b�g
	int SetFaceIcon(const std::string&); //�摜�A�C�R�����Z�b�g
	int SetEXPGaugeBoard(const std::string&); //�o���l�\���摜�̃Z�b�g
	int SetMeterFrame(const std::string&); //���[�^�[�t���[���摜�̃Z�b�g
	void ChangeSmallModeFlag(bool);; //�������\���t���O�ύX
private:
	void DrawNormalMode();
	void DrawSmallMode();

	std::weak_ptr<Player> _player;
	double _hp_ratio; //HP�̊���
	double _smash_gauge; //�K�E�Q�[�W�̊���
	double _level_up; //���x���A�b�v�Q�[�W�̊���
	Vector2D<int> _position;
	int _round_gauge_gh; //�~�`�Q�[�W�p�摜�n���h��
	int _exp_gauge_board_gh;
	int _meter_frame_gh;
	int _face_icon_gh; //��A�C�R���摜�n���h��
	int _level;

	int _level_string_fh;

	Vector2D<int> _smash_meter_size; //�K�E���[�^�[�̃T�C�Y
	Vector2D<int> _smash_meter_pos_on_g; //�摜��̕K�E���[�^�[�̈ʒu
	Vector2D<int> _hp_meter_size; //HP���[�^�[�̃T�C�Y
	Vector2D<int> _hp_meter_pos_on_g; //�摜���HP���[�^�[�̈ʒu
	Vector2D<int> _exp_gauge_pos_on_g; //�o���l���[�^�̉摜��̒��S�ʒu

	bool _small_mode; //�������\�����邩

	int calcMeterInfo();
};
