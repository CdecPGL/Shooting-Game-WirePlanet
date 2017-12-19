#include"CollisionDetecter.h"
#include"Planet.h"
#include"CollisionDetect.h"
#include<algorithm>
#include"CreateEffectFuncs.h"

#include"Action.h"

using namespace std;

bool CollisionFuncs::CollisionDetectsCsC(const CircleObject& co0, const CircleObject& co1, Vector2D<double>* f){
	//衝突物体の保持者の画像角度を考慮した位置を求める(Character::Drawのコードを流用)
	/*int rev0 = co0.h_reverse ? -1 : 1;
	int rev1 = co1.h_reverse ? -1 : 1;*/
	/*Vector2D<double> co0p(co0.h_position.x - rev0 * co0.position.x * sin(co0.h_grotation / 180 * DX_PI - DX_PI / 2) - co0.position.y * cos(co0.h_grotation / 180 * DX_PI - DX_PI / 2), co0.h_position.y +  rev0 * co0.position.x * cos(co0.h_grotation / 180 * DX_PI - DX_PI / 2) - co0.position.y * sin(co0.h_grotation / 180.0 * DX_PI - DX_PI / 2));
	Vector2D<double> co1p(co1.h_position.x - rev1 * co1.position.x * sin(co1.h_grotation / 180 * DX_PI - DX_PI / 2) - co1.position.y * cos(co1.h_grotation / 180 * DX_PI - DX_PI / 2), co1.h_position.y +  rev1 * co1.position.x * cos(co1.h_grotation / 180 * DX_PI - DX_PI / 2) - co1.position.y * sin(co1.h_grotation / 180.0 * DX_PI - DX_PI / 2));*/
//	DrawCircle(co0p.x, co0p.y, co0.radius, GetColor(0, 0, 255));
//	DrawCircle(co1p.x, co1p.y, co1.radius, GetColor(0, 0, 255));
	/*return CollisionDetect::CollisionDetectsCsC(co0p, co0.radius, co1p, co1.radius, f) == 0 ? false : true;*/
	return true;
}

Vector2D<double> CollisionFuncs::CalculateScreenPositionByColObj(const CollisionDatas::CharacterCollisionObject& cco,const CircleObject& co){
	int rev = cco.reverse ? -1 : 1;
	return Vector2D<double>(cco.position.x - rev * co.position.x * sin(cco.grotation / 180 * DX_PI - DX_PI / 2) - co.position.y * cos(cco.grotation / 180 * DX_PI - DX_PI / 2), cco.position.y + rev * co.position.x * cos(cco.grotation / 180 * DX_PI - DX_PI / 2) - co.position.y * sin(cco.grotation / 180.0 * DX_PI - DX_PI / 2));
}

bool CollisionFuncs::CollisionDetectsCharsChar(const CollisionDatas::CharacterCollisionObject& cco0, CollisionDataWithCharacter* cdc0, const CollisionDatas::CharacterCollisionObject& cco1, CollisionDataWithCharacter* cdc1){
	bool col_flag = false;
	for (const CircleObject& c0 : cco0.circle_objects){
		//衝突物体の保持者の画像角度を考慮した位置を求める
		Vector2D<double> co0p(CalculateScreenPositionByColObj(cco0, c0));
//		Vector2D<double> co0p(cco0.position);
		for (const CircleObject& c1 : cco1.circle_objects){
			//組み合わせが攻撃とダメージの組み合わせ出なかったら
			if (!(((c0.attribute & Col_Atrb_Atck) == Col_Atrb_Atck && (c1.attribute & Col_Atrb_Dmge) == Col_Atrb_Dmge)
				|| ((c0.attribute & Col_Atrb_Dmge) == Col_Atrb_Dmge && (c1.attribute & Col_Atrb_Atck) == Col_Atrb_Atck)))
			{
				//更に双方にアイテム属性が含まれているわけではなかったらスキップ
				if ((c0.attribute & Col_Atrb_Item) == Col_Atrb_Item && (c1.attribute & Col_Atrb_Item) == Col_Atrb_Item){
				}
				else{
					continue;
				}
			}
			//衝突物体の保持者の画像角度を考慮した位置を求める
			Vector2D<double> co1p(CalculateScreenPositionByColObj(cco1, c1));
//			Vector2D<double> co1p(cco1.position);
			Vector2D<double> f;
			if (CollisionDetect::CollisionDetectsCsC(co0p, c0.radius, co1p, c1.radius, &f) == 0 ? false : true){
				col_flag = true;
				if (cdc0){
					cdc0->push_vector = f;
					cdc0->char_attribute = cco1.char_attribute;
					if (c0.attribute & Col_Atrb_Dmge){ 
						cdc0->receive_damage = c1.attack_power;
						//無敵時間は最大のものを
						if (cdc0->matchless_time < c1.matchless_time){ cdc0->matchless_time = c1.matchless_time; }
					}
					if (c0.attribute & Col_Atrb_Atck){ cdc0->give_damage = c0.attack_power; }
				}
				if (cdc1){
					cdc1->push_vector = -f;
					cdc1->char_attribute = cco0.char_attribute;
					if (c1.attribute & Col_Atrb_Dmge){
						cdc1->receive_damage = c0.attack_power;
						//無敵時間は最大のものを
						if (cdc1->matchless_time < c0.matchless_time){ cdc1->matchless_time = c0.matchless_time; }
					}
					if (c1.attribute & Col_Atrb_Atck){ cdc1->give_damage = c1.attack_power; }
				}
			}
		}
	}
	return col_flag;
}

bool CollisionFuncs::CollisionDetectsCharsPlanet(const CollisionDatas::CharacterCollisionObject& cco, shared_ptr<Planet> p, CollisionDataWithPlanet* cdwp){
	bool col_flag = false;
	//地形との衝突
	//位置が一番低いもの(相対位置が一番大きいもの)で判定
	auto co_bottom = min_element(cco.circle_objects.begin(), cco.circle_objects.end(), [](const CircleObject& c0, const CircleObject& c1)->bool{
		//属性に地形衝突がないものは評価を悪くする
		if ((c0.attribute & Col_Atrb_Trrn) != Col_Atrb_Trrn){ return false; }
		if ((c1.attribute & Col_Atrb_Trrn) != Col_Atrb_Trrn){ return true; }
		return c0.radius + c0.position.y > c1.radius + c1.position.y;
	});
	////属性に地形衝突があったら(なかったら地形衝突オブジェクトが存在しないということになるので攻撃オブジェクトとの判定はしない)
	if ((*co_bottom).attribute & Col_Atrb_Trrn){
		//当たり判定保持者の極座標角度と衝突オブジェクトの相対位置から衝突オブジェクトの極座標角度を求める
		double t = cco.t + (2.0 * (*co_bottom).position.x) / pow((*co_bottom).radius, 2);
		double r = cco.r - (*co_bottom).position.y;
		double hit_distance = p->GetDistance(t) + (*co_bottom).radius;
		//当たったら
		if (r < hit_distance){
			if (cdwp){
				cdwp->r = hit_distance + (*co_bottom).position.y;
				cdwp->t = t;
			}
			col_flag = true;
		}
		else{
			if (cdwp){
				cdwp->r = cco.r; cdwp->t = cco.t;
			}
		}
	}
	else{
		return col_flag;
	}

	return col_flag;
}

bool CollisionFuncs::CollisionDetectsCharsLaser(const CollisionDatas::CharacterCollisionObject& cco, const CollisionDatas::LaserCollisionObject& lco, CollisionDataWithCharacter* cdwc){
	bool col_flag = false;
	for (auto& co : cco.circle_objects){
		if ((co.attribute & Col_Atrb_Dmge) == 0){ break; }
		Vector2D<double> scr_pos(CalculateScreenPositionByColObj(cco, co));
		if (CollisionDetect::CollisionDetectsCsL(scr_pos, co.radius + lco.width/2, lco.position, lco.position + Vector2D<double>::GetParallelUnitVector(lco.direction)*lco.length)){
			col_flag = true;
			if (cdwc){
				cdwc->give_damage = 0;
				cdwc->receive_damage = lco.power;
				cdwc->matchless_time =2;
			}
			break;
		}
	}
	return col_flag;
}

int CollisionFuncs::CollisionDetectsCharsRectin(const CollisionDatas::CharacterCollisionObject& cco, const RectAngle<int>& ra, CollisionDataWithPlanet* cdp){
	//xy座標とtr座標では縦方向に座標が反転しているので注意
	//キャラクターの判定矩形を求める
	int l(0), r(0), t(0), b(0);
	for (auto& co : cco.circle_objects){
		if ((co.attribute & Col_Atrb_Trrn) != Col_Atrb_Trrn){ continue; }
		//回転は考えてない
		Vector2D<int> c_pos((int)co.position.x, (int)co.position.y);
		int cl(c_pos.x - co.radius), cr(c_pos.x + co.radius), ct(c_pos.y - co.radius), cb(c_pos.y + co.radius);
		if (cl < l){ l = cl; }
		if (cr > r){ r = cr; }
		if (ct < t){ t = ct; }
		if (cb > b){ b = cb; }
	}
	if (l >= r || t >= b){ return false; }
	int wl(cco.position.x+l), wr(cco.position.x+r), wt(cco.position.y+t), wb(cco.position.y+b);
	if (cdp){
		cdp->t = cco.t;
		cdp->r = cco.r;
		cdp->damage = 0;
	}
	if (wl < ra.left()){

	}
	//衝突判定
	int col_flag = 0;
	if (wl <= ra.left()){
		cdp->t = ra.left() - l;
		col_flag = 1;
	}
	else if (wr >= ra.right()){
		cdp->t = ra.right() - r;
		col_flag = 1;
	}
	if (wt <= ra.top()){
		cdp->r = -(ra.top() - t);
		col_flag = 1;
	}
	else if (wb >= ra.bottom()){
		cdp->r = -(ra.bottom() - b);
		col_flag = 1;
	}
	return col_flag;
}
