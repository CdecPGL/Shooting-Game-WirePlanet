#ifndef DEF_BULLET_H
#define DEF_BULLET_H

#include "CharacterBase.h"

class BulletMover;

class Bullet : public CharacterBase{
	friend class BulletMover;
public:
	//コンストラクタ(加算ブレンドモード、ブレンドパラメータ、色(RGBを16進数で表現したもの)、Actionファイル名)
	Bullet(int,int,int,const std::string&);
	//デストラクタ
	virtual ~Bullet() = default;
	//描画
	void Draw()override;
	//更新
	void Update()override final;

	//セット(w,vr)
	void SetVelocity(double,double);
	void SetSuspendFrame(int); //持続フレーム設定(-1でデフォルト値をセット)
protected:
	virtual void move() = 0;
	int _inc_cntr;
	int _suspend_frame; //弾が持続する時間
private:
	int blend_mode;
	int blend_param;
	int col[3]; //色RGB

	//衝突処理
	void ProcessCollisionWithPlanet()override;
	void ProcessCollisionWithCharacter()override;

	static int Convert16ColToRGBColor(int, int*, int*, int*); //色16進数値、R、G、B
};

#endif DEF_BULLET_H