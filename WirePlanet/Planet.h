#pragma once
#include "object.h"


class Planet :
	public Object
{
public:
	//コンストラクタ(質量、摩擦、半径、分割数)
	Planet(double,double,double,double);
	virtual ~Planet()override;

	virtual int Init()override;

	virtual void Update() override;
	void Draw() override;

	//変数取得関数
	double GetRadius()const;
	int GetSeparation()const;

	virtual double GetDistance(double)const;

	virtual void AddForce(double, double); //力を加える(位置、大きさ(下を生とする))

	double GetMinimamDistance()const; //中心からの距離が最も近い点の距離を取得

	//惑星修復(指定された割合(0-1)だけドットの位置をもとに戻す)
	void RepairPlanet(double);

	void RandomTransform(int); //ランダムに変形(変形幅)(minimumサイズは更新されません)

	//リセット
	virtual void Reset();
protected:
	double radius; //半径
	long separation; //分割数
	double current_minimam_distance; //現在の中心距離最小距離
	virtual void DrawBody()const; //本体描画

	struct DotData{
		DotData();
		double gap;

	};
	std::vector<DotData> dot_data;
};

