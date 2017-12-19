#include "LevelChangeEffect.h"
#include"GraphicUtils.h"
#include"boost/lexical_cast.hpp"
#include"SoundManager.h"

using namespace std;

namespace{
	const string TutorialStageString("Tutorial");
	const string FinalStageString("Final Stage");
	const string GameOverString("Game Over...");
	const string TestStageString("Test Stage");
	const string LevelClearString("Clear!");
	const string LastWaveString("Final Wave");
	const string BossString("Boss");
	const string NormalClearString("Game Clear!");
	const string ReadyString("Ready……?");
	const int AppearanceFrame(30); //登場フレーム数
	const int WaitFrame(30); //待機フレーム数
	const int LeavingFrame(30); //退場フレーム数
}

LevelChangeEffect::LevelChangeEffect() :stage_num(0), f_size(16), fh(CreateFontToHandle(NULL, -1, -1)), gh(-1), cnt(0), separation_y(10), playing(false)
{

}


LevelChangeEffect::~LevelChangeEffect()
{
	if (fh >= 0)DeleteFontToHandle(fh);
	if (gh >= 0)DeleteGraph(gh);
}

int LevelChangeEffect::SetUpEffect(){
	if (gh != -1){ DeleteGraph(gh); }
	string ds = GetEffectString(stage_num);
	gh = GraphicUtils::CreateGraphFromStringToHandle(ds, GetColor(0, 0, 0), fh, GetColor(255, 255, 255));
	GetGraphSize(gh, &g_size.x, &g_size.y);
	cnt = 0;
	return 0;
}

void LevelChangeEffect::Update(){
	if (playing){
		++cnt;
		if (cnt > AppearanceFrame + WaitFrame + LeavingFrame + separation_y){ playing = false; }
	}
}

void LevelChangeEffect::Draw(){
	if (playing){
		int center_x = position.x + size.x/2 - g_size.x/2;
		for (int i = 0; i < separation_y; ++i){
			int cnt_i = (cnt - i < 0) ? cnt : cnt - i;
			Vector2D<int> d_pos;
			if (cnt_i < AppearanceFrame){
				d_pos.Set(center_x - pow((-sin(((double)cnt_i / AppearanceFrame)*DX_PI/2)+1),2)*(size.x) * ((i % 2) * 2 - 1), position.y + g_size.y / separation_y*i);
			}
			else if (cnt_i < AppearanceFrame + WaitFrame){
				d_pos.Set(center_x, position.y + g_size.y / separation_y*i);
			}
			else if (cnt_i < AppearanceFrame + WaitFrame + LeavingFrame){
				d_pos.Set(center_x - pow((-cos((double)(cnt_i- AppearanceFrame - WaitFrame) / LeavingFrame * DX_PI/2)+1),2)*(size.x) * ((i % 2) * 2 - 1), position.y + g_size.y / separation_y*i);
			}
			else{
				continue;
			}
			DrawRectGraph(d_pos.x, d_pos.y, 0, g_size.y / separation_y*i, g_size.x, g_size.y / separation_y, gh, true, false);
		}
	}
}

void LevelChangeEffect::DebugDraw(){
	DrawLineBox(position.x, position.y, position.x + size.x, position.y + size.y, GetColor(255, 0, 0));
}

int LevelChangeEffect::Init(){
	SetUpEffect();
	return 0;
}

void LevelChangeEffect::StartEffect(){
	SoundManager::GetInstance().PlaySoundByID("StartWaveEffect");
	playing = true;
	cnt = 0;
}

void LevelChangeEffect::SetStageNumber(int sn){
	stage_num = sn;
	SetUpEffect();
}

void LevelChangeEffect::SetPosition(const Vector2D<int>& p){ position = p; }

void LevelChangeEffect::SetFontData(const std::string& n, int tk, int tp){
	if (fh >= 0){ DeleteFontToHandle(fh); }
	f_name = n;
	f_thick = tk;
	f_type = tp;
	fh = CreateFontToHandle(f_name.c_str(), f_size, f_thick, f_type);
}

void LevelChangeEffect::SetSize(const Vector2D<int>& s){ 
	if (fh >= 0){ DeleteFontToHandle(fh); }
	size = s;
	f_size = size.y;
	fh = CreateFontToHandle(f_name.c_str(), f_size, f_thick, f_type);
}

string LevelChangeEffect::GetEffectString(int stage_num){
	string out;
	if (stage_num >= 1){
		out = GetOrdin(stage_num) + " Wave";
	}
	else{
		if (stage_num == 0){ out = TutorialStageString; }
		else if (stage_num == -1){ out = FinalStageString; }
		else if (stage_num == -2){ out = GameOverString; }
		else if (stage_num == -3){ out = TestStageString; }
		else if (stage_num == -4){ out = LevelClearString; }
		else if (stage_num == -5){ out = LastWaveString; }
		else if (stage_num == -6){ out = BossString; }
		else if (stage_num == -7){ out = NormalClearString; }
		else if (stage_num == -8){ out = ReadyString; }
	}
	return out;
}

string LevelChangeEffect::GetOrdin(int num){
	string out = boost::lexical_cast<string>(num);
	if (10<num && num<20){
		out += "th";
	}
	else{
		switch (num % 10)
		{
		case 1:
			out += "st";
			break;
		case 2:
			out += "nd";
			break;
		case 3:
			out += "rd";
			break;
		default:
			out += "th";
			break;
		}
	}
	return out;
}

bool LevelChangeEffect::Playing()const{ return playing; }
