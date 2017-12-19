#pragma once

#include"DxLib.h"

class Counter
{
public:
	~Counter();
	Counter(const Counter&) = delete;
	static Counter& GetInstance();
	int GetCount()const; //�J�E���g�̎擾
	void ResetCount(); //�J�E���g��0�Ƀ��Z�b�g
	void SetFPSInterval(int); //FPS�̌v���Ԋu��ݒ�
	int GetFPS()const; //fps�̎擾
	int GetProcessRate()const; //���ח��̎擾
	double GetProcessRateHiprecision(); //���ח��̎擾(�����x)
	void StartMeasurePR(); //���ח�����X�^�[�g(������Ă�ł���Updatede�ō��v�������Ԃ�������邪�����)
	void Update(); //�X�V
	int GetTimeFromStartBySecond()const; //�N������̌o�ߎ��Ԃ�b�Ŏ擾
	unsigned long long GetFrameFromStart()const; //�N������̌o�߃t���[�����擾
private:
	Counter();
	int count; //�A�b�v�f�[�g�̂��т�+1
	int ptime; //�O���fps�X�V����GetNowCount()�̒l
	int fpsinterval; //fps�̌v���Ԋu(�f�t�H���g�ł�60count)
	double fps; //FPS
	int standardfps; //�FPS(�f�t�H���g�ł�60)
	double processrate; //��l�ł̕��ח�(fps�ƈꏏ�ɍX�V)
	int tptime; //�O��fps�̌v������̍��v��������
	int startmprtime; //�s�v���J�n���̎���(-1�ō��v�������Ԃ�������Ȃ�)
	unsigned long long _cnt_since_start; //�N������̃J�E���g
	int _time_at_start; //�N������
};
