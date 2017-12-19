#pragma once
#include"Planet.h"
class WirePlanet : public Planet
{
public:
	WirePlanet(double, double, double, double);
	~WirePlanet()override;
	int Init()override;
	void Update()override;

	virtual void AddForce(double, double)override;
	virtual double GetDistance(double)const override;

	void SetColor(int, int, int);
private:
	void DrawBody()const override;
	int _col;
	//変位データ
	struct DisData{
		DisData();
		double gap; //変位
		double velocity; //速度
	};
	std::vector<DisData> _dis_data; //変位データ
	struct Wave{
		Wave();
		double s_t_rad; //波の開始点(ラジアン)
		double cnt; //波が生成されてからのフレーム数
		double length; //波の長さ(ピクセル)
		double power; //強さ
	};
	std::vector<Wave> _wave;
	double _wave_velocity; //波の速さ(ピクセル/フレーム)
};

