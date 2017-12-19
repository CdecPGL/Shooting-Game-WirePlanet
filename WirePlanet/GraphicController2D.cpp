#include "GraphicController2D.h"
#include"boost/lexical_cast.hpp"
#include"InitDatas.h"

int gc2d::Init(){
	return 0;
}

void gc2d::UpdateCamera(){
	GetCamera2D().Update();
}

int gc2d::DrawRotaGraph(int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag ){
	Camera2D& camera2d(GetCamera2D());
	int size_x(0), size_y(0);
	GetGraphSize(GrHandle, &size_x, &size_y);
	return DrawBillboard3D(VGet((float)x, (float)y, 0), 0.5f, 0.5f, (float)size_x*ExRate, (float)(-Angle + camera2d.rotate), GrHandle, TransFlag, TurnFlag);
}

int gc2d::DrawRotaGraphF(float x, float y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag){
	Camera2D& camera2d(GetCamera2D());
	int size_x(0), size_y(0);
	GetGraphSize(GrHandle, &size_x, &size_y);
	return DrawBillboard3D(VGet(x, y, 0), 0.5f, 0.5f, (float)size_x*ExRate, (float)(-Angle + camera2d.rotate), GrHandle, TransFlag, TurnFlag);
}

int gc2d::DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag){
	Camera2D& camera2d(GetCamera2D());
	int size_x, size_y;
	GetGraphSize(GrHandle, &size_x, &size_y);
	Vector2D<float> p[4] = { { (float)x - (float)cx *(float)ExtRateX, (float)y - (float)cy * (float)ExtRateY }, { (float)x + (float)(size_x - cx)*(float)ExtRateX, (float)y - (float)cy*(float)ExtRateY }, { (float)x + (float)(size_x - cx)*(float)ExtRateX, (float)y + (float)(size_y - cy)*(float)ExtRateY }, { (float)x - (float)cx*(float)ExtRateX, (float)y + (float)(size_y - cy)*(float)ExtRateY } };
	for (auto& pos : p){
		pos = RotationalTransformation(Angle, pos - Vector2D<float>((float)x, (float)y)) + Vector2D<float>((float)x, (float)y);
		pos -= camera2d.position;
		pos = RotationalTransformation(-camera2d.rotate, pos);
	}

	return DrawModiBillboard3D(VGet((float)camera2d.position.x, (float)camera2d.position.y, 0), p[0].x, -p[0].y, p[1].x, -p[1].y, p[2].x, -p[2].y, p[3].x, -p[3].y, GrHandle, TransFlag);
}

int gc2d::DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag){
	Camera2D& camera2d(GetCamera2D());
	Vector2D<float> p[4] = { { (float)x1, (float)y1 }, { (float)x2, (float)y2 }, { (float)x3, (float)y3 }, { (float)x4, (float)y4 } };
	for (auto& pos : p){
		pos -= camera2d.position;
		pos = RotationalTransformation(-camera2d.rotate, pos);

	}
	return DrawModiBillboard3D(VGet((float)camera2d.position.x, (float)camera2d.position.y, 0), p[0].x, -p[0].y, p[1].x, -p[1].y, p[2].x, -p[2].y, p[3].x, -p[3].y, GrHandle, TransFlag);
}

int gc2d::DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag){
	Camera2D& camera2d(GetCamera2D());
	Vector2D<float> p[4] = { {x1,y1}, {x2-1,y2}, {x3-1,y3-1}, {x4,y4-1} };
	for (auto& pos : p){
		pos -= camera2d.position;
		pos = RotationalTransformation(-camera2d.rotate, pos);

	}

//	DrawQuadrangle(p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, p[3].x, p[3].y, GetColor(255, 0, 0), false);
	/*if (p[0].x > p[1].x){
		std::swap(p[0], p[1]);
	}
	if (p[2].x < p[3].x){
		std::swap(p[2], p[3]);
	}
	if (-p[0].y > -p[3].y){
		std::swap(p[0], p[3]);
	}
	if (-p[1].y > -p[2].y){
		std::swap(p[1], p[2]);
	}*/
	return DrawModiBillboard3D(VGet((float)camera2d.position.x, (float)camera2d.position.y, 0), p[0].x, -p[0].y, p[1].x+1, -p[1].y, p[2].x+1, -p[2].y+1, p[3].x, -p[3].y+1, GrHandle, TransFlag);
}

int gc2d::DrawQuadrangle(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int Color, int FillFlag){
	int result(0);
	VECTOR lu = VGet((float)x1, (float)y1, 0);
	VECTOR ru = VGet((float)x2, (float)y2, 0);
	VECTOR rd = VGet((float)x3, (float)y3, 0);
	VECTOR ld = VGet((float)x4, (float)y4, 0);
	if (FillFlag){
		result |= DrawTriangle3D(lu, ru, ld, Color, true);
		result |= DrawTriangle3D(ld, ru, rd, Color, true);
	}
	else{
		result |= DrawLine3D(lu, ru, Color);
		result |= DrawLine3D(ru, rd, Color);
		result |= DrawLine3D(rd, ld, Color);
		result |= DrawLine3D(ld, lu, Color);
	}
	return result;
}

int gc2d::DrawCircle(int x, int y, int r, int Color, int FillFlag , int LineThickness){
//	DrawSphere3D(VGet((float)x,(float)y,0), r, int DivNum, int DifColor, int SpcColor, int FillFlag);
	if (FillFlag){
		return -1;
	}
	else{
		int sep = r * 2;
		for (int i = 0; i < sep; ++i){
			DrawLine3D(VGet((float)x + (float)r*cos(DX_PI*2.0 * ((float)i / sep)), (float)y + (float)r*sin(DX_PI*2.0  * ((float)i / sep)), 0.0), VGet((float)x + (float)r*cos(DX_PI*2.0 * ((float)(i + 1) / sep)), (float)y + (float)r*sin(DX_PI*2.0 * ((float)(i+1) / sep)), 0.0), Color);
		}
	}
	return 0;
}

int gc2d::DrawCircleF(float x, float y, float r, int Color, int FillFlag, int LineThickness){
	//	DrawSphere3D(VGet((float)x,(float)y,0), r, int DivNum, int DifColor, int SpcColor, int FillFlag);
	if (FillFlag){
		return -1;
	}
	else{
		int sep = r * 2;
		for (int i = 0; i < sep; ++i){
			DrawLine3D(VGet(x + r*cos(DX_PI*2.0 * ((float)i / sep)), y + r*sin(DX_PI*2.0  * ((float)i / sep)), 0.0), VGet(x + r*cos(DX_PI*2.0 * ((float)(i + 1) / sep)), y + r*sin(DX_PI*2.0 * ((float)(i + 1) / sep)), 0.0), Color);
		}
	}
	return 0;
}

Vector2D<int> gc2d::GetDrawAreaSize(){
	static Vector2D<int> draw_area_size(-1, -1);
	if (draw_area_size.x < 0 || draw_area_size.y < 0){
		draw_area_size.x = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawWidth"));
		draw_area_size.y = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawHeight"));
	}
	return draw_area_size;
}

Camera2D& gc2d::GetCamera2D(){
	static Camera2D camera2d(GetDrawAreaSize());
	return camera2d;
}
