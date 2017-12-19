#include "PlanetG.h"

#include<algorithm>
#include"GraphicController2D.h"
#include"GraphicManager.h"
#include"Animation.h"

/*メモ
現在幅2048のテクスチャを扱っているのでPCによってはテクスチャサイズの制限に引っかかる可能性あり。
中心のテクスチャが無駄になってる
*/

using namespace std;

namespace{
	const int V_SEPARATION(5); //ポリゴン描画時の縦分割数
	const string CORE_ANIM("PlanetCore"); //コアのアニメファイル名
	const string CORE_PROTECTER_ANIM("PlanetCoreProtecter"); //コアプロテクターのアニメファイル名
}

PlanetG::PlanetG(const std::string& ogn, double m_m, double m_f, double m_r, double m_s) : Planet(m_m, m_f, m_r, m_s), _v_separation(V_SEPARATION), _core_anim(make_unique<AnimLib::Animation>()), _core_prtctr_anim(make_unique<AnimLib::Animation>()), _origin_graph_name(ogn)
{
}


PlanetG::~PlanetG()
{
	delete[] _vtxs;
	delete[] _p_indxs;
	if (_texture_ghandle != ghandle_origine){
		DeleteGraph(_texture_ghandle);
	}
}

int PlanetG::Init(){
	if (Planet::Init() < 0){ return -1; }
	if (_v_separation < 1){ return -2; }
	//画像の用意
	ghandle_origine = GraphicManager::GetInstance().GetGraphHandle(_origin_graph_name);
	GetGraphSize(ghandle_origine, &_g_size.x, &_g_size.y);
	//サイズが2の累乗でなかったら画像作成
	if (is2Pow(_g_size.x) == false || is2Pow(_g_size.y) == false){
		int buf_gh = MakeScreen(_g_size.x, _g_size.y, true);
		int cur_draw_scr = GetDrawScreen();
		_t_size.Set(getMin2Pow(_g_size.x), getMin2Pow(_g_size.y));
		_texture_ghandle = MakeGraph(_t_size.x, _t_size.y);
		SetDrawScreen(buf_gh);
		DrawGraph(0, 0, ghandle_origine, true);
		GetDrawScreenGraph(0, 0, _g_size.x, _g_size.y, _texture_ghandle);
		SetDrawScreen(cur_draw_scr);
		DeleteGraph(buf_gh);
	}
	//サイズが2の累乗だったらそのまま使う
	else{
		_texture_ghandle = ghandle_origine;
		_t_size = _g_size;
	}

	//ポリゴンの準備
	_polygon_num = separation * 2 * _v_separation;
	_vertex_num = (_v_separation + 1)*(separation + 1);
	_vtxs = new VERTEX3D[_vertex_num];

	//テクスチャの利用領域
	Vector2D<double> _tex_use_area((double)_g_size.x / _t_size.x, (double)_g_size.y / _t_size.y);
	//画像座標のセットと頂点情報のリセット
	for (int i = 0; i < separation + 1; ++i){ //水平方向は時計回りにセットしていく
		for (int j = 0; j < _v_separation + 1; ++j){ //垂直方向は上から順にセットしていく
			//テクスチャ座標のセット
			_vtxs[(_v_separation + 1)*i + j].u = (float)i / separation * _tex_use_area.x;
			_vtxs[(_v_separation + 1)*i + j].v = (float)j / _v_separation * _tex_use_area.y;
			//その他値のリセット
			_vtxs[(_v_separation + 1)*i + j].norm = VGet(0.0f, 0.0f, -1.0f);
			_vtxs[(_v_separation + 1)*i + j].dif = GetColorU8(255, 255, 255, 255);
			_vtxs[(_v_separation + 1)*i + j].spc = GetColorU8(0, 0, 0, 0);
			_vtxs[(_v_separation + 1)*i + j].su = 0;
			_vtxs[(_v_separation + 1)*i + j].sv = 0;

		}
	}

	//頂点インデックス作成
	_p_indxs = new unsigned short[_polygon_num * 3];
	for (int i = 0; i < separation; ++i){
		for (int j = 0; j < _v_separation; ++j){
			_p_indxs[i*(_v_separation * 6) + j * 6] = i*(_v_separation + 1) + j;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 1] = i*(_v_separation + 1) + j + 1;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 2] = (i + 1)*(_v_separation + 1) + j;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 3] = i*(_v_separation + 1) + j + 1;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 4] = (i + 1)*(_v_separation + 1) + j;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 5] = (i + 1)*(_v_separation + 1) + j + 1;
		}
	}

	//コア
	if (_core_anim->SetAnimationData(CORE_ANIM) < 0){
		printf("PlanetG::PlanetG アニメーションファイル(コア)の取得に失敗しました。\n");
		return -3;
	}
	//コアプロテクタ
	if (_core_prtctr_anim->SetAnimationData(CORE_PROTECTER_ANIM) < 0){
		printf("PlanetG::PlanetG アニメーションファイル(コアプロテクタ)の取得に失敗しました。\n");
		return -4;
	}
	return 0;
}

void PlanetG::Update(){
	_core_prtctr_anim->Update();
	Planet::Update();
}

//本体描画
void PlanetG::DrawBody()const{
	std::vector<DotData>::const_iterator it, nit; //いてれーた,次のドットのいてらーた
	float x0, y0, x1, y1; //左上,右上,左右下の座標
	vector<float> _v_dises; //各頂点の距離の
	//拡張店の距離を求める
	for (int i = 0; i < separation; ++i){
		_v_dises.push_back(radius + dot_data[i].gap);
	}
	_v_dises.push_back(_v_dises[0]);
	float cx, cy; //中心座標;
	cx = static_cast<float>(position.x); cy = static_cast<float>(position.y);

//	for (it = dot_data.begin(); it != dot_data.end(); it++){
//		if (it != dot_data.end() - 1){       //イテレータが最後でなければnitはitの次
//			nit = it + 1;
//		}
//		else{
//			nit = dot_data.begin();         //イテレータが最後だったらnitは最初
//		}
//		double d_rate = 1.0;
//		double dis = radius + (*it).gap, ndis = radius + (*nit).gap;
//		x0 = static_cast<float>(d_rate * dis * cos((double)2 * DX_PI / separation * i) + position.x), y0 = static_cast<float>(d_rate * dis * sin((double)2 * DX_PI / separation * i) + position.y);
//		x1 = static_cast<float>(d_rate * ndis * cos((double)2 * DX_PI / separation * (i + 1)) + position.x), y1 = static_cast<float>(d_rate * ndis * sin((double)2 * DX_PI / separation * (i + 1)) + position.y);
////		SetDrawMode(DX_DRAWMODE_BILINEAR);
////		gc2d::DrawModiGraphF(x0, y0, x1, y1, cx, cy, cx, cy, ghandle[i], false);
////		DrawModiGraphF(x0, y0, x1 + 1, y1, cx + 1, cy + 1, cx, cy + 1, ghandle[i], true);
////		gc2d::DrawModiGraphF(x0, y0, x1, y1, cx, cy, cx, cy, ghandle[i], true);
////		SetDrawMode(DX_DRAWMODE_NEAREST);
//		for (int j = 0; j < _v_separation; ++j){
//			_vtxs[i*(_v_separation + 1)].pos = VGet(x0, y0, 0);
//		}
//		_vtxs[i*(_v_separation + 1) + _v_separation].pos = VGet(cx, cy, 0);
//		++i;
//	}
	//各頂点の座標セット
	for (int i = 0; i < separation + 1; ++i){
		float angle_rad = 2.0 * DX_PI / separation * i; //現在の惑星頂点の位置角度
		for (int j = 0; j < _v_separation; ++j){
			float dis_ratio = 1.0f - (float)j / _v_separation;
			_vtxs[i*(_v_separation + 1) + j].pos = VGet(cx + _v_dises[i] * dis_ratio * cos(angle_rad), cy + _v_dises[i] * dis_ratio * sin(angle_rad), 0);
		}
		//中心座標
		_vtxs[i*(_v_separation + 1) + _v_separation].pos = VGet(cx, cy, 0);
	}
	//表示
	DrawPolygonIndexed3D(_vtxs, _vertex_num, _p_indxs, _polygon_num, ghandle_origine, true);

	//コアとプロテクタの表示
	gc2d::DrawRotaGraphF(position.x, position.y, 1.0, 0.0, _core_anim->GetCurrentGraph(), true);
	gc2d::DrawRotaGraphF(position.x, position.y, 1.0, 0.0, _core_prtctr_anim->GetCurrentGraph(), true);

	return;



	//ワイヤーフレームを表示
	for (int i = 0; i < _polygon_num; ++i){
		unsigned short* p_idx = _p_indxs + i * 3;
		float pos[3][2] = { { _vtxs[p_idx[0]].pos.x, _vtxs[p_idx[0]].pos.y }, { _vtxs[p_idx[1]].pos.x, _vtxs[p_idx[1]].pos.y }, { _vtxs[p_idx[2]].pos.x, _vtxs[p_idx[2]].pos.y } };
		DrawLine3D(VGet(pos[0][0], pos[0][1], 0), VGet(pos[1][0], pos[1][1], 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(pos[1][0], pos[1][1], 0), VGet(pos[2][0], pos[2][1], 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(pos[2][0], pos[2][1], 0), VGet(pos[0][0], pos[0][1], 0), GetColor(255, 0, 0));
	}
}

bool PlanetG::is2Pow(int num){
	int bit = sizeof(num) * 8;
	bool flag=false;
	for (int i = 0; i < bit; ++i,num>>=1){
		if (num & 1){
			if (flag){
				return false;
			}
			else{
				flag = true;
			}
		}
	}
	if (flag){
		return true;
	}
	else{
		return false;
	}
}

int PlanetG::getMin2Pow(int num){
	int out = 1;
	int bit = sizeof(num) * 8;
	for (int i = 0; i < bit && out<num; ++i, out <<= 1){
	}
	return out;
}
