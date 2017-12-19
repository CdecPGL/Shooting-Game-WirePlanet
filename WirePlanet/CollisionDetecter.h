#ifndef COLLISION_DETECTER_H
#define COLLISION_DETECTER_H

#include"CollisionDatas.h"
#include<vector>
#include"RectAngle.h"

class Planet;

namespace CollisionFuncs{
	//円同士。押し出しベクトルはXY座標
	bool CollisionDetectsCsC(const CircleObject&, const CircleObject&, Vector2D<double>* = nullptr);
	//衝突物体の保持者の画像角度を考慮した位置を求める(Character::Drawのコードを流用)
	Vector2D<double> CalculateScreenPositionByColObj(const CollisionDatas::CharacterCollisionObject&, const CircleObject&);
	//キャラ同士
	bool CollisionDetectsCharsChar(const CollisionDatas::CharacterCollisionObject&, CollisionDataWithCharacter*, const CollisionDatas::CharacterCollisionObject&, CollisionDataWithCharacter*);
	//キャラと惑星。押し出しベクトルは惑星極座標TR
	bool CollisionDetectsCharsPlanet(const CollisionDatas::CharacterCollisionObject&, std::shared_ptr<Planet>, CollisionDataWithPlanet* = nullptr);
	//キャラとレーザー
	bool CollisionDetectsCharsLaser(const CollisionDatas::CharacterCollisionObject&, const CollisionDatas::LaserCollisionObject&, CollisionDataWithCharacter* = nullptr);
	//キャラと内矩形(return:0:未衝突 1:衝突)
	int CollisionDetectsCharsRectin(const CollisionDatas::CharacterCollisionObject&, const RectAngle<int>&, CollisionDataWithPlanet* = nullptr);
}

#endif