#include "CollisionDatas.h"

using namespace std;

CircleObject::CircleObject() :radius(0){}

CollisionDataWithCharacter::CollisionDataWithCharacter() : receive_damage(0), give_damage(0), col_chara_id(-1), matchless_time(0) {}

CollisionDataWithPlanet::CollisionDataWithPlanet() : r(0.0), t(0.0), damage(0){}

CollisionDatas::CharacterCollisionObject::CharacterCollisionObject() : hp(0), t(0), r(0), grotation(0), reverse(false),/*belongto(0),*/col_with_friend(false)/*,hit_stop(false)*/{}

CollisionDatas::LaserCollisionObject::LaserCollisionObject() : power(0), length(0), belongto(), width(0){}