#include "GraphicUtils.h"

using namespace std;

int GraphicUtils::CreateGraphFromStringToHandle(const string& str, int col, int fh, int e_col){
	//復元用データの保持
	RECT r;
	GetDrawArea(&r);

	int size;
	GetFontStateToHandle(NULL, &size, NULL, fh);

	size *= 1.5;

	int width = GetDrawStringWidthToHandle(str.c_str(), str.size(), fh);
	int da = MakeScreen(width, size, true);
	SetDrawScreen(da);
	/*if (e_col != 0){
		DrawFillBox(0,0,width,size,e_col);
		}*/
	DrawStringToHandle(0, 0, str.c_str(), col, fh, e_col);
	int out = MakeGraph(width, size);
	GetDrawScreenGraph(0, 0, width, size, out);
	SetDrawScreen(DX_SCREEN_BACK);
	DeleteGraph(da);

	//データの復元
	SetDrawArea(r.left, r.top, r.right, r.bottom);
	return out;
}