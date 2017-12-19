#pragma once
#include "object.h"
#include"GraphicController2D.h"

/*�Փ˔���ɂ���
Laser��CharacterBase���p�����Ă��Ȃ����A�Փ˔���ł�Character�̈��Ƃ��Ĉ���
*/

class Laser :
	public Object
{
public:
	Laser();
	virtual ~Laser();
	int Init()override;
	int End()override;
	void Update()override;
	void Draw()override;
	void SetWidth(int);
	void SetDirection(double); //�����ݒ�(�x)
	void SetDuration(int); //�������Ԑݒ�
	void SetSuspension(int); //���˗P�\�ݒ�
	void SetGraphicHandle(int, int); //�摜�n���h���Z�b�g(���[�U�[�A���[�U�[�Փ�)
	void SetColor(int, int, int); //�F�Z�b�g
	void SetInversion(bool); //�F���]�t���O

	int GetLength()const;
	int GetWidth()const;
private:
	int _width; //��
	int _length; //����
	int _duration; //��������
	int _suspension; //���˗P�\
	int _laser_ghandle; //�摜�n���h��
	int _laser_col_ghandle; //�摜�n���h��
	int _lg_width;
	int _lg_height;
	Vector2D<int> _lcg_size;
	int _color;
	int _r, _g, _b;

	int _inc_cnt; //�C���N�������g�J�E���^�[
	int _current_width; //���݂̕�
	bool _inversion; //���]�t���O

	void calculate_length(); //�f�����l�����ă��[�U�[�̒������v�Z
	Vector2D<int> _col_pos_with_planet; //�f���Ƃ̏Փˈʒu(�����Ƃ���0xFFFFFFFF,0)
};
