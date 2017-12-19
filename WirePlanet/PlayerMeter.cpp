#include "PlayerMeter.h"

#include"player.h"
#include"GraphicManager.h"

using namespace std;

namespace{
	const int LEVEL_STRING_FONT_SIZE(10);
	const char* LEVEL_STRING_FONT_NAME("NULL");
}

PlayerMeter::PlayerMeter() :_hp_ratio(0.0), _smash_gauge(0.0), _level_up(0.0), _level(0), _face_icon_gh(-1), _exp_gauge_board_gh(-1), _meter_frame_gh(-1), _round_gauge_gh(-1), _level_string_fh(-1), _small_mode(false)
{
	calcMeterInfo();
}


PlayerMeter::~PlayerMeter()
{
	DeleteFontToHandle(_level_string_fh);
}

int PlayerMeter::Init(){
	if(_level_string_fh<0)_level_string_fh = CreateFontToHandle(LEVEL_STRING_FONT_NAME, LEVEL_STRING_FONT_SIZE, 1, DX_FONTTYPE_EDGE, -1, 1);
	Update();
	return calcMeterInfo();
}

void PlayerMeter::Update(){
	if (_small_mode){ return; }
	shared_ptr<Player> p = _player.lock();
	if (p){
		_hp_ratio = (double)p->GetHP() / p->GetMaxHP();
		_level_up = 1 - (double)p->GetExpRemainingToNextLevel() / p->GetExpDifferenceToNextLevel();
		_level = p->GetLevel();
	}
}

void PlayerMeter::Draw(){
	if (_small_mode){
		DrawSmallMode();
	}
	else{
		DrawNormalMode();
	}
}

void PlayerMeter::DrawNormalMode(){
	//smash
	Vector2D<int> smash_m_pos = _position + _smash_meter_pos_on_g;
	DrawFillBox(smash_m_pos.x, smash_m_pos.y, (smash_m_pos + _smash_meter_size).x, (smash_m_pos + _smash_meter_size).y, GetColor(0, 0, 0));
	DrawFillBox(smash_m_pos.x, smash_m_pos.y + _smash_meter_size.y*(1.0 - _smash_gauge), (smash_m_pos + _smash_meter_size).x, smash_m_pos.y + _smash_meter_size.y, GetColor(0, 0, 255));
	//hp
	Vector2D<int> hp_m_pos = _position + _hp_meter_pos_on_g;
	DrawFillBox(hp_m_pos.x, hp_m_pos.y, hp_m_pos.x + _hp_meter_size.x, hp_m_pos.y + _hp_meter_size.y, GetColor(0, 0, 0));
	DrawFillBox(hp_m_pos.x, hp_m_pos.y + _hp_meter_size.y*(1.0 - _hp_ratio), hp_m_pos.x + _hp_meter_size.x, hp_m_pos.y + _hp_meter_size.y, GetColor(0, 255, 0));

	DrawGraph(_position.x, _position.y, _meter_frame_gh, true);
	//exp
	Vector2D<int> exp_g_pos = _position + _exp_gauge_pos_on_g;
	DrawRotaGraph(exp_g_pos.x, exp_g_pos.y, 1.0, 0.0, _exp_gauge_board_gh, true);
	DrawRotaGraph(exp_g_pos.x, exp_g_pos.y, 1.0, 0.0, _face_icon_gh, true);
	SetDrawBright(255, 0, 0);
	DrawCircleGauge(exp_g_pos.x, exp_g_pos.y, _level_up * 100, _round_gauge_gh);
	SetDrawBright(255, 255, 255);

	//level
	Vector2D<int> lv_string_pos(exp_g_pos.x - GetDrawStringWidthToHandle("Lv", 2,_level_string_fh) / 2, exp_g_pos.y + 10);
	DrawFormatStringToHandle(lv_string_pos.x, lv_string_pos.y, GetColor(255, 255, 255),_level_string_fh, "Lv");
	Vector2D<int> lvnum_string_pos(exp_g_pos.x - GetDrawFormatStringWidthToHandle(_level_string_fh, "%d", _level) / 2, lv_string_pos.y + LEVEL_STRING_FONT_SIZE);
	DrawFormatStringToHandle(lvnum_string_pos.x, lvnum_string_pos.y, GetColor(255, 255, 255),_level_string_fh, "%d", _level);
}

void PlayerMeter::DrawSmallMode(){
	Vector2D<int> _smash_meter_size(2,50); //必殺メーターのサイズ
	Vector2D<int> _smash_meter_pos_on_g(1,0); //画像上の必殺メーターの位置
	Vector2D<int> _hp_meter_size(4,50); //HPメーターのサイズ
	Vector2D<int> _hp_meter_pos_on_g(3,0); //画像上のHPメーターの位置
	Vector2D<int> _exp_gauge_pos_on_g(4,58); //経験値メータの画像上の中心位置


	Vector2D<int> smash_m_pos = _position + _smash_meter_pos_on_g;
	Vector2D<int> hp_m_pos = _position + _hp_meter_pos_on_g;
	DrawFillBox(smash_m_pos.x - 1, smash_m_pos.y - 1 - 1, smash_m_pos.x + _smash_meter_size.x + _hp_meter_size.x + 1, smash_m_pos.y + _smash_meter_size.y + 1 + 4, GetColor(227, 193, 9));

	//smash
	DrawFillBox(smash_m_pos.x, smash_m_pos.y, (smash_m_pos + _smash_meter_size).x, (smash_m_pos + _smash_meter_size).y, GetColor(0, 0, 0));
	DrawFillBox(smash_m_pos.x, smash_m_pos.y + _smash_meter_size.y*(1.0 - _smash_gauge), (smash_m_pos + _smash_meter_size).x, smash_m_pos.y + _smash_meter_size.y, GetColor(0, 0, 255));

	//hp
	DrawFillBox(hp_m_pos.x, hp_m_pos.y, hp_m_pos.x + _hp_meter_size.x, hp_m_pos.y + _hp_meter_size.y, GetColor(0, 0, 0));
	DrawFillBox(hp_m_pos.x, hp_m_pos.y + _hp_meter_size.y*(1.0 - _hp_ratio), hp_m_pos.x + _hp_meter_size.x, hp_m_pos.y + _hp_meter_size.y, GetColor(0, 255, 0));

//	DrawGraph(_position.x, _position.y, _meter_frame_gh, true);

	//exp
	Vector2D<int> exp_g_pos = _position + _exp_gauge_pos_on_g;
//	DrawRotaGraph(exp_g_pos.x, exp_g_pos.y, 0.5, 0.0, _exp_gauge_board_gh, true);
	DrawRotaGraph(exp_g_pos.x, exp_g_pos.y, 1.0, 0.0, _face_icon_gh, true);
	SetDrawBright(255, 0, 0);
	DrawCircleGauge(exp_g_pos.x, exp_g_pos.y, _level_up * 100, _round_gauge_gh);
	SetDrawBright(255, 255, 255);

	//level
//	Vector2D<int> lv_string_pos(exp_g_pos.x - GetDrawStringWidthToHandle("Lv", 2, _level_string_fh) / 2, exp_g_pos.y + 10);
//	DrawFormatStringToHandle(lv_string_pos.x, lv_string_pos.y, GetColor(255, 255, 255), _level_string_fh, "Lv");
	Vector2D<int> lvnum_string_pos(exp_g_pos.x - GetDrawFormatStringWidthToHandle(_level_string_fh, "%d", _level) / 2, exp_g_pos.y+7);
	DrawFormatStringToHandle(lvnum_string_pos.x, lvnum_string_pos.y, GetColor(255, 255, 255), _level_string_fh, "%d", _level);
}

int PlayerMeter::SetPlayer(shared_ptr<CharacterBase> p){
	auto sp = dynamic_pointer_cast<Player>(p);
	if (sp){
		_player = sp;
		return 0;
	}
	else{
		return -1;
	}
}

void PlayerMeter::SetPosition(const Vector2D<int>& pos){ _position = pos; }

int PlayerMeter::SetRountGauge(const string& fig_name){
	_round_gauge_gh = GraphicManager::GetInstance().GetGraphHandle(fig_name);
	return _round_gauge_gh >= 0 ? 0 : -1;
}

int PlayerMeter::SetFaceIcon(const string& fig_name){
	_face_icon_gh = GraphicManager::GetInstance().GetGraphHandle(fig_name);
	return _face_icon_gh >= 0 ? 0 : -1;
}

int PlayerMeter::SetEXPGaugeBoard(const string& fig_name){
	_exp_gauge_board_gh = GraphicManager::GetInstance().GetGraphHandle(fig_name);
	return _exp_gauge_board_gh >= 0 ? 0 : -1;
}

int PlayerMeter::SetMeterFrame(const string& fig_name){
	_meter_frame_gh = GraphicManager::GetInstance().GetGraphHandle(fig_name);
	return _meter_frame_gh >= 0 ? 0 : -1;
}

void PlayerMeter::ChangeSmallModeFlag(bool sm){ _small_mode = sm; }

int PlayerMeter::calcMeterInfo(){
	if (_meter_frame_gh < 0){return -1; }
	//メーターフレーム画像ハンドルからソフトイメージを作成
	Vector2D<int> mfg_size;
	GetGraphSize(_meter_frame_gh, &mfg_size.x, &mfg_size.y);
	int scrh = MakeScreen(mfg_size.x, mfg_size.y, true);
	int ds_buf = GetDrawScreen();
	SetDrawScreen(scrh);
	DrawGraph(0, 0, _meter_frame_gh, true);
	int sih = MakeARGB8ColorSoftImage(mfg_size.x, mfg_size.y);
	GetDrawScreenSoftImage(0, 0, mfg_size.x, mfg_size.y, sih);
	SetDrawScreen(ds_buf);
	DeleteGraph(scrh);

	//各種情報を調べる(上左1ドットは無視)
	int meter_top = 0;
	int meter_h = 0;
	int smash_m_w=0;
	int smash_m_x=0;
	int hp_m_w = 0;
	int hp_m_x = 0;
	int stage = 0; //情報取得ステージ
	for (int y = 1; y < mfg_size.y; ++y){
		for (int x = 1; x < mfg_size.x; ++x){
			int a;
			GetPixelSoftImage(sih, x, y, nullptr, nullptr, nullptr, &a);
			switch (stage){
				//メーターのトップ位置を求める
			case 0:
				if (a == 0){
					meter_top = y;
					x = mfg_size.x;
					y = mfg_size.y / 2 - 1;
					++stage;
				}
				break;
			//スマッシュメータの幅を求める
			case 1:
				if (a == 0){
					if (smash_m_w == 0){ smash_m_x = x; }
					++smash_m_w;
				}
				else if (smash_m_w > 0){
					++stage;
				}
				break;
				//HPメータの幅を求める
			case 2:
				if (a == 0){
					if (hp_m_w == 0){ hp_m_x = x; }
					++hp_m_w;
				}
				else if (hp_m_w > 0){
					++stage;
				}
				break;
				//メーターの高さを求める
			case 3:
				x = hp_m_x;
				for (int y2 = y + 1; y2 < mfg_size.y; ++y2){
					GetPixelSoftImage(sih, x, y2, nullptr, nullptr, nullptr, &a);
					if (a != 0){
						meter_h = y2 - meter_top;
						break;
					}
				}
				++stage;
				break;
			default:
				x = mfg_size.x;
				y = mfg_size.y;
				break;
			}
		}
	}
	DeleteSoftImage(sih);

	//情報の検証とセット
	_smash_meter_pos_on_g.Set(smash_m_x, meter_top);
	_smash_meter_size.Set(smash_m_w, meter_h);
	_hp_meter_pos_on_g.Set(hp_m_x, meter_top);
	_hp_meter_size.Set(hp_m_w, meter_h);
	_exp_gauge_pos_on_g.Set(mfg_size.x / 2, meter_top + meter_h + 10);
	return 0;
}
