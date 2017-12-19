#pragma once
#include "object.h"
#include "Action.h"

enum CharaAtrribtute{ PLAYER, ENEMY, BULLET, ITEM };

class CharacterBase :
	public Object
{
public:
	//コンストラクタ(摩擦,最大HP、敵か,ActionDataの参照)
	CharacterBase(double, int,ObjectBelongingTo, const std::string&);
	virtual ~CharacterBase()override;

	void Update()override = 0;
	virtual void Draw()override;
	void ProcessCollision()override final;
	/*void ResistToManager()override;
	void UnresistFromManager()override;*/

	int GetGHandle()const;
	bool GetReverseFlag()const; //反転フラグ取得
	bool GetCollisionWithFriendFlag()const; //仲間との衝突フラグ取得

	int StartAction(const std::string&); //アクションの開始(-1で失敗)

	//変数取得関数
	int GetHP()const;
	int GetMaxHP()const;

	//変数設定関数
	void SetAttackPowerMagnification(double); //攻撃力倍率設定
	void SetReverse(bool); //飯店の設定

	//アクションサイズの取得
	int GetActionSize()const;
	virtual bool GetCollisionObject(CollisionDatas::CharacterCollisionObject*)const;

	void CopyStatus(const CharacterBase&); //キャラの状態をコピー

	//キャラ属性取得
	CharaAtrribtute GetCharaAttribute()const;

	//CharID
	void SetCharID(int);
	int GetCharID()const;
protected:
	int _current_ghandle; //現在の画像ハンドル
	std::shared_ptr<Action> action; //アクション
	bool reverse; //反転ありか(向き)(0で無し(右向き))
	bool col_with_friend;
	std::vector<HitAreaData*> hitarea_data;
	//キャラ定数
	long HP; //HP
	double _atk_pow_magnification; //攻撃力倍率
	//パラメータ
	int p_HP; //現在のHP

	CharaAtrribtute _attribute; //キャラ属性

	bool _bright_flag; //光るフラグ。
	bool _col_revers_flag; //色反転フラグ
	int _revers_draw_gh; //反転表示用画像ハンドル

	//摩擦処理
	void Friction();

	virtual void ProcessCollisionWithPlanet();
	virtual void ProcessCollisionWithCharacter();

	void DrawHitArea();

	void UpdateHitAreaData(); //衝突判定用データの更新
	void UpdateCurrentGHandle(); //現在の画像ハンドルんお更新
	void CommonUpdateProcess(); //共通の更新処理
private:
	//ID
	int char_ID; //キャラクタID
	bool id_settable;
};

