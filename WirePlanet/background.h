#ifndef DEF_BACKGROUND_H
#define DEF_BACKGROUND_H

#include <list>
#include <cmath>
#include "define.h"
#include "DxLib.h"
#include<memory>
#include"Vector2D.h"

//�w�i�N���X
class BackGround{
public:
	//�R���X�g���N�^
	BackGround();
	//�f�X�g���N�^
	~BackGround();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�w�i�ύX(�w�i�ԍ�,��ԃt���O)
	void ChangeBackGround(int,bool = true);
	//�w�i�i�߂�
	void ProgressBackGround();
private:
	unsigned long counter; //�t���[���J�E���^�[
	int current_bg_num; //���݂̔w�i�ԍ�
	int _pre_bg_num; //�O�̔w�i�ԍ�
	int _interp_rest_frame; //��Ԃ̎c��t���[��
	int inc_cnt;
	Vector2D<int> _draw_area_size; //�`��G���A�T�C�Y
	void _draw_bg_body(int);

};

#endif