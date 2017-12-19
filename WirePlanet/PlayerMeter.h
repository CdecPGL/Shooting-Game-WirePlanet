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
	int SetRountGauge(const std::string&); //円形ゲージの画像セット
	int SetFaceIcon(const std::string&); //画像アイコンをセット
	int SetEXPGaugeBoard(const std::string&); //経験値表示板画像のセット
	int SetMeterFrame(const std::string&); //メーターフレーム画像のセット
	void ChangeSmallModeFlag(bool);; //小さく表示フラグ変更
private:
	void DrawNormalMode();
	void DrawSmallMode();

	std::weak_ptr<Player> _player;
	double _hp_ratio; //HPの割合
	double _smash_gauge; //必殺ゲージの割合
	double _level_up; //レベルアップゲージの割合
	Vector2D<int> _position;
	int _round_gauge_gh; //円形ゲージ用画像ハンドル
	int _exp_gauge_board_gh;
	int _meter_frame_gh;
	int _face_icon_gh; //顔アイコン画像ハンドル
	int _level;

	int _level_string_fh;

	Vector2D<int> _smash_meter_size; //必殺メーターのサイズ
	Vector2D<int> _smash_meter_pos_on_g; //画像上の必殺メーターの位置
	Vector2D<int> _hp_meter_size; //HPメーターのサイズ
	Vector2D<int> _hp_meter_pos_on_g; //画像上のHPメーターの位置
	Vector2D<int> _exp_gauge_pos_on_g; //経験値メータの画像上の中心位置

	bool _small_mode; //小さく表示するか

	int calcMeterInfo();
};
