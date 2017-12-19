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
	//�ψʃf�[�^
	struct DisData{
		DisData();
		double gap; //�ψ�
		double velocity; //���x
	};
	std::vector<DisData> _dis_data; //�ψʃf�[�^
	struct Wave{
		Wave();
		double s_t_rad; //�g�̊J�n�_(���W�A��)
		double cnt; //�g����������Ă���̃t���[����
		double length; //�g�̒���(�s�N�Z��)
		double power; //����
	};
	std::vector<Wave> _wave;
	double _wave_velocity; //�g�̑���(�s�N�Z��/�t���[��)
};

