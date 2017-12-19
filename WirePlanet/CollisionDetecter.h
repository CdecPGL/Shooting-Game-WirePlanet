#ifndef COLLISION_DETECTER_H
#define COLLISION_DETECTER_H

#include"CollisionDatas.h"
#include<vector>
#include"RectAngle.h"

class Planet;

namespace CollisionFuncs{
	//�~���m�B�����o���x�N�g����XY���W
	bool CollisionDetectsCsC(const CircleObject&, const CircleObject&, Vector2D<double>* = nullptr);
	//�Փ˕��̂̕ێ��҂̉摜�p�x���l�������ʒu�����߂�(Character::Draw�̃R�[�h�𗬗p)
	Vector2D<double> CalculateScreenPositionByColObj(const CollisionDatas::CharacterCollisionObject&, const CircleObject&);
	//�L�������m
	bool CollisionDetectsCharsChar(const CollisionDatas::CharacterCollisionObject&, CollisionDataWithCharacter*, const CollisionDatas::CharacterCollisionObject&, CollisionDataWithCharacter*);
	//�L�����Ƙf���B�����o���x�N�g���͘f���ɍ��WTR
	bool CollisionDetectsCharsPlanet(const CollisionDatas::CharacterCollisionObject&, std::shared_ptr<Planet>, CollisionDataWithPlanet* = nullptr);
	//�L�����ƃ��[�U�[
	bool CollisionDetectsCharsLaser(const CollisionDatas::CharacterCollisionObject&, const CollisionDatas::LaserCollisionObject&, CollisionDataWithCharacter* = nullptr);
	//�L�����Ɠ���`(return:0:���Փ� 1:�Փ�)
	int CollisionDetectsCharsRectin(const CollisionDatas::CharacterCollisionObject&, const RectAngle<int>&, CollisionDataWithPlanet* = nullptr);
}

#endif