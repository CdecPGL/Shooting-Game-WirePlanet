#include "background.h"
#include"GraphicManager.h"
#include"InitDatas.h"
#include"boost/lexical_cast.hpp"
#include"ScreenManager.h"

using namespace std;

namespace{
	int BG_INTERPOLATE_FRAME(60); //背景の補間フレーム
}

//Backgroundクラス

//コンストラクタ
BackGround::BackGround() :counter(0), inc_cnt(0), current_bg_num(0),_pre_bg_num(0),_interp_rest_frame(0){
	int error(0);
	_draw_area_size.x = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawWidth"));
	_draw_area_size.y = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawHeight"));
}

//デストラクタ
BackGround::~BackGround(){
}

//描画
void BackGround::Draw(){
	if (_interp_rest_frame <= 0){ _draw_bg_body(current_bg_num); }
	else{
		int brb, bgb, bbb;
		GetDrawBright(&brb, &bgb, &bbb);
		int b = 255 * ((double)_interp_rest_frame / BG_INTERPOLATE_FRAME);
		SetDrawBright(b, b, b);
		_draw_bg_body(_pre_bg_num);
		b = 255 - b;
		SetDrawBright(b, b, b);
		_draw_bg_body(current_bg_num);
		SetDrawBright(brb, bgb, bbb);
		--_interp_rest_frame;
	}
}

void BackGround::_draw_bg_body(int bgn){
	switch (bgn){
	case 0: //真っ暗
		DrawBox(0, 0, _draw_area_size.x, _draw_area_size.y, GetColor(0, 0, 0), 1);
		break;
	case 1: //緑
	{
		int intvl = 32;
		for (int i = 0; i < _draw_area_size.x / intvl; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.x / intvl));
			DrawLine(x, 0, x, _draw_area_size.y, GetColor(0, 31, 0));
			x = _draw_area_size.x - x;
			DrawLine(x, 0, x, _draw_area_size.y, GetColor(0, 31, 0));
		}
		for (int i = 0; i < _draw_area_size.y / intvl; ++i){
			int y = (double)_draw_area_size.y / (_draw_area_size.y / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.y / intvl));
			DrawLine(0, y, _draw_area_size.x, y, GetColor(0, 31, 0));
			y = _draw_area_size.y - y;
			DrawLine(0, y, _draw_area_size.x, y, GetColor(0, 31, 0));
		}
	}
		break;
	case 2: //水色
	{
		int intvl = 32;
		for (int i = 0; i < _draw_area_size.x / intvl; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.x / intvl));
			DrawLine(x, 0, x, _draw_area_size.y, GetColor(0, 31, 31));
			x = _draw_area_size.x - x;
			DrawLine(x, 0, x, _draw_area_size.y, GetColor(0, 31, 31));
		}
		int c = (_draw_area_size.x + _draw_area_size.y) / intvl;
		for (int i = 0; i < c; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), c);
			DrawLine(x, 0, 0, x, GetColor(0, 31, 31));
			DrawLine(_draw_area_size.x - x, 0, _draw_area_size.x, x, GetColor(0, 31, 31));
		}
	}
		break;
	case 3: //赤
	{
		int intvl = 32;
		for (int i = 0; i < _draw_area_size.x / intvl; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.x / intvl));
			DrawLine(x, 0, x, _draw_area_size.y, GetColor(31, 0, 0));
			x = _draw_area_size.x - x;
			DrawLine(x, 0, x, _draw_area_size.y, GetColor(31, 0, 0));
		}
		for (int i = 0; i < _draw_area_size.y / intvl; ++i){
			int y = (double)_draw_area_size.y / (_draw_area_size.y / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.y / intvl));
			DrawLine(0, y, _draw_area_size.x, y, GetColor(31, 0, 0));
			y = _draw_area_size.y - y;
			DrawLine(0, y, _draw_area_size.x, y, GetColor(31, 0, 0));
		}
		int c = (_draw_area_size.x + _draw_area_size.y) / intvl;
		for (int i = 0; i < c; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), c);
			DrawLine(x, 0, 0, x, GetColor(31, 0, 0));
			DrawLine(_draw_area_size.x - x, 0, _draw_area_size.x, x, GetColor(31, 0, 0));
			DrawLine(x, _draw_area_size.y, 0, _draw_area_size.y - x, GetColor(31, 0, 0));
			DrawLine(_draw_area_size.x - x, _draw_area_size.y, _draw_area_size.x, _draw_area_size.y - x, GetColor(31, 0, 0));
		}
	}
		break;
	case 4: //紫
	{
		int intvl = 256;
		int intvlr = 64;
		int draw_naname = _draw_area_size.Length();
		for (int i = 0; i <= _draw_area_size.x / intvl; ++i){
			for (int j = 0; j <= _draw_area_size.y / intvl; ++j){
				for (int k = 0; k < draw_naname / intvlr; ++k){
					DrawCircle(intvl*i, intvl*j, intvlr*fmod((double)k + inc_cnt / 600.0, draw_naname / intvlr), GetColor(15, 0, 31), false);
				}
			}
		}
		++inc_cnt;
	}
		break;
	case 5: //ピンク
	{
		int intvl = 64;
		int num = 60;
		for (int i = 0; i < 4; ++i){
			Vector2D<int> pos(_draw_area_size.x / 4 + (i % 2) * _draw_area_size.x / 2, _draw_area_size.y / 4 + (i / 2) * _draw_area_size.y / 2);
			int rota_dir = (i == 1 || i == 2) ? -1 : 1;
//			if (i == 4){ pos = _draw_area_size / 2; }
			for (int j = 0; j < num; ++j){
				double rad = fmod(DX_PI * 2.0 / num * j + (double)inc_cnt / 1000.0 * rota_dir, DX_PI * 2.0);
				DrawLine(pos.x, pos.y, pos.x + 800 * cos(rad), pos.y + 800 * sin(rad), GetColor(31, 0, 15));
			}
		}
		/*for (int i = -1; i < _draw_area_size.x / intvl+1; ++i){
			for (int j = -1; j < _draw_area_size.x / intvl+1; ++j){
			DrawLine(intvl*(fmod(((double)i - inc_cnt / 300.0 + _draw_area_size.x / intvl), _draw_area_size.x / intvl+2)-1), 0, intvl*(fmod(((double)j + inc_cnt / 300.0), _draw_area_size.x / intvl+2)-1), _draw_area_size.y, GetColor(31, 0, 15));
			}
			}*/
	}
		break;
	case 6: //白
	{
		int intvl = 32;
		for (int i = 0; i < _draw_area_size.x / intvl; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.x / intvl));
			DrawLine(x, 0, 0, _draw_area_size.x - x, GetColor(31, 31, 31));
			DrawLine(_draw_area_size.x - x, 0, _draw_area_size.x, _draw_area_size.x - x, GetColor(31, 31, 31));
			DrawLine(x, _draw_area_size.y, 0, _draw_area_size.y - (_draw_area_size.x - x), GetColor(31, 31, 31));
			DrawLine(_draw_area_size.x - x, _draw_area_size.y, _draw_area_size.x, _draw_area_size.y - (_draw_area_size.x - x), GetColor(31, 31, 31));
		}
		++inc_cnt;
	}
		break;
	case 7: //青
	{
		int intvl = 32;
		for (int i = 0; i < _draw_area_size.y / intvl; ++i){
			int y = (double)_draw_area_size.y / (_draw_area_size.y / intvl) * fmod(((double)i + inc_cnt / 300.0), (_draw_area_size.y / intvl));
			//			DrawLine(0, y, _draw_area_size.x, y, GetColor(0, 0, 31));
			y = _draw_area_size.y - y;
			DrawLine(0, y, _draw_area_size.x, y, GetColor(0, 0, 31));
		}
		int c = (_draw_area_size.x + _draw_area_size.y) / intvl;
		for (int i = 0; i < c; ++i){
			int x = (double)_draw_area_size.x / (_draw_area_size.x / intvl) * fmod(((double)i + inc_cnt / 300.0), c);
			DrawLine(x, 0, 0, x * 2, GetColor(0, 0, 31));
			DrawLine(_draw_area_size.x - x, 0, _draw_area_size.x, x * 2, GetColor(0, 0, 31));
		}
	}
		break;
	}
}

//更新
void BackGround::Update(){
	++inc_cnt;
}

void BackGround::ChangeBackGround(int bn,bool intpf){
	if (bn == current_bg_num){ return; }
	_pre_bg_num = current_bg_num;
	current_bg_num = bn;
	if (intpf){ _interp_rest_frame = BG_INTERPOLATE_FRAME; }
}

void BackGround::ProgressBackGround(){
	++current_bg_num;
}