#pragma once

#include"DxLib.h"
#include<vector>
#include"Vector2D.h"

class LevelChangeEffect
{
public:
	LevelChangeEffect();
	~LevelChangeEffect();
	void Update();
	void Draw();
	void DebugDraw();
	int Init();
	
	void StartEffect();

	bool Playing()const;

	//ステージ番号をセットする(1~:通常 0:チュートリアル -1:ファイナルステージ -2:ゲームオーバー -3:テストステージ -4:クリア -5:最終波 -6:ボス -7:通常クリア -8:Ready……?)
	void SetStageNumber(int);
	void SetFontData(const std::string&,int,int); //フォントデータ瀬戸(フォント名、太さ、フォントタイプ)
	void SetPosition(const Vector2D<int>&);
	void SetSize(const Vector2D<int>&);
private:
	Vector2D<int> position;
	Vector2D<int> size;
	int stage_num;
	int fh;
	int f_size;
	std::string f_name;
	int f_thick;
	int f_type;
	int gh;
	int cnt;
	Vector2D<int> g_size;
	int separation_y; //縦分割数
	bool playing; //再生中か

	int SetUpEffect(); //エフェクト生成
	static std::string GetEffectString(int); //エフェクト文字列取得
	static std::string GetOrdin(int); //序数取得
};

