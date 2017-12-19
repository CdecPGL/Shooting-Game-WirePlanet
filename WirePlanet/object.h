#ifndef DEF_OBJECT_H //二重include防止

#define DEF_OBJECT_H

#include "define.h"
#include <cmath>
#include "DxLib.h"
#include"CollisionDatas.h"
#include<vector>
#include"curve.h"
#include"interpolating.h"
#include<mutex>

class CharacterBase;
class Planet;

/*
惑星に所属してない場合はTR座標はワールド座標と上下反対ナ座標になります
*/

//所属
enum class ObjectBelongingTo{ Neutral, Friend, Enemy, None }; //中立、味方、敵、なし

//Objectクラス
class Object{
public:
	//既定のコンストラクタ
	Object();
	//コンストラクタ(質量,摩擦)
	Object(double, double);
	// コンストラクタ(質量, 摩擦,所属)
	Object(double, double, ObjectBelongingTo);
	//デストラクタ
	virtual ~Object();
	//初期化
	virtual int Init();
	//終了処理
	virtual int End();
	//描画
	virtual void Draw();
	//行動
	virtual void Update() = 0;
	//メッセージ受け取り(成功でtrueを返す)
	virtual bool ReceiveMessage(const std::string&, double = 0);
	//衝突の処理
	virtual void ProcessCollision();
	//マネージャーに登録
	//virtual void ResistToManager();
	////マネージャから削除
	//virtual void UnresistFromManager();

	//変数取得関数
	Vector2D<double> GetPositionXY()const;
	double GetRotation()const;
	double GetT()const{ return t; };
	double GetR()const{ return r; };
	double GetVH()const;
	double GetVR()const;
	double GetM()const{ return m; };
	double GetFriction()const{ return friction; };
	double GetRotationVelocity()const;
	std::shared_ptr<Object> GetParent()const; //親のポインタを返す
	std::shared_ptr<Planet> GetBelongingPlanet()const; //所属惑星のポインタ
	ObjectBelongingTo GetBelongingTo()const; //所属取得(中立、味方、敵)
	std::shared_ptr<CharacterBase> GetTarget()const; //ターゲット取得
	//重力(dependence=trueで機能)
	void Gravity(double = 1.0);

	void SetBelongingPlanet(std::shared_ptr<Planet>); //SetXY,TRより先に所属惑星をセットする
	void SetXY(const Vector2D<double>&);
	void SetTR(double, double);
	void SetVhVr(double, double);
	void SetRotation(double);
	void SetPolarVelocity(double, double); //極座標速度セット(速さ、方向(度))
	void SetRatationVelocity(double); //回転速度をセット(度)
	void SetObjectBelongingTo(ObjectBelongingTo);
	void SetT(double);
	void SetR(double);
	void SetVH(double);
	void SetVR(double);

	//衝突データ追加
	void AddCollisionData(const CollisionDataWithPlanet&);
	void AddCollisionData(const CollisionDataWithCharacter&);

	//描画の回転度を取得
	double GetDrawRotation()const;

	//ターゲットの惑星極座標取得
	double GetTargetT()const;
	double GetTargetR()const;
	double GetTargetDistance()const; //ターゲットとの直線距離取得
	double GetTargetTFromMe()const; //自分からのターゲットの相対Tを取得
	double GetTargetRFromMe()const; //自分からのターゲットの相対Tを取得
	double GetTargetDirectionByDeg()const;
	//ターゲットセット
	void SetTarget(std::shared_ptr<CharacterBase>);

	//自動移動
	bool IsAutoMoving()const; //自動移動中か確認
	void StartLinearMove(const Vector2D<double>&, int); //直線移動開始(目的地、移動フレーム)
	void StartHermiteMove(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, int); //エルミート移動開始(目的地、出発速度、到着速度、移動フレーム)
	void StartBezierMove(const Vector2D<double>&, const std::vector<Vector2D<double>>&, int); //ベジェ移動開始(目的地、制御点リスト、移動フレーム)
protected:
	//惑星極座標からワールド座標へ変換
	void ConvertPTW();
	//ワールド座標から惑星極座標へ変換
	void ConvertWTP();
	//速度を座標に適用する
	void ApplyVelocity();
	Vector2D<double> position; //座標
	Vector2D<double> velocity; //速度
	double rotation; //角度(°)(所属惑星がセットされている場合は画面上のではなく、惑星上での回転なので注意)
	double rota_vel; //回転速度(度)
	double m; //重さ
	double friction; //摩擦(すべりにくさ)

	bool visible; //見えるか

	ObjectBelongingTo _obj_belonging_to; //所属

	//ターゲット
	std::shared_ptr<CharacterBase> target;

	//親
	std::shared_ptr<Object> parent;

	//所属惑星
	std::shared_ptr<Planet> myplanet;
	//惑星基準極座標
	double t, vh; //角度(°),水平速度(px)
	double r, vr; //距離,距離速度

	//衝突判定寛永
	//衝突データリスト
	std::vector<CollisionDataWithPlanet> col_data_p; //衝突惑星データ
	std::vector<CollisionDataWithCharacter> col_data_c; //衝突キャラデータ

	//自動移動を処理する*惑星極座標
	void ProcessAutoMove();

	//	std::recursive_mutex _planet_pointer_mtx; //惑星ポインタ用ミューテックス
	//	std::recursive_mutex _target_pointer_mtx; //ターゲットポインタ用ミューテックス
	//	std::recursive_mutex _t_mtx, _r_mtx, _vh_mtx, _vr_mtx;
private:
	class AutoMove{
	public:
		virtual ~AutoMove() = default;
		virtual bool move(Vector2D<double>*) = 0; //移動処理(引数にとったポインタに格納。移動終了でfalseを返す)
	};
	class LinearMove : public AutoMove{
	public:
		LinearMove(const Vector2D<double>&, const Vector2D<double>&, int);
		bool move(Vector2D<double>*)override;
	private:
		otd::interpoliting<Vector2D<double>> _interp;
		otd::interpoliting<Vector2D<double>>::iterator it;
	};
	class HermiteMove : public AutoMove{
	public:
		HermiteMove(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, int);
		bool move(Vector2D<double>*)override;
	private:
		otd::HermiteCurve<Vector2D<double>> _hCurve;
		otd::HermiteCurve<Vector2D<double>>::iterator it;
	};
	class BezierMove : public AutoMove{
	public:
		BezierMove(const Vector2D<double>&, const Vector2D<double>&, const std::vector<Vector2D<double>>&, int);
		bool move(Vector2D<double>*)override;
	private:
		otd::BezierCurve<Vector2D<double>> _bCurve;
		otd::BezierCurve<Vector2D<double>>::iterator it;
	};
	std::unique_ptr<AutoMove> _auto_move;
};

#endif