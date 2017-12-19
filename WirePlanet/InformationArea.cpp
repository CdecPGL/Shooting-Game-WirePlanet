#include "InformationArea.h"
#include"character.h"
#include"PlayerMeter.h"
#include<algorithm>
#include"GPIUtil.h"
#include"GraphicController2D.h"

using namespace std;

namespace{
	Vector2D<int> NORMAL_METER_POSITION(16,150);
	Vector2D<int> SMALL_METER_POSITION(4, 150);
	int SMALL_METER_GAP(70);

	const string PLAYERB_FACE_ICON_GRAPH("PlayerBFaceIcon");
	const string PLAYERG_FACE_ICON_GRAPH("PlayerGFaceIcon");
	const string PLAYER_METER_EXP_GAUGE_BOARD_GRAPH("EXPGaugeBoard");
	const string PLAYER_METER_FRAME_GRAPH("PlayerMeterFrame");
	const string PLAYER_METER_ROUND_GAUGE("RoundGauge");

	const string DEFAULTFONT_NAME("メイリオ");
	const int DEFAULTFONT_SIZE(16);
}

InformationArea::InformationArea() :left_area_width(80), right_area_width(80), usg_player(make_pair("NULL", nullptr)), _point(0), _default_fh(CreateFontToHandle(DEFAULTFONT_NAME.c_str(), DEFAULTFONT_SIZE, -1, DX_FONTTYPE_ANTIALIASING_EDGE)), _hight_score(0), _draw_area_size(gc2d::GetDrawAreaSize())
{
}


InformationArea::~InformationArea()
{
	DeleteFontToHandle(_default_fh);
}

int InformationArea::Init(){
	if(_default_fh<0)_default_fh = CreateFontToHandle(DEFAULTFONT_NAME.c_str(), DEFAULTFONT_SIZE, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	_hight_score = GPIUtil::GetHightScore();
	return 0;
}

void InformationArea::Update(){
	if (usg_player.second){
		usg_player.second->Update();
	}
}

void InformationArea::Draw(){
	//プレイヤーメーターの表示
	for (auto it = _player_proc_list.rbegin(); it != _player_proc_list.rend(); ++it){
		(*it)->meter->Draw();
	}

	//スコア表示
	DrawFormatStringFToHandle(4, 4, GetColor(255, 255, 255), _default_fh, "SCORE:%d", (int)_point);
	//ハイスコア表示
	if (_hight_score < _point){ _hight_score = (unsigned int)_point; }
	DrawFormatStringFToHandle(_draw_area_size.x-GetDrawFormatStringWidthToHandle(_default_fh,"HIGH SCORE:%d",(int)_hight_score)-4, 4, GetColor(255, 255, 255), _default_fh, "HIGH SCORE:%d", (int)_hight_score);
}

//プレイヤー交代
int InformationArea::ChangePlayer(const std::string& id){
	if (id == usg_player.first){ return 0; }
	auto it = _players.find(id);
	if (it == _players.end()){ return -1; }
	
	if (usg_player.second){
		usg_player.second->meter->ChangeSmallModeFlag(true);
		usg_player.second->meter->SetPosition(SMALL_METER_POSITION);
	}
	usg_player.first = id;
	usg_player.second = (*it).second.get();
	usg_player.second->meter->ChangeSmallModeFlag(false);
	usg_player.second->meter->SetPosition(NORMAL_METER_POSITION);


	//交代したプレーヤーをプレイヤー処理リストの先頭に配置する
	auto lit = find(_player_proc_list.begin(), _player_proc_list.end(), (*it).second.get());
	_player_proc_list.erase(lit);
	_player_proc_list.push_front((*it).second.get());
	set_player_meter_position();

	return 0;
}

void InformationArea::SetLeftAreaWidth(int w){ left_area_width = w; }
void InformationArea::SetRightAreaWidth(int w){ right_area_width = w; }


int InformationArea::ResistPlayer(const std::string& id, std::shared_ptr<CharacterBase> ptr, const std::string& icon){
	if (_players.find(id) != _players.end()){ return -1; }
	unique_ptr<PlayerInfo> pi = make_unique<PlayerInfo>();
	pi->meter->SetPlayer(ptr);
	pi->meter->SetFaceIcon(icon);
	pi->meter->SetEXPGaugeBoard(PLAYER_METER_EXP_GAUGE_BOARD_GRAPH);
	pi->meter->SetMeterFrame(PLAYER_METER_FRAME_GRAPH);
	pi->meter->SetRountGauge(PLAYER_METER_ROUND_GAUGE);
	if (_players.size() == 0){ pi->meter->ChangeSmallModeFlag(false); }
	else{ pi->meter->ChangeSmallModeFlag(true); }
	if (pi->meter->Init() < 0){ return -2; }
	pi->player = ptr;
	pi->Update();
	_player_proc_list.push_back(pi.get());
	_players.insert(make_pair(id, move(pi)));
	set_player_meter_position();
	return 0;
}

int InformationArea::UnesistPlayer(const std::string& id){
	auto it = _players.find(id);
	if (it == _players.end()){ return -1; }
	auto lit = find(_player_proc_list.begin(), _player_proc_list.end(), (*it).second.get());
	_player_proc_list.erase(lit);
	_players.erase(it);
	set_player_meter_position();
	return 0;
}

void InformationArea::SetPoint(unsigned long long int p){ _point = p; }

//PlayerInfo
InformationArea::PlayerInfo::PlayerInfo() : meter(make_unique<PlayerMeter>()){
}
InformationArea::PlayerInfo::PlayerInfo(shared_ptr<CharacterBase> p) : player(p),meter(make_unique<PlayerMeter>()){
	meter->SetPlayer(p);
	Update();
}

void InformationArea::PlayerInfo::Update(){
	if (player == nullptr){ return; }
	meter->Update();
}

void InformationArea::set_player_meter_position(){
	int i = 0;
	for (auto it = _player_proc_list.begin(); it != _player_proc_list.end(); ++it){
		if (it == _player_proc_list.begin()){
			(*it)->meter->SetPosition(NORMAL_METER_POSITION);
		}
		else{
			(*it)->meter->SetPosition(SMALL_METER_POSITION + Vector2D<int>(0, SMALL_METER_GAP*i));
			++i;
		}
	}
}
