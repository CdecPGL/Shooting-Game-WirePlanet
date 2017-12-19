#include "ViewPlayInformations.h"
#include"SceneManager.h"
#include"GPIUtil.h"
#include"ScreenManager.h"
#include"SoundManager.h"
#include"GPIUtil.h"
#include"background.h"

ViewPlayInformations::ViewPlayInformations() :_play_frame(0), _play_second(0), _cntr(Counter::GetInstance()), _pre_c_play_second(0), _ave_fps(60), bg(make_unique<BackGround>())
{
}

ViewPlayInformations::~ViewPlayInformations()
{
}

int ViewPlayInformations::Init(shared_ptr<DataContainer> dc){
	Scene::Init(dc);
	bg->ChangeBackGround(GPIUtil::GetNormalClearCount() > 0 ? GetRand(6) + 1 : 0, true); //ノーマルクリア済みだったら背景をランダムに
	_play_frame = GPIUtil::GetPlayFrame();
	_play_second = GPIUtil::GetPlaySecond();
	return Scene::Init(dc);
}

int ViewPlayInformations::End(){
	dc->SetData("TitleState", 1);
	return Scene::End();
}

void ViewPlayInformations::Update(){
	if (SoundManager::GetInstance().CheckAssignedMusicPlaying("Information") == 0){
		SoundManager::GetInstance().PlayMusicByID("Information");
	}
	if (key.GetCustomInputPush(CUSTOM_INPUT_B)){
		SceneManager::GetInstance().CreateAndChangeScene("Title");
		SoundManager::GetInstance().PlaySoundByID("Cancel");
	}
	bg->Update();
}

void ViewPlayInformations::Draw(){
	ScreenManager::GetInstance().PushScreen();
	using namespace GPIUtil;
	int line_height(20);
	int left_margin(20);
	int top_margin(20);
	bg->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	int l = 0;
	{
		DateTime dt = GetFirstPlayDateTime();
		DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "最初に起動した日時 : 西暦%04d年%02d月%02d日%02d時%02d分%02d秒", dt.year, dt.month, dt.day, dt.hour, dt.minuits, dt.second);

	}
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "起動回数 : %d 回", GetPlayCount());
	{
		auto c_play_frame = _play_frame + _cntr.GetFrameFromStart();
		auto c_play_second = _play_second + _cntr.GetTimeFromStartBySecond();
		unsigned int h, m, s;
		if (_pre_c_play_second != c_play_second){
			_ave_fps = (double)c_play_frame / c_play_second;
		}
		_pre_c_play_second = c_play_second;
		_calc_hms_from_s(c_play_second, &h, &m, &s);
		DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "合計起動時間 : %d時間%d分%d秒 , %dフレーム",(int)h,(int)m,(int)s, (int)c_play_frame);
		DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "初回起動時からの平均フレームレート : %lf FPS", _ave_fps);
	}
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "ゲームポイント : %d Points", GetGamePoint());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "Normalモードハイスコア : %d", GetHightScore());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "倒した敵の総数 : %d 体", GetDefeatedEnemy());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "発射した弾の総数 : %d 発", GetShootedBullet());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "作成したクローンの総数 : %d 体", GetMadeClone());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "ゲームオーバー回数 : %d 回", GetGameOverCount());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "コンティニュー回数 : %d 回", GetContinueCount());
	{
		int acc = GetAllClearCount();
		if (acc == 0){
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "クリア回数 : %d 回", GetNormalClearCount());
		}
		else{
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "クリア回数 : %d 回", GetNormalClearCount());
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "ノーコンティニュークリア回数 : %d 回！！！", acc);
		}
	}
	{
		int ec = GetErrorCount();
		if (ec > 0){
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "検出可能なエラーによりゲームが停止した回数 : %d 回 "/* ごめんなさいm(_ _)m*/, ec);
		}
	}
	{
		int cc = GetCheatCount();
		if (cc > 0){
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "チート検出回数 : %d 回  チートはやめてね", cc);
		}
	}
	++l;
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "Aキーで戻る");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	ScreenManager::GetInstance().ApplyEffectToTopScreen(ScreenManager::ScreenEffect::Broom, 1025);
}

void ViewPlayInformations::_calc_hms_from_s(unsigned long long s, unsigned int* hb, unsigned int* mb, unsigned int* sb){
	*hb = s / 3600;
	*mb = (s / 60) % 60;
	*sb = s % 60;
}
