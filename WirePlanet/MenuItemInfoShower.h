#pragma once

#include"Vector2D.h"
#include"DxLib.h"

#include<unordered_map>

class MenuItemInfoShower
{
public:
	MenuItemInfoShower();
	~MenuItemInfoShower();

	int Init();
	void Update();
	void Draw();

	void ChangeSelectedItem(int);

	void SetPosition(const Vector2D<int>&);
	void SetSize(const Vector2D<int>&);
	void SetAnimationSpeed(double);

	void AddGraphHandle(int, int); //画像ハンドル追加(インデックス、画像ハンドル)

private:
	Vector2D<int> position;
	Vector2D<int> size;
	double circle_rate; //円の大きさ倍率
	int anim_cnt; //アニメーションカウンタ
	double anim_speed; //アニメーションの速さ

	std::unordered_map<int, int> ghandle_list; //画像ハンドルリスト
	int current_index; //現在のインデックス
	int next_index; //変更後のインデックス

	Vector2D<int> _draw_area_size;
};

