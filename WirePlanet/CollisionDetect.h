#ifndef DEF_COLLISIONDETECT_H
#define DEF_COLLISIONDETECT_H

//Version1.0 2013/12/18
//Version1.1 2014/2/18 静止した円同士の衝突判定CollisionDetectsSsS()を追加
//Version2.0 2014/4/22 NULLをnullptrに変更。円を表す文字をSからCに変更。矩形(ボックス)はBからRに。sCsCで押し出しベクトル取得可能に。
//Version2.1 2014/4/25 関数のベクタ受け取りを値渡しからconst参照渡しに変更。半直線同士の衝突判定追加。

#include"Vector2D.h"
//#include"boost/rational.hpp"

namespace CollisionDetect{
	//今のところは線分の始点から終点を見た時に左側を裏、右側を表としている。
	//裏表処理確認済み:dPsL

	//静止した円と線分の当たり判定
	//引数:円の中心座標、半径、線分の始点、終点(,位置補正値)
	//戻り値:0衝突してない 1衝突した
	/////引数の線分の値が負数の時不具合?
	int CollisionDetectsCsL(const Vector2D<double>&, const double, const Vector2D<double>&, const Vector2D<double>&, Vector2D<double>* = nullptr); //線分の両端の外での位置補正値要検証
	//静止した線分同士の当たり判定
	//引数:線分1の始点終点、線分2の始点終点
	//戻り値:0衝突してない 1表衝突した
	int CollisionDetectsLsL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	//静止した点と静止した線分の位置関係を判定
	//引数:点の位置、線分の始点終点
	//戻り値:線分の始点から終点を見た時に左側を表、右側を裏し、0線分上 1表側 2裏側(要検証 右が表になってる)
	int DetectsPsL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	/**
	* @brief 動く点と静止した線分の当たり判定(位置補正値要検証)
	* @param 点の位置(移動前)、移動ベクトル、線分の始点終点(,押し出しベクトル格納先)
	* @return 0:衝突してない 1:表から衝突した 2:裏から衝突した(要検証)
	*/
	int CollisionDetectdPsL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&,Vector2D<double>* = nullptr);
//	int CollisionDetectdPsL(const Vector2D<int>&, const Vector2D<int>&, const Vector2D<int>&, const Vector2D<int>&, Vector2D<int>* = nullptr);
	/**
	* @brief 動く点と静止した半直線の当たり判定(位置補正値要検証)
	* @param 点の位置(移動前)、移動ベクトル、線分の始点、方向ベクトル(,押し出しベクトル格納先)
	* @return 0:衝突してない 1:表から衝突した 2:裏から衝突した(要検証)
	*/
	int CollisionDetectdPsHL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, Vector2D<double>* = nullptr);
	//動く円と静止した線分の当たり判定
	//引数:円の位置、半径、移動ベクトル、線分の始点終点
	//戻り値:0衝突してない 1表から衝突した 2裏から衝突した(要検証)
	int CollisionDetectdCsL(const Vector2D<double>&, double, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	/**
	*@brief 静止した矩形同士の当たり判定
	*@param 矩形0の位置、サイズ,矩形1の位置、サイズ(位置は左上座標)
	*@return 0衝突してない 1:衝突した
	*/
	int CollisionDetectsRsR(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	/**
	*@brief 動く矩形と静止下矩形の当たり判定
	*@param 動く矩形の位置(移動前)、サイズ、移動ベクトル、静止している矩形の位置、サイズ、押し出しベクトル格納変数
	*@return 0:衝突してない 1:衝突した
	*/
	int CollisionDetectdRsR(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, Vector2D<double>* = nullptr);
	/**
	*@brief 静止した円同士の当たり判定
	*@param 円0の位置、半径,円1の位置、半径
	*@return 0衝突してない 1:衝突した
	*/
	int CollisionDetectsCsC(const Vector2D<double>&, double, const Vector2D<double>&, double,Vector2D<double>* = nullptr);
	/**
	*@brief 静止した半直線同士の衝突判定
	*@param 半直線1の始点、方向ベクトル、半直線2の始点、方向ベクトル
	*@return false:衝突してない true:衝突している
	*/
	bool CollisionDetectsHLsHL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
}

#endif