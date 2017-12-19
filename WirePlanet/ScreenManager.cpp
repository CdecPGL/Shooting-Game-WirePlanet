#include "ScreenManager.h"
#include<algorithm>
#include"InitDatas.h"
#include"boost/lexical_cast.hpp"
#include"GraphicController2D.h"

using namespace std;

ScreenManager::ScreenManager() :_current_buf_num(-1), _cmn_ps(make_pair(-1, -1))
{
	printf("ScreenManagerのインスタンスが作成されました。\n");
}


ScreenManager::~ScreenManager()
{
	printf("ScreenManagerのインスタンスが破棄されました。\n");
}

ScreenManager& ScreenManager::GetInstance(){
	static ScreenManager am;
	return am;
}

int ScreenManager::Init(){
	//ゲーム描画サイズ取得
	try{
		_draw_area_size.Set(boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawWidth")), boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawHeight")));
	}
	catch (InitDatas::init_datas_error& e){
		printf("ScreenManager::Init 初期データのセクション[Init]が不足しています。(%s)\n", e.what());
		return -1;
	}
	catch (out_of_range& e){
		printf("ScreenManager::Init 初期データのセクション[DrawWidth or DrawHeight]が不足しています。(%s)\n", e.what());
		return -2;
	}
	catch (boost::bad_lexical_cast& e){
		printf("ScreenManager::Init 初期データの値[DrawWidth or DrawHeight]が不正です。(%s)\n", e.what());
		return -3;
	}
	//スクリーン作成
	int nscr = MakeScreen(_draw_area_size.x, _draw_area_size.y, false);
	if (nscr < 0){
		printf("ScreenManager::Init スクリーンの作成に失敗しました。%s\n", _draw_area_size.ToString().c_str());
		return -4;
	}
	_scr_buf.push_back(nscr);
	//シェーダ関連
	for (int i = 0; i < 4; i++)
		{
		vertex[i].pos = VGet((i % 2)*(float)_draw_area_size.x, (i / 2)*(float)_draw_area_size.y, 0);
			vertex[i].rhw = 1.0f;
			vertex[i].dif = GetColorU8(255, 255, 255, 255);
			vertex[i].spc = GetColorU8(0, 0, 0, 0);
			vertex[i].u = vertex[i].su = (float)(i % 2);
			vertex[i].v = vertex[i].sv = (float)(i / 2);
		}
	printf("ScreenManagerが初期化されました。\n");

//	_cmn_ps = make_pair(LoadPixelShader("black_hole_300.pso"), 0);
	return 0;
}

int ScreenManager::End(){
	SetDrawScreen(DX_SCREEN_BACK);
	for (int sh : _scr_buf){
		DeleteGraph(sh);
	}
	return 0;
}

int ScreenManager::ChangeScreenBufferNum(int num){
	if (num < 1){ return -1; }
	while ((int)_scr_buf.size() != num){
		if ((int)_scr_buf.size() < num){
			int nscr = MakeScreen(_draw_area_size.x, _draw_area_size.y, true);
			if (nscr < 0){
				printf("ScreenManager::ChangeScreenBufferNum スクリーンの作成に失敗しました。%s\n", _draw_area_size.ToString().c_str());
				return -2;
			}
			_scr_buf.push_back(nscr);
		}
		else{
			DeleteGraph(_scr_buf[_scr_buf.size() - 1]);
			_scr_buf.pop_back();
		}
	}
	printf("ScreenManager::ChangeScreenBufferNum スクシーンバッファサイズを変更しました。%d\n", num);
	return 0;
}

int ScreenManager::PushScreen(){
	++_current_buf_num;
	if ((int)_scr_buf.size() <= _current_buf_num){
		if (ChangeScreenBufferNum(_scr_buf.size() + 1) < 0){
			return -1;
		}
	}
	if (SetDrawScreen(_scr_buf[_current_buf_num]) < 0){
		printf("ScreenManager::PushScreen 描画対象画面の設定に失敗しました。%d\n", _scr_buf[_current_buf_num]);
		return -2;
	}
	ClearDrawScreen();
	gc2d::UpdateCamera();
	return 0;
}

void ScreenManager::DrawScreen(){
	if (_current_buf_num >= 0){ SetDrawScreen(_scr_buf[0]); }
	if (_cmn_ps.second == 0){
		ApplyPixcelShader(_scr_buf[0], -1, _cmn_ps.first);
	}
	int cntr = 1;
	for (auto it = _scr_buf.begin() + 1; it != _scr_buf.end() && cntr<=_current_buf_num;++it,++cntr){
		DrawGraph(0, 0, (*it), true);
		if (_cmn_ps.second == cntr){
			ApplyPixcelShader(_scr_buf[0],-1,_cmn_ps.first);
		}
	}
	_current_buf_num = -1;
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, _scr_buf[0],false);
}

void ScreenManager::SetCommonPixShader(int sph, int tmg){
	_cmn_ps = make_pair(sph, tmg);
	cmn_pix_shader_cnst_vals.size = 0;
	
}

void ScreenManager::ResetCommonPixShader(){
	_cmn_ps = make_pair(-1, -1);
	cmn_pix_shader_cnst_vals.size = 0;
}

int ScreenManager::ApplyEffectToTopScreen(ScreenEffect sf,int sgt){
	if (_current_buf_num<0){ return -1; }
	switch (sf){
	case ScreenEffect::Broom:
	{
		const int DOWN_SCALE(2);
		const int GAUSS_RATIO(100);
		int cs = _scr_buf[_current_buf_num];
		int hbs = MakeScreen(_draw_area_size.x, _draw_area_size.y, false);
		int dss = MakeScreen(_draw_area_size.x / DOWN_SCALE, _draw_area_size.y / DOWN_SCALE, false);
		int gs = MakeScreen(_draw_area_size.x / DOWN_SCALE, _draw_area_size.y / DOWN_SCALE, false);
		// 描画結果から高輝度部分のみを抜き出した画像を得る
		GraphFilterBlt(cs, hbs, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, sgt%256, TRUE, GetColor(0, 0, 0), 255);
		// 高輝度部分を８分の１に縮小した画像を得る
		GraphFilterBlt(hbs, dss, DX_GRAPH_FILTER_DOWN_SCALE, DOWN_SCALE);
		// ８分の１に縮小した画像をガウスフィルタでぼかす
		GraphFilterBlt(dss, gs, DX_GRAPH_FILTER_GAUSS, 16, GAUSS_RATIO);
		// 描画モードをバイリニアフィルタリングにする( 拡大したときにドットがぼやけるようにする )
		int dm = GetDrawMode();
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		// 描画ブレンドモードを加算にする
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		// 高輝度部分を縮小してぼかした画像を画面いっぱいに２回描画する( ２回描画するのはより明るくみえるようにするため )
		for (int i = 0; i < sgt / 256 + 1; ++i){
			DrawExtendGraph(0, 0, _draw_area_size.x, _draw_area_size.y, gs, false);
		}
		// 描画ブレンドモードをブレンド無しに戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		// 描画モードを二アレストに戻す
		SetDrawMode(dm);
		DeleteGraph(hbs);
		DeleteGraph(dss);
		DeleteGraph(gs);
	}
			break;
		default:
			return -1;
			break;
	}
	return 0;
}

void ScreenManager::ApplyPixcelShader(int in,int out,int sdr){
	static int ppp = -1;
	if (ppp < 0){ ppp = MakeScreen(_draw_area_size.x, _draw_area_size.y, false); }
	SetDrawScreen(ppp);

	/*FLOAT4* pos;
	int pos_num(4);
	pos = new FLOAT4[pos_num];
	for (int i = 0; i < pos_num; ++i){
		pos[i].x = 0;
		pos[i].y = 0;
	}
	int mx, my;
	GetMousePoint(&mx, &my);
	int cx(360), cy(270);
	int vec_x(mx - cx), vec_y(my - cy);
	for (int i = 0; i < pos_num; ++i){
		float rad = 3.1415 * 2 * ((float)i / pos_num);
		float mvec_x = vec_x*cos(rad) - vec_y*sin(rad);
		float mvec_y = vec_x*sin(rad) + vec_y*cos(rad);
		pos[i].x = cx + mvec_x;
		pos[i].y = cy + mvec_y;
	}*/

	SetUseTextureToShader(0, in);

	//SetPSConstSF(0, (float)_draw_area_size.x);//画面幅
	//SetPSConstSF(1, (float)_draw_area_size.y);//画面高さ
	//SetPSConstSF(2, 1000);//質量
	//SetPSConstSF(3, 1000.0f);//影響半径
	//SetPSConstSF(4, 1.0);//BHフラグ
	//SetPSConstSF(16, pos_num);//位置数
	//SetPSConstFArray(17, pos, pos_num); //位置セット

	void SetCmnPixShaderCnstVals();

	SetUsePixelShader(sdr);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	DrawPrimitive2DToShader(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
//	DrawPolygon2DToShader(vertex, 4);
	SetDrawScreen(in);
	DrawGraph(0,0,ppp,false);

	/*delete[] pos;*/
}

void ScreenManager::SetCmnPixShaderCnstVals(){
	SetPSConstFArray(0, cmn_pix_shader_cnst_vals.vals, cmn_pix_shader_cnst_vals.size);
}
