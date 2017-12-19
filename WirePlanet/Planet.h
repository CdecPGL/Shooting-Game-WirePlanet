#pragma once
#include "object.h"


class Planet :
	public Object
{
public:
	//�R���X�g���N�^(���ʁA���C�A���a�A������)
	Planet(double,double,double,double);
	virtual ~Planet()override;

	virtual int Init()override;

	virtual void Update() override;
	void Draw() override;

	//�ϐ��擾�֐�
	double GetRadius()const;
	int GetSeparation()const;

	virtual double GetDistance(double)const;

	virtual void AddForce(double, double); //�͂�������(�ʒu�A�傫��(���𐶂Ƃ���))

	double GetMinimamDistance()const; //���S����̋������ł��߂��_�̋������擾

	//�f���C��(�w�肳�ꂽ����(0-1)�����h�b�g�̈ʒu�����Ƃɖ߂�)
	void RepairPlanet(double);

	void RandomTransform(int); //�����_���ɕό`(�ό`��)(minimum�T�C�Y�͍X�V����܂���)

	//���Z�b�g
	virtual void Reset();
protected:
	double radius; //���a
	long separation; //������
	double current_minimam_distance; //���݂̒��S�����ŏ�����
	virtual void DrawBody()const; //�{�̕`��

	struct DotData{
		DotData();
		double gap;

	};
	std::vector<DotData> dot_data;
};

