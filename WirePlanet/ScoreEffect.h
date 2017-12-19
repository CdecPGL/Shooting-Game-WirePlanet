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
	//�������O�ɌĂ�
	void SetDrawScore(int); //�\���X�R�A���Z�b�g
	void SetDrawFrame(int); //�\���t���[�����ԃZ�b�g
	void SetDrawColor(int); //�\���F�Z�b�g
private:
	int draw_score; //�\���X�R�A
	int draw_frame; //�\���t���[����
	int draw_color; //�\���F
	int fh; //�t�H���g�n���h��

	int inc_cnt;
};
