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
	Vector2D<double> position; //カメラの位置
	double scale; //拡大率
	Vector2D<int> size;
	double rotate; //回転度(ラジアン)
	MATRIX GetMatrix()const;
	void Update();
};

