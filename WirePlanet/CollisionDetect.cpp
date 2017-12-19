#include"CollisionDetect.h"

/*なぜかCollisionDetect.hと整合性が取れていない(こっちの方が古い)。OneDriveの同期時の問題？Keyライブラリも同様*/

//静止した円と静止した線分の当たり判定
int CollisionDetect::CollisionDetectsCsL(const Vector2D<double>& sp, double sr, const Vector2D<double>& ls, const Vector2D<double>& le, Vector2D<double>* fix){
	//線分の始点、終点から円の中心へのベクトル
	Vector2D<double> lssp_vec(sp - ls), lesp_vec(sp - le);
	//線分の視点から終点へのベクトル
	Vector2D<double> lsle_vec(le - ls);
	//線分の始点と円の中心のなす角の内積、線分の終点と円の中心のなす角の内積
	double lslelssp_dot(Vector2D<double>::Dot(lsle_vec, lssp_vec)), lslelesp_dot(Vector2D<double>::Dot(lsle_vec, lesp_vec));

	//位置補正値を求める場合
	//円の中心と線分の距離
	double spl_dis;
	//線分ベクトルの垂直単位ベクトル(反時計回りに90度)
	Vector2D<double> lsle_vtcl_uvec;
	if (fix != NULL){
		spl_dis = fabs(Vector2D<double>::Cross(lssp_vec, lsle_vec)) / lsle_vec.Length();
		lsle_vtcl_uvec = -Vector2D<double>::GetVerticalUnitVector(lsle_vec);
	}

	//円と線分の位置関係によって場合分け
	if (lslelssp_dot * lslelesp_dot > 0){ //円が線分の両端より外側にあるとき
		//両端との距離が一つでも半径より小さかったら衝突
		double lssp_length(lssp_vec.Length()), lesp_length(lesp_vec.Length());
		if (lssp_length < sr){

			//位置補正値を求める場合
			if (fix != NULL){
				*fix = lsle_vtcl_uvec*(lssp_length*Vector2D<double>::GetCosin(lssp_vec, lsle_vtcl_uvec) - spl_dis);
			}

			return 1;
		}
		else if (lesp_length < sr){

			//位置補正値を求める場合
			if (fix != NULL){
				*fix = lsle_vtcl_uvec*(lesp_length*Vector2D<double>::GetCosin(lsle_vtcl_uvec, lesp_vec) - spl_dis);
			}

			return 1;
		}
	}
	else{ //円が線分の両端より内側にあるとき
		//線分との距離が半径より小さかったら衝突
		double lsle_length(Vector2D<double>::Length(lsle_vec));
		double lslesp_dis(fabs(Vector2D<double>::Cross(lsle_vec, lssp_vec)) / lsle_length);
		if (lslesp_dis < sr){

			//位置補正値を求める場合
			if (fix != NULL){
				*fix = lsle_vtcl_uvec*(sr - spl_dis);
			}

			return 1;
		}
	}
	return 0;
}

//静止した線分同士の当たり判定
int CollisionDetect::CollisionDetectsLsL(const Vector2D<double>& l1s, const Vector2D<double>& l1e, const Vector2D<double>& l2s, const Vector2D<double>& l2e){
	//線分1の始点から終点へのベクトル
	Vector2D<double> l1sl1e_vec(l1e - l1s);
	//線分2の始点から終点へのベクトル
	Vector2D<double> l2sl2e_vec(l2e - l2s);
	//上二つの外積
	double l1sl1el2sl2e_cross(Vector2D<double>::Cross(l1sl1e_vec, l2sl2e_vec));
	//線分1と線分2が平行(外積が0)だったら当たってない
	if (l1sl1el2sl2e_cross == 0){ return 0; }
	//線分1の始点から線分2の始点へのベクトル
	Vector2D<double> l1sl2s_vec(l2s - l1s);
	//線分1,線分2上の衝突地点の内分点(t1:1-t1)(t2:1-t2)
	double t1, t2;
	t1 = Vector2D<double>::Cross(l1sl2s_vec, l2sl2e_vec) / l1sl1el2sl2e_cross;
	t2 = Vector2D<double>::Cross(l1sl2s_vec, l1sl1e_vec) / l1sl1el2sl2e_cross;
	//t1とt2が0以上1以下だったら当たった
	if ((0 <= t1 && t1 <= 1) && (0 <= t2 && t2 <= 1)){
		return 1;
	}
	else{
		return 0;
	}
}

//静止した点と静止した線分の位置関係を判定
int CollisionDetect::DetectsPsL(const Vector2D<double>& p, const Vector2D<double>& ls, const Vector2D<double>& le){
	//線の始点から点へのベクトル
	Vector2D<double> lsp_vec(p - ls);
	//線の始点から終点へのベクトル
	Vector2D<double> lsle_vec(le - ls);
	//上二つのベクトルの外積
	double lslelsp_cross = Vector2D<double>::Cross(lsle_vec, lsp_vec);
	//外積が正なら表側、負なら裏側、0なら線分上
	if (lslelsp_cross > 0){
		return 1;
	}
	else if (lslelsp_cross < 0){
		return 2;
	}
	else{
		return 0;
	}
}

//動く点と静止した線分の当たり判定
int CollisionDetect::CollisionDetectdPsL(const Vector2D<double>& p, const Vector2D<double>& pm, const Vector2D<double>& ls, const Vector2D<double>& le,Vector2D<double>* fix){
	//流れはsLsL,sPsLの順に処理。かぶった処理を省いてある。
	//線分の始点から終点へのベクトル
	Vector2D<double> lsle_vec(le - ls);
	//上と点移動べクトルのの外積
	double lslepm_cross(Vector2D<double>::Cross(lsle_vec, pm));
	//線分と点の移動ベクトルが平行(外積が0)だったら当たってない
	if (lslepm_cross == 0){ return 0; }
	//線分の始点から点へのベクトル
	Vector2D<double> lsp_vec(p - ls);
	//線分,点移動ベクトル上の衝突地点の内分点(t1:1-t1)(t2:1-t2)
	double t1, t2;
	//線分の始点から点ベクトルと線分ベクトルの外積
	double lsplsle_cross = Vector2D<double>::Cross(lsp_vec, lsle_vec);
	t1 = Vector2D<double>::Cross(lsp_vec, pm) / lslepm_cross;
	t2 = lsplsle_cross / lslepm_cross;
	//t1とt2が0以上1以下だったら当たった
	if ((0 <= t1 && t1 <= 1) && (0 <= t2 && t2 <= 1)){
		//fixにセットされてたら補正値を求める
		if (fix != NULL){
			double lppm_dis = Vector2D<double>::Cross(lsle_vec, p + pm - ls) / lsle_vec.Length(); //移動後の点と線分の距離
			*fix = -Vector2D<double>::GetVerticalUnitVector(lsle_vec) * lppm_dis; //補正ベクトルを求める
		}
		//線分ベクトルと線分の始点から点ベクトルの外積
		double lslelsp_cross(-lsplsle_cross);
		//外積が正なら表側、負なら裏側、0なら線分上(点の初期位置が)
		if (lslelsp_cross > 0){
			return 1;
		}
		else if (lslelsp_cross < 0){
			return 2;
		}
		else{
			//線分上だったら移動後で判断
			Vector2D<double> lspmd_vec(p + pm - ls);
			double lslelspmd_cross(Vector2D<double>::Cross(lsle_vec, lspmd_vec));
			if (lslelspmd_cross>0){
				return 2;
			}
			else if (lslelspmd_cross<0){
				return 1;
			}
			else{
				//移動前後ともに線分上だったら当たってないとみなす
				return 0;
			}
		}

	}
	else{
		return 0;
	}
}

//動く円と静止した線分の当たり判定
int CollisionDetect::CollisionDetectdCsL(const Vector2D<double>& sp, double sr, const Vector2D<double>& sm, const Vector2D<double>& ls, const Vector2D<double>& le){
	return -1;
}

//静止したボックス同士の衝突判定
int CollisionDetect::CollisionDetectsRsR(const Vector2D<double>& b0p, const Vector2D<double>& b0s, const Vector2D<double>& b1p,const  Vector2D<double>& b1s){
	//b0p.x <= b1p.x + b1s.x&&b1p.x <= b0p.x + b0s.x&&b0p.y <= b1p.y + b1s.y&&b1p.y <= b0p.y + b0s.y;
	if (b0p.x > b1p.x + b1s.x){ return 0; }
	if (b1p.x > b0p.x + b0s.x){ return 0; }
	if (b0p.y > b1p.y + b1s.y){ return 0; }
	if (b1p.y > b0p.y + b0s.y){ return 0; }
	return 1;
}

//静止した円同士の衝突判定
int CollisionDetect::CollisionDetectsCsC(const Vector2D<double>& sp0, double sr0, const Vector2D<double>& sp1, double sr1, Vector2D<double>*){
	double dis_2(Vector2D<double>::Length_Square(sp0 - sp1));
	double r_sum_2(pow(sr0 + sr1, 2));
	if (dis_2 <= r_sum_2){ return 1; }
	return 0;
}