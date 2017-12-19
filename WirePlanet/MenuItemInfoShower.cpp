#include "MenuItemInfoShower.h"
#include"boost/lexical_cast.hpp"
#include"InitDatas.h"

using namespace std;

const double ANIM_LATENCE(10.0);

MenuItemInfoShower::MenuItemInfoShower() :circle_rate(0.0), anim_cnt(-1), anim_speed(1.0), current_index(-1)
{
}


MenuItemInfoShower::~MenuItemInfoShower()
{
}

int MenuItemInfoShower::Init(){
	_draw_area_size.x = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawWidth"));
	_draw_area_size.y = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawHeight"));
	return 0;
}

void MenuItemInfoShower::Update(){
	if (anim_cnt >= 0){
		++anim_cnt;
		if (((double)anim_cnt / ANIM_LATENCE*anim_speed) > DX_PI / 2){ current_index = next_index; }
		if (((double)anim_cnt/ANIM_LATENCE*anim_speed) > DX_PI){ anim_cnt = -1; };
	}
}

void MenuItemInfoShower::Draw(){
	//Zバッファを有効化
	SetUseZBufferFlag(true);
	Vector2D<int> draw_center_pos(position + size / 2);
	Vector2D<int> radius = size * abs(cos((double)anim_cnt / ANIM_LATENCE * anim_speed)) / 2.0;
	//画面全体を描画不可にする
	DrawBoxToZBuffer(0, 0, _draw_area_size.x, _draw_area_size.y, true, DX_ZWRITE_MASK);
	//指定の円内部を描画可能にする
	DrawCircleToZBuffer(draw_center_pos.x, draw_center_pos.y, radius.x, true, DX_ZWRITE_CLEAR);
	//描画する
	auto it = ghandle_list.find(current_index);
	if (it == ghandle_list.end()){
		DrawCircle(draw_center_pos.x, draw_center_pos.y, radius.x, GetColor(200, 200, 200), true);
		DrawString(draw_center_pos.x - GetDrawStringWidth("No image", 8) / 2, draw_center_pos.y - 8 / 2, "No image", GetColor(0, 0, 0));
	}
	else{
		int gw, gh;
		GetGraphSize((*it).second, &gw, &gh);
		double ext = gw > gh ? (double)size.x / gh : (double)size.x / gw;
		DrawRotaGraph2(draw_center_pos.x, draw_center_pos.y, gw / 2, gh / 2, ext, 0.0, (*it).second, true, false);
	}
	//Zバッファ無効か
	SetUseZBufferFlag(false);

	DrawCircle(draw_center_pos.x, draw_center_pos.y, radius.x,  GetColor(0, 0, 0), false);
//	DrawLineBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(255, 0, 0));
}

void MenuItemInfoShower::ChangeSelectedItem(int idx){
	if (current_index == idx){ return; }
	anim_cnt = 0;
	next_index = idx;
}

void MenuItemInfoShower::SetPosition(const Vector2D<int>& p){ position = p; }

void MenuItemInfoShower::SetSize(const Vector2D<int>& s){ size = s; }

void MenuItemInfoShower::SetAnimationSpeed(double as){ anim_speed = as; }

void MenuItemInfoShower::AddGraphHandle(int idx, int gh){
	if (gh == -1){ return; }
	ghandle_list.insert(unordered_map<int, int>::value_type(idx, gh));
}
