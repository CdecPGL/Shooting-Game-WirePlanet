#pragma once

#include"Camera2D.h"

namespace gc2d
{
	int Init();
	void UpdateCamera();

	int DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE);
	int DrawRotaGraphF(float x, float y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag = FALSE);
	int DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag);
	int DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag);
	int DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag);
	int DrawQuadrangle(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Color, int FillFlag);
	int DrawCircle(int x, int y, int r, int Color, int FillFlag = TRUE, int LineThickness = 1);
	int DrawCircleF(float x, float y, float r, int Color, int FillFlag = TRUE, int LineThickness = 1);

	Vector2D<int> GetDrawAreaSize();
	Camera2D& GetCamera2D();
};

