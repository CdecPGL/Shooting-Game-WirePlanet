#include "Counter.h"

Counter::Counter() :count(0), ptime(0), fpsinterval(60), fps(0), standardfps(60), processrate(0), tptime(0), startmprtime(-1), _cnt_since_start(0), _time_at_start(GetNowCount())
{
}

Counter::~Counter()
{
}

Counter& Counter::GetInstance(){
	static Counter c;
	return c;
}

//カウントのリセット
void Counter::ResetCount(){
	count = 0;
}

//FPSの計測間隔をセット
void Counter::SetFPSInterval(int fi){
	if (fi <= 0){ return; }
	fpsinterval = fi;
}

//FPSんｐ取得
int Counter::GetFPS()const{
	return (int)(fps + 0.5);
}

//負荷率の取得
int Counter::GetProcessRate()const{
	return (int)(processrate + 0.5);
}
double Counter::GetProcessRateHiprecision(){
	return processrate;
}

//カウントの取得
int Counter::GetCount()const{
	return count;
}

//更新
void Counter::Update(){
	int time = GetNowCount();
	++count;
	++_cnt_since_start;
	if (startmprtime != -1){
		tptime += (time - startmprtime);
	}
	if (count%fpsinterval == 0){
		fps = (double)fpsinterval / ((double)time - (double)ptime) * 1000.0;
		if (tptime != 0){
			processrate = (double)tptime / (1000.0 / (double)standardfps * (double)fpsinterval)*100.0;
		}
		else{
			processrate = 0;
		}
		tptime = 0;
		ptime = GetNowCount();
	}
}

//負荷率測定スタート
void Counter::StartMeasurePR(){
	startmprtime = GetNowCount();
}

//起動からの毛尾以下時間取得
int Counter::GetTimeFromStartBySecond()const{
	int i= (GetNowCount() - _time_at_start) / 1000;
	return i;
}

//起動からの経過フレーム取得
unsigned long long Counter::GetFrameFromStart()const{
	return _cnt_since_start;
}
