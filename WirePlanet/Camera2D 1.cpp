//http://dixq.net/forum/viewtopic.php?f=3&t=14988

#include "Camera2D.h"

Camera2D::Camera2D() :size(640, 480), position(320, 240), rotate(0),scale(1.0)
{
}


Camera2D::~Camera2D()
{
}

void Camera2D::Update(){
	SetupCamera_Ortho((float)size.y / (float)scale);
	SetCameraPositionAndTargetAndUpVec(VGet((float)position.x, (float)position.y, 100.0f), VGet((float)position.x, (float)position.y, -100.0f), VGet(-cos((float)rotate-3.1415/2), sin((float)rotate-3.1415/2), 0.0f));
//	SetCameraPositionAndTargetAndUpVec(VGet(320, 240, -100.0f), VGet(320, 240, 1.0f), VGet(0.0f, 1.0f, 0.0f));
}

MATRIX Camera2D::GetMatrix()const{
	MATRIX m1;
	VECTOR Pos;
	VECTOR LPos;
	VECTOR Scale;

	/*�x�N�g����2d����R����*/
	Pos.x = position.x;
	Pos.y = position.y;
	Pos.z = -0.0*-1 - 10;

	Scale.x = scale;
	Scale.y = scale;
	Scale.z = 1.0;

//	LPos.x = Lpos.x;
//	LPos.y = Lpos.y;
	LPos.x = position.x;
	LPos.y = position.y;
	LPos.z = 0.0;

	/*��L�����ɍs��𐶐�*/
	m1 = MMult(MGetTranslate(Pos), MGetScale(Scale));//���s�ړ��i-�ʒu�j�~�g��

	m1 = MMult(m1, MGetRotY(3.1415));//����ɉ�]��������

	m1 = MMult(m1, MGetRotZ(rotate));//����ɉ�]��������

//	m1 = MMult(m1, MGetTranslate(LPos));//�������ɉ�ʂ̒��S��������

	return m1;
}
