//http://dixq.net/forum/viewtopic.php?f=3&t=14988

#include "Camera2D.h"

Camera2D::Camera2D(const Vector2D<int>& s) :size(s), position(s/2), rotate(0), scale(1)
{
}


Camera2D::~Camera2D()
{
}

void Camera2D::Reset(){
	position = size / 2;
	rotate = 0;
	scale = 1;
}

void Camera2D::Update(){
	SetupCamera_Ortho((float)size.y / (float)scale);
//	SetCameraPositionAndTargetAndUpVec(VGet((float)position.x, (float)position.y, 100.0f), VGet((float)position.x, (float)position.y, -100.0f), VGet(-cos((float)rotate - 3.1415 / 2), sin((float)rotate - 3.1415 / 2), 0.0f));
	SetCameraPositionAndTargetAndUpVec(VGet((float)position.x, (float)position.y, 100.0f), VGet((float)position.x, (float)position.y, -100.0f), VGet(cos((float)rotate - DX_PI/2), sin((float)rotate - DX_PI_F/2), 0.0f));
}