#pragma once

#include"DxLib.h"
#include<string>
#include<vector>
#include"Vector2D.h"

class MenuItems
{
public:
	MenuItems();
	~MenuItems();
	void Update();
	void Draw();

	void AddMenuItem(int,const std::string&); //項目追加(インデックス、項目名)
	void ClearMenuItem(); //項目削除


	int ProgressCursor(); //カーソルを進める(変更後のインデックス。)
	int BackCursor(); //カーソルを戻す(変更後のインデックス)

	void SetPosition(const Vector2D<int>&);
	void SetSize(const Vector2D<int>&);
	void SetLineHeight(int);
	void SetMargine(int);
	int SetFont(const std::string&); //フォントセット(フォント名)
	const Vector2D<int>& GetPosition()const;

	int GetCurrentCursorPosition()const; //現在のカーソル位置を取得
private:
	Vector2D<int> position;
	Vector2D < int > size;
	int line_height; //行の高さ
	int margine; //行間の隙間
	int cursor_pos; //カーソルの位置
	double cursor_pos_d; //表示に使うカーソルの位置
	int draw_line; //表示する行数
	struct Item{
		std::string name;
	};
	std::vector<Item> items;
	int fh; //フォントハンドル
};

