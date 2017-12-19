#include "MenuItems.h"

using namespace std;

MenuItems::MenuItems() :cursor_pos(0), fh(CreateFontToHandle("ÉÅÉCÉäÉI", -1, -1,DX_FONTTYPE_ANTIALIASING)), line_height(16), draw_line(0), margine(0), cursor_pos_d(0.0)
{
}


MenuItems::~MenuItems()
{
	DeleteFontToHandle(fh);
}

void MenuItems::Update(){
	cursor_pos_d = cursor_pos;
	/*if (cursor_pos < cursor_pos_d){
		cursor_pos_d -= (1.0 / (double)line_height) / 5.0;
		if (cursor_pos > cursor_pos_d){ cursor_pos_d = cursor_pos; }
	}
	else if (cursor_pos > cursor_pos_d){
		cursor_pos_d += (1.0 / (double)line_height) / 5.0;
		if (cursor_pos < cursor_pos_d){ cursor_pos_d = cursor_pos; }
	}*/
}

void MenuItems::Draw(){
	int center_draw_pos_y = position.y + size.y / 2 - line_height / 2;
	int center_i = (draw_line - 1) / 2;
	int cursor_width = GetDrawStringWidthToHandle("ÅÀ",1,fh);
	for (int i = 0; i < draw_line; ++i){
		int idx = (int)cursor_pos_d - (draw_line - 1) / 2 + i;
		while (idx < 0){ idx += items.size(); }
		if (idx >= (int)items.size()){ idx = idx%items.size(); }
		Vector2D<int> draw_pos(position.x + cursor_width, center_draw_pos_y + (i - center_i)*(line_height + margine) + (int)((cursor_pos - cursor_pos_d) * (line_height + margine)));
		DrawStringToHandle(draw_pos.x, draw_pos.y, items[idx].name.c_str(), GetColor(255, 255, 255), fh);
	}
	DrawStringToHandle(position.x, center_draw_pos_y, "ÅÀ", GetColor(255, 255, 255), fh);
//	DrawLineBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(255, 0, 0));
//	DrawFormatString(0, 100, GetColor(255, 255, 255), "%d,%lf", cursor_pos, cursor_pos_d);
}

void MenuItems::AddMenuItem(int idx,const string& n){
	Item item;
	item.name = n;
	if (idx < (int)items.size()){ items.insert(items.begin() + idx, item); }
	else{ items.push_back(item); }
}

void MenuItems::ClearMenuItem(){
	items.clear();
}

int MenuItems::ProgressCursor(){
//	if (cursor_pos != cursor_pos_d){ return cursor_pos; }
	++cursor_pos;
	if (cursor_pos >= (int)items.size()){ cursor_pos = 0; }
	return cursor_pos;
}

int MenuItems::BackCursor(){
//	if (cursor_pos != cursor_pos_d){ return cursor_pos;; }
	--cursor_pos;
	if (cursor_pos < 0){ cursor_pos = (int)items.size() - 1; }
	return cursor_pos;
}

void MenuItems::SetPosition(const Vector2D<int>& p){ position = p; }

void MenuItems::SetSize(const Vector2D<int>& s){
	size = s;
	draw_line = size.y / (line_height + margine);
	if (draw_line % 2 == 0){ --draw_line; }
}

void MenuItems::SetLineHeight(int lh){ 
	line_height = lh; 
	char fn[64];
	GetFontStateToHandle(fn,NULL,NULL,fh);
	DeleteFontToHandle(fh);
	fh = CreateFontToHandle(fn, line_height, -1,DX_FONTTYPE_ANTIALIASING);
}

void MenuItems::SetMargine(int m){ 
	margine = m; 
	draw_line = size.y / (line_height + margine);
	if (draw_line % 2 == 0){ --draw_line; }
}

int MenuItems::SetFont(const string& fn){
	int n_fh = CreateFontToHandle(fn.c_str(), line_height, -1, DX_FONTTYPE_ANTIALIASING);
	if (n_fh == -1){ return -1; }
	DeleteFontToHandle(fh);
	fh = n_fh;
	return 0;
}

const Vector2D<int>& MenuItems::GetPosition()const{ return position; }

int MenuItems::GetCurrentCursorPosition()const{ return cursor_pos; }
