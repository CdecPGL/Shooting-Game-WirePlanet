#pragma once

#include<string>
#include<list>
#include"DxLib.h"
#include "Vector2D.h"
#include"Counter.h"
#include<unordered_map>

/*
DebugInfo
エラー情報など。一度登録したら一定数まだ保持される。
DebugData
デバッグ用に画面に表示するデータDraw()で破棄されるので、毎フレーム登録する。
*/

//シングルトン

using namespace std;

class DebugWindow
{
public:
	~DebugWindow();
	static DebugWindow& GetInstance();

	void Update(); //更新
	void Draw();
	void SetDebugInfo(int, string, string, double); //情報登録(重要度、位置、内容、エラー番号)
	void SetDebugData(Vector2D<int>, string, int); //データ登録(表示位置、表示内容、色(GetColor()))
	void AddExecutionTime(const std::string&, int); //実行時間追加
	void ChangeMaxBuffer(int);
	void ChangeMaxLine(int);
	void ChangeDrawTime(int);
	void ChangeFontSize(int);
private:
	DebugWindow();

	int max_buffer; //最大保存数
	int max_line; //最大表示桁
	int draw_time; //表示時間(フレーム)
	int size; //文字サイズ
	struct DebugInfo{
		DebugInfo(int, string, string, double);
		int level; //重要度
		string location; //場所
		string info; //情報
		double num; //番号
	};
	list<DebugInfo> di; //デバッグ情報
	int di_fh; //デバッグ情報用フォントハンドル

	int counter; //変更からの経過時間

	int GetColorByLevel(int); //重要度の色を取得

	struct DebugData{
		DebugData(Vector2D<int>, string, int);
		Vector2D<int> position; //表示位置
		string data; //データ文字列
		int color; //色(GetColor())
	};
	list<DebugData> dd; //デバッグ用表示文字列
	int dd_ak_fh; //デバッグデータ用・各種データ表示用フォントハンドル

	std::unordered_map<std::string, int> _execution_times; //実行時間
	void DrawPerformanceInfo(); //パフォーマンス情報表示
	void DrawManagerInfo();

	enum class DrawInfo{ None, MngrInnfo, PfmcInfo };
	DrawInfo _draw_info;
	void ChangeDrawInfo();
	bool debug_data_visible; //デバッグデータ表示フラグ
	Counter& cntr;
};

//書式付情報登録
#   define SetFormatDebugInfo( lev,loc,inf,nm, ... ) \
{ \
	TCHAR c[256]; \
	_stprintf_s(c, inf, __VA_ARGS__); \
	DebugWindow::GetInstance().SetDebugInfo(lev, loc, c, nm); \
}

// 書式付データ登録
#   define SetFormatDebugData( x,y,col,dat, ... ) \
{ \
	TCHAR c[256]; \
	_stprintf_s(c, dat, __VA_ARGS__); \
	DebugWindow::GetInstance().SetDebugData(Vector2D<int>(x,y), c, col); \
}