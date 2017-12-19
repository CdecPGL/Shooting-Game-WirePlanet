#ifndef DEF_BACKGROUND_H
#define DEF_BACKGROUND_H

#include <list>
#include <cmath>
#include "define.h"
#include "DxLib.h"
#include<memory>
#include"Vector2D.h"

//背景クラス
class BackGround{
public:
	//コンストラクタ
	BackGround();
	//デストラクタ
	~BackGround();
	//更新
	void Update();
	//描画
	void Draw();
	//背景変更(背景番号,補間フラグ)
	void ChangeBackGround(int,bool = true);
	//背景進める
	void ProgressBackGround();
private:
	unsigned long counter; //フレームカウンター
	int current_bg_num; //現在の背景番号
	int _pre_bg_num; //前の背景番号
	int _interp_rest_frame; //補間の残りフレーム
	int inc_cnt;
	Vector2D<int> _draw_area_size; //描画エリアサイズ
	void _draw_bg_body(int);

};

#endif