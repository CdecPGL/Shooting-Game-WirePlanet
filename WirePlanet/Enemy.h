#ifndef DEF_ENEMY_H
#define DEF_ENEMY_H

#include "character.h"

class EnemyMoverManager;

class Enemy :public Character{
public:
	//コンストラクタ(体力、摩擦、無敵時間、ドロップエナジー、ムーバID、Actionファイル名)
	Enemy(int,double,int,int,const std::string&,const std::string&);
	//デストラクタ
	~Enemy(void)override;

	int Init()override;

	//衝突処理
	virtual void ProcessCollisionWithCharacter()override;

	//ムーバ用
	int StartReverseAction(const std::string&); //アクションの逆再生
	bool CheckActionPlaying()const; //アクション再生中か調べる
	bool GetGroundFrag()const;
	void AddDefaultGravity();
	void ProcessFriction(); //摩擦の処理
	//ターゲット(プレイヤー)との距離取得
	double GetTargetDistance()const;
	double GetTargetDistanceT()const;
	double GetTargetDistanceR()const;
	void AddForceToPlanet(double,double); //惑星に力を加える
	int GetBelongingPlanetRadius()const; //所属惑星の半径取得(なかったら-1を返す)
	void Landed(); //上陸した
	double GetHPPercentage()const;
	//ムーバーが作成したオブジェクトの取得。(オブジェクトID)
	Object* GetMCObject(int);
	//オブジェクトの終了
	int EndMCObject(int);
	//オブジェクト作成系関数
	int FireBullet(const std::string&, double, double, double); //弾発射(初期化ID、方向、速度、攻撃力)
	int FireRotatingBullet(const std::string&, double, double, double, double); //回転弾発射(初期化ID、方向、速度、攻撃力、回転速度)
	int FireLaser(int, int, int, int, int, int, double); //レーザー発射(太さ、遅延、発車時間、r,g,b,方向(度))(仮実装)
	int CreateCharacter(const std::string&);

private:
	//動き
	void move();
	//変数
	double distance_toplayer; //プレイヤーとの距離
	EnemyMoverManager& emm;
	int _mvr_id;
	std::string mvr_name;
	int mvr_state; //ムーバの状態(0:初回実行 1:普通実行 2:終了)
	bool _land_flag; //上陸フラグ
	int _drop_energy;

	int _explosion_cntr; //爆発カウンター
	
	std::unordered_map<int ,std::pair<bool,std::weak_ptr<Object>>> _mvr_created_objects; //map<id,<死んだ時に終了処理を行うか,オブジェクト>>
	int _mvr_crt_obj_cnt;
	int setMvrCrtObj(std::weak_ptr<Object>,bool); //オブジェクト、死んだ時に終了処理を行うか
	void endAllMCObject(); //ムーバーが作成したオブジェクトをすべて終了する(End()よ呼び出す)

	void ReceivedAttack(int)override;

	//状態ごとの処理関数
	void NormalState()override;
	void DieState()override;
};

#endif