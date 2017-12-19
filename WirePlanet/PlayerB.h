#pragma once
#include "player.h"
//プレーヤー男

class PlayerB :
	public Player
{
public:
	//コンストラクタ(摩擦,HP,最大移動速度,無敵時間,ActionData名)
	PlayerB(double, long, double, long,const std::string&);
	virtual ~PlayerB()override;
protected:
	void DieState()override;
	//能力パラメータ
	int _shoot_interval; //弾の発射間隔
	int _shoot_speed; //弾の発射速度
	double _shoot_power; //弾の攻撃力
	int _throw_speed; //剣の発射速度
	double _throw_power; //県の攻撃力
	int _h_shoot_suspend; //横ショット弾の持続時間
	int _max_throw_num; //同時に投げられる剣の数
	double _walk_acceralation; //歩くときの加速度
	int _jump_suspend; //ジャンプの持続時間
	double _exp_to_hp_rate; //経験値をHPに変換するときのレート
	double _barrier_exp_consumption; //バリアのEXP消費量
	//現在の状態パラメータ
	int _jumping_frame; //ジャンプをしているフレーム数
	bool _barrier_flag;
	unsigned int _cntr_from_create_clone;
	//パラメータセット関数
	void SetParameterByLevel(int)override;
	//下位状態共通関数
	void UniversalLowStateProc()override;

	void ProcessCollisionWithCharacter()override;

	std::vector<int> throwed_sword_id; //投げた剣のID

	std::vector<std::shared_ptr<Player>> _clones;

	//NormalState下位状態関数マップセット
	void SetNormalStateLowStateFuncMap()override;
	//NormalState下位状態関数
	void NormalLStateUpdate(); //通常
	void NormalLStateEnter();
	void HShootLStateUpdate(); //横弾
	void HShootLStateEnter();
	void BarrierLStateUpdate(); //バリア
	void BarrierLStateEnter();
	void BarrierLStateLeave();
	void SquatLStateEnter(); //しゃがむ
	void SquatLStateUpdate();
	void SquatLStateLeave();
	void LaserLStateEnter(); //レーザー(検討中)
	void CreateCloneLStateUpdate(); //クローン作成
	void CreateCloneLStateEnter(); //クローン作成
	void CreateCloneLStateLeave(); //クローン作成

};

