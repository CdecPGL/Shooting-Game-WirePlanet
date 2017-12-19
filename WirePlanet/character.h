#ifndef DEF_CHARACTER_H //二重include防止

#define DEF_CHARACTER_H

#include "CharacterBase.h"
#include<memory>

class Planet;

const std::string ANIM_TAG_CHAR_ATTACKED("attacked");
const std::string ANIM_TAG_CHAR_IDOL("idol");

struct CollisionDatas::CharacterCollisionObject;

//キャラの状態(攻撃受け中、通常状態、死んだ)
enum CharacterState{ ATTACKED,  NORMAL,DIE};

//Characterクラス
class Character : public CharacterBase{
public:
	//コンストラクタ(摩擦,HP,敵か,無敵時間,ActionDataの参照)
	Character(double,long,ObjectBelongingTo,long,const std::string&);
	 //デストラクタ
	virtual ~Character()override;

	//変数取得関数

	//CharacterManagerに呼び出される

	//初期化
	virtual int Init();

	void Update()override final;

	virtual bool ReceiveMessage(const std::string&, double)override;

	//接地フラグ変更(trueで接地)
	void ChangeGroundFlag(bool);
	//一時停止フラグ変更
	void ChangePauseFlag(bool);
	//衝突オブジェクトの取得(無かったらfalse)
	bool GetCollisionObject(CollisionDatas::CharacterCollisionObject*)const override;

	//モード(自立、外部からControl)
	enum class Mode{ StandAlone=0,Control=1 };
	void SetMode(Mode);
	void SetMode(int);

	enum Direction { LEFT, RIGHT };
	void ChangeDirection(Direction); //向き変更
protected:
	//モード
	Mode _mode;
	void UpdateInStandAloneMode();
	void UpdateInControlMode();
	//キャラ定数
	long matchless_time; //無敵時間(定数)

	//キャラパラメータ
	bool matchless; //trueで無敵
	long time_from_attacked; //攻撃を受けてからの経過時間

	bool ground_flag; //接地フラグ(1で接地)
	bool pause_flag; //一時停止フラグ
	int hit_stop_cnt; //ヒットストップカウンター

	//攻撃を受けないフラグ(イベント時など)
	bool _muteki;

	//衝突処理
	virtual void ProcessCollisionWithCharacter()override;
	virtual void ProcessCollisionWithPlanet()override;

	//キャラの状態ごとの処理関数
	virtual void NormalState();
	virtual void AttackedState();
	virtual void DieState();

	//状況変更関数
	//攻撃を受けた(ダメージの大きさ)
	virtual void ReceivedAttack(int);
	//通常に復帰
	virtual void ReturnNormal();
	//死んだ
	virtual void Died();
private:

	CharacterState state;
	//状況に関係しない共通処理
	void CommonState();
};

#endif