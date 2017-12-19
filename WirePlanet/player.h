#ifndef DEF_PLAYER_H //二重include防止

#define DEF_PLAYER_H

#include "character.h"
#include "DxLib.h"

#include"SecureObject.h"

//namespace secure_obj{
//	class BitInvertEncrypter;
//	template < typename T, class E = BitInvertEncrypter > 
//	class SecureObject;
//}

class LowStateIndivisualManager;
namespace PlayerInput{
	static const unsigned int UP(1);
	static const unsigned int DOWN(2);
	static const unsigned int LEFT(4);
	static const unsigned int RIGHT(8);
	static const unsigned int A(16);
	static const unsigned int B(32);
	static const unsigned int X(64);
	static const unsigned int Y(128);
	static const unsigned int L(256);
	static const unsigned int R(512);
	enum MultiInput{OR,AND};
}

//Playerクラス
class Player : public Character{
public:
	//コンストラクタ(摩擦,HP,最大移動速度,無敵時間,ActionData名)
	Player(double,long,double,long,const std::string&);
	//デストラクタ
	virtual ~Player()override;
	//初期化
	int Init()override;
	//描画
	void Draw();
	//メッセージ受け取り
	bool ReceiveMessage(const std::string&, double=0)override;

	//経験値取得
	void AddExperiencePoint(int);
	//レベル取得
	int GetLevel()const;
	//経験値取得
	unsigned int GetEXP()const;
	//経験値差取得(つぎのレベルと現在のレベルの)
	int GetExpDifferenceToNextLevel()const;
	//つぎのレベルまでに必要な経験値取得
	int GetExpRemainingToNextLevel()const;
	//入力情報セット
	void SetInput(unsigned int);
	void ClearInput();
protected:
	double maximum_speed; //最大移動速度
	bool bullet_flag; //trueで弾発射済み 接地時にリセット
	secure_obj::SecureObject<int> _total_exp_s; //累計経験値(不正対策済み)
	int _level; //レベル
	int _level_resistance; //レベルの上がりに草

	//レベルに応じたパラメータセット(レベル)
	virtual void SetParameterByLevel(int);

	//レベル計算機(累計経験値、レベルの上がりにくさ)
	static int LevelCalculater(int, int);
	//経験値逆算機(レベル、レベルの上がりにくさ)
	static int ExpCaluculater(int, int);

	//上位状況関数
	void NormalState()override;
	void DieState()override;

	////NormalState下位状態変数
	std::vector<int> ns_vars; //下位状態変更時にリセット
	//NormalState下位状態関数マップセット
	virtual void SetNormalStateLowStateFuncMap();
	virtual void UniversalLowStateProc(); //下位状態共通処理
	//NormalState下位状態管理
	std::unique_ptr < LowStateIndivisualManager > ns_ls_mgr;

	//入力関係
	bool ButtonPush(unsigned int,PlayerInput::MultiInput = PlayerInput::AND)const; //ボタンが押されたか(確認するボタン、複数の場合の処理方法)
	bool ButtonStatus(unsigned int, PlayerInput::MultiInput = PlayerInput::AND)const;  //ボタンの状態(確認するボタン、複数の場合の処理方法)
private:
	unsigned int _input; //入力
	unsigned int _pinput; //前回の入力
	unsigned int _pushed_input; //押下されたボタん
	void ChangeLevel(int); //レベル変更処理
};

#endif