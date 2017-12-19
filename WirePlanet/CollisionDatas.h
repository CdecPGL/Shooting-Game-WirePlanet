#pragma once
#include"Vector2D.h"
#include<memory>
#include<vector>
#include<unordered_map>

enum class ObjectBelongingTo;
enum CharaAtrribtute;
class Character;
struct CircleObject;

namespace CollisionDatas{
	//キャラクター衝突オブジェクト
	struct CharacterCollisionObject{
		CharacterCollisionObject();
		int hp;
		double t, r;
		Vector2D<double> position;
		std::vector<CircleObject> circle_objects;
		double grotation; //キャラ画像の回転度
		bool reverse; //キャラが反転してるか
		ObjectBelongingTo belongto; //所属しているキャラの種類
		CharaAtrribtute char_attribute; //所属しているキャラの属性
		bool col_with_friend; //味方に攻撃が当たるか
		//		bool hit_stop; //ヒットストップ中か
	};
	//レーザー衝突オブジェクト
	struct LaserCollisionObject{
		LaserCollisionObject();
		int power;
		Vector2D<double> position;
		Vector2D<double> direction;
		int length;
		int width;
		ObjectBelongingTo belongto; //所属しているキャラの種類
	};
}

struct CircleObject{
	CircleObject();
	Vector2D<double> position;
	double radius;
	int attribute; //属性
	int attack_power; //攻撃力
	int matchless_time; //与える無敵時間
};

//キャラ衝突データ
struct CollisionDataWithCharacter{
	CollisionDataWithCharacter();
	Vector2D<double> push_vector; //押し出しベクトル
	Vector2D<double> blow_vector; //吹っ飛ばしベクトル
	CharaAtrribtute char_attribute; //衝突相手のキャラ属性
	//int my_attribute; //衝突された衝突オブジェクトの属性
	int give_damage; //与えたダメージ
	int receive_damage; //受けるダメージ
	int col_chara_id; //衝突相手のID
	int matchless_time; //無敵時間
};

//惑星衝突データ
struct CollisionDataWithPlanet{
	CollisionDataWithPlanet();
	double t; //衝突地点の惑星極座標角度
	double r; //移動すべき半径
	int damage; //ダメージ
};
