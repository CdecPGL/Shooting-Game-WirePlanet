#pragma once
#include "Effect.h"
class ScoreEffect :
	public Effect
{
public:
	ScoreEffect();
	~ScoreEffect();
	int Init()override;
	void Update()override;
	void Draw()override;
	//初期化前に呼ぶ
	void SetDrawScore(int); //表示スコアをセット
	void SetDrawFrame(int); //表示フレーム時間セット
	void SetDrawColor(int); //表示色セット
private:
	int draw_score; //表示スコア
	int draw_frame; //表示フレーム数
	int draw_color; //表示色
	int fh; //フォントハンドル

	int inc_cnt;
};
