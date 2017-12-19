#include "ScoreEffect.h"

const int SE_SIZE(12);
const int SE_THICK(1);
const int SE_FONT_TYPE(DX_FONTTYPE_EDGE);

ScoreEffect::ScoreEffect() :draw_color(GetColor(255, 255, 255)), draw_frame(60), draw_score(0), inc_cnt(0), fh(CreateFontToHandle(NULL,SE_SIZE,SE_THICK,SE_FONT_TYPE))
{
}


ScoreEffect::~ScoreEffect()
{
	DeleteFontToHandle(fh);
}

int ScoreEffect::Init(){
	return 0;
}

void ScoreEffect::Update(){
	if (inc_cnt >= draw_frame){ DeleteMe(); }
	++inc_cnt;
}

void ScoreEffect::Draw(){
	DrawFormatStringToHandle((int)position.x, (int)position.y, draw_color,fh, "%d", draw_score);
}

void ScoreEffect::SetDrawColor(int c){ draw_color = c; }
void ScoreEffect::SetDrawFrame(int f){ draw_frame = f; }
void ScoreEffect::SetDrawScore(int s){ draw_score = s; }
