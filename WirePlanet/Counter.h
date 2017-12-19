#pragma once

#include"DxLib.h"

class Counter
{
public:
	~Counter();
	Counter(const Counter&) = delete;
	static Counter& GetInstance();
	int GetCount()const; //カウントの取得
	void ResetCount(); //カウントを0にリセット
	void SetFPSInterval(int); //FPSの計測間隔を設定
	int GetFPS()const; //fpsの取得
	int GetProcessRate()const; //負荷率の取得
	double GetProcessRateHiprecision(); //負荷率の取得(高精度)
	void StartMeasurePR(); //負荷率測定スタート(これを呼んでおとUpdatedeで合計処理時間が足されるがされる)
	void Update(); //更新
	int GetTimeFromStartBySecond()const; //起動からの経過時間を秒で取得
	unsigned long long GetFrameFromStart()const; //起動からの経過フレームを取得
private:
	Counter();
	int count; //アップデートのたびに+1
	int ptime; //前回のfps更新時のGetNowCount()の値
	int fpsinterval; //fpsの計測間隔(デフォルトでは60count)
	double fps; //FPS
	int standardfps; //基準FPS(デフォルトでは60)
	double processrate; //基準値での負荷率(fpsと一緒に更新)
	int tptime; //前回fpsの計測からの合計処理時間
	int startmprtime; //不可計測開始時の時間(-1で合計処理時間が足されない)
	unsigned long long _cnt_since_start; //起動からのカウント
	int _time_at_start; //起動時刻
};
