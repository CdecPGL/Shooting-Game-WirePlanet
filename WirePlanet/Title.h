#pragma once
#include "Scene.h"
#include"Vector2D.h"
#include"Action.h"

class MenuItems;
class Planet;
class BackGround;

class Title :
	public Scene
{
public:
	Title();
	~Title();
	int Init(shared_ptr<DataContainer>)override;
	int End()override;
	void Update()override;
	void Draw()override;
private:
	std::shared_ptr<Planet> planet;
	std::shared_ptr<BackGround> bg;
	int title_font_0;
	int title_font_1;
	int _title_logo_gh;
	Vector2D<int> _draw_area_size;
	std::vector<Action*> _actions;
	void draw_chara();
	int _inc_cntr;
	int _state_cntr;
	std::shared_ptr<MenuItems> menu_items;
	int _start_stage_num;
	int _state; //0:タイトル 1:セレクトメニュー表示
	unsigned int _total_point;
	unsigned int _nmode_high_score;
};

