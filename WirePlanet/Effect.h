#pragma once
#include"object.h"
class Effect : public Object
{
public:
	//コンストラクタ(ピクセルサイズ幅)
	Effect();
	virtual ~Effect();
	virtual int Init() = 0;
	virtual void Update()override = 0;
	virtual void Draw()override = 0;
	//初期化前に呼ぶ
	void SetWidth(int);

protected:
	int size_w; //ピクセルサイズ幅(0で原寸大又は自動判別)

	void DeleteMe(); //自分を削除
};
