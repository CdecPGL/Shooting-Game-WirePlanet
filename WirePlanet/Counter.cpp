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

//�J�E���g�̃��Z�b�g
void Counter::ResetCount(){
	count = 0;
}

//FPS�̌v���Ԋu���Z�b�g
void Counter::SetFPSInterval(int fi){
	if (fi <= 0){ return; }
	fpsinterval = fi;
}

//FPS�񂐎擾
int Counter::GetFPS()const{
	return (int)(fps + 0.5);
}

//���ח��̎擾
int Counter::GetProcessRate()const{
	return (int)(processrate + 0.5);
}
double Counter::GetProcessRateHiprecision(){
	return processrate;
}

//�J�E���g�̎擾
int Counter::GetCount()const{
	return count;
}

//�X�V
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

//���ח�����X�^�[�g
void Counter::StartMeasurePR(){
	startmprtime = GetNowCount();
}

//�N������̖є��ȉ����Ԏ擾
int Counter::GetTimeFromStartBySecond()const{
	int i= (GetNowCount() - _time_at_start) / 1000;
	return i;
}

//�N������̌o�߃t���[���擾
unsigned long long Counter::GetFrameFromStart()const{
	return _cnt_since_start;
}
