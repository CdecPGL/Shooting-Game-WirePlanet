#pragma once
#include "object.h"
#include"GraphicController2D.h"

/*衝突判定について
LaserはCharacterBaseを継承していないが、衝突判定ではCharacterの一種として扱う
*/

class Laser :
	public Object
{
public:
	Laser();
	virtual ~Laser();
	int Init()override;
	int End()override;
	void Update()override;
	void Draw()override;
	void SetWidth(int);
	void SetDirection(double); //方向設定(度)
	void SetDuration(int); //持続時間設定
	void SetSuspension(int); //発射猶予設定
	void SetGraphicHandle(int, int); //画像ハンドルセット(レーザー、レーザー衝突)
	void SetColor(int, int, int); //色セット
	void SetInversion(bool); //色反転フラグ

	int GetLength()const;
	int GetWidth()const;
private:
	int _width; //幅
	int _length; //長さ
	int _duration; //持続時間
	int _suspension; //発射猶予
	int _laser_ghandle; //画像ハンドル
	int _laser_col_ghandle; //画像ハンドル
	int _lg_width;
	int _lg_height;
	Vector2D<int> _lcg_size;
	int _color;
	int _r, _g, _b;

	int _inc_cnt; //インクリメントカウンター
	int _current_width; //現在の幅
	bool _inversion; //反転フラグ

	void calculate_length(); //惑星を考慮してレーザーの長さを計算
	Vector2D<int> _col_pos_with_planet; //惑星との衝突位置(無いときは0xFFFFFFFF,0)
};
