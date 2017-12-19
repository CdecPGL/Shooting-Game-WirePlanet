#pragma once

#include"DxLib.h"
#include"Matrix.h"

class Camera2D
{
public:
	Camera2D(const Vector2D<int>&);
	Camera2D(const Camera2D&) = delete;
	const Camera2D& operator=(const Camera2D&) = delete;
	void Reset();
	~Camera2D();
	Vector2D<double> position; //�J�����̈ʒu
	double scale; //�g�嗦
	Vector2D<int> size;
	double rotate; //��]�x(���W�A��)
	MATRIX GetMatrix()const;
	void Update();
};

