#include"CollisionDetect.h"

/*�Ȃ���CollisionDetect.h�Ɛ����������Ă��Ȃ�(�������̕����Â�)�BOneDrive�̓������̖��HKey���C�u���������l*/

//�Î~�����~�ƐÎ~���������̓����蔻��
int CollisionDetect::CollisionDetectsCsL(const Vector2D<double>& sp, double sr, const Vector2D<double>& ls, const Vector2D<double>& le, Vector2D<double>* fix){
	//�����̎n�_�A�I�_����~�̒��S�ւ̃x�N�g��
	Vector2D<double> lssp_vec(sp - ls), lesp_vec(sp - le);
	//�����̎��_����I�_�ւ̃x�N�g��
	Vector2D<double> lsle_vec(le - ls);
	//�����̎n�_�Ɖ~�̒��S�̂Ȃ��p�̓��ρA�����̏I�_�Ɖ~�̒��S�̂Ȃ��p�̓���
	double lslelssp_dot(Vector2D<double>::Dot(lsle_vec, lssp_vec)), lslelesp_dot(Vector2D<double>::Dot(lsle_vec, lesp_vec));

	//�ʒu�␳�l�����߂�ꍇ
	//�~�̒��S�Ɛ����̋���
	double spl_dis;
	//�����x�N�g���̐����P�ʃx�N�g��(�����v����90�x)
	Vector2D<double> lsle_vtcl_uvec;
	if (fix != NULL){
		spl_dis = fabs(Vector2D<double>::Cross(lssp_vec, lsle_vec)) / lsle_vec.Length();
		lsle_vtcl_uvec = -Vector2D<double>::GetVerticalUnitVector(lsle_vec);
	}

	//�~�Ɛ����̈ʒu�֌W�ɂ���ďꍇ����
	if (lslelssp_dot * lslelesp_dot > 0){ //�~�������̗��[���O���ɂ���Ƃ�
		//���[�Ƃ̋�������ł����a��菬����������Փ�
		double lssp_length(lssp_vec.Length()), lesp_length(lesp_vec.Length());
		if (lssp_length < sr){

			//�ʒu�␳�l�����߂�ꍇ
			if (fix != NULL){
				*fix = lsle_vtcl_uvec*(lssp_length*Vector2D<double>::GetCosin(lssp_vec, lsle_vtcl_uvec) - spl_dis);
			}

			return 1;
		}
		else if (lesp_length < sr){

			//�ʒu�␳�l�����߂�ꍇ
			if (fix != NULL){
				*fix = lsle_vtcl_uvec*(lesp_length*Vector2D<double>::GetCosin(lsle_vtcl_uvec, lesp_vec) - spl_dis);
			}

			return 1;
		}
	}
	else{ //�~�������̗��[�������ɂ���Ƃ�
		//�����Ƃ̋��������a��菬����������Փ�
		double lsle_length(Vector2D<double>::Length(lsle_vec));
		double lslesp_dis(fabs(Vector2D<double>::Cross(lsle_vec, lssp_vec)) / lsle_length);
		if (lslesp_dis < sr){

			//�ʒu�␳�l�����߂�ꍇ
			if (fix != NULL){
				*fix = lsle_vtcl_uvec*(sr - spl_dis);
			}

			return 1;
		}
	}
	return 0;
}

//�Î~�����������m�̓����蔻��
int CollisionDetect::CollisionDetectsLsL(const Vector2D<double>& l1s, const Vector2D<double>& l1e, const Vector2D<double>& l2s, const Vector2D<double>& l2e){
	//����1�̎n�_����I�_�ւ̃x�N�g��
	Vector2D<double> l1sl1e_vec(l1e - l1s);
	//����2�̎n�_����I�_�ւ̃x�N�g��
	Vector2D<double> l2sl2e_vec(l2e - l2s);
	//���̊O��
	double l1sl1el2sl2e_cross(Vector2D<double>::Cross(l1sl1e_vec, l2sl2e_vec));
	//����1�Ɛ���2�����s(�O�ς�0)�������瓖�����ĂȂ�
	if (l1sl1el2sl2e_cross == 0){ return 0; }
	//����1�̎n�_�������2�̎n�_�ւ̃x�N�g��
	Vector2D<double> l1sl2s_vec(l2s - l1s);
	//����1,����2��̏Փ˒n�_�̓����_(t1:1-t1)(t2:1-t2)
	double t1, t2;
	t1 = Vector2D<double>::Cross(l1sl2s_vec, l2sl2e_vec) / l1sl1el2sl2e_cross;
	t2 = Vector2D<double>::Cross(l1sl2s_vec, l1sl1e_vec) / l1sl1el2sl2e_cross;
	//t1��t2��0�ȏ�1�ȉ��������瓖������
	if ((0 <= t1 && t1 <= 1) && (0 <= t2 && t2 <= 1)){
		return 1;
	}
	else{
		return 0;
	}
}

//�Î~�����_�ƐÎ~���������̈ʒu�֌W�𔻒�
int CollisionDetect::DetectsPsL(const Vector2D<double>& p, const Vector2D<double>& ls, const Vector2D<double>& le){
	//���̎n�_����_�ւ̃x�N�g��
	Vector2D<double> lsp_vec(p - ls);
	//���̎n�_����I�_�ւ̃x�N�g��
	Vector2D<double> lsle_vec(le - ls);
	//���̃x�N�g���̊O��
	double lslelsp_cross = Vector2D<double>::Cross(lsle_vec, lsp_vec);
	//�O�ς����Ȃ�\���A���Ȃ痠���A0�Ȃ������
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

//�����_�ƐÎ~���������̓����蔻��
int CollisionDetect::CollisionDetectdPsL(const Vector2D<double>& p, const Vector2D<double>& pm, const Vector2D<double>& ls, const Vector2D<double>& le,Vector2D<double>* fix){
	//�����sLsL,sPsL�̏��ɏ����B���Ԃ����������Ȃ��Ă���B
	//�����̎n�_����I�_�ւ̃x�N�g��
	Vector2D<double> lsle_vec(le - ls);
	//��Ɠ_�ړ��׃N�g���̂̊O��
	double lslepm_cross(Vector2D<double>::Cross(lsle_vec, pm));
	//�����Ɠ_�̈ړ��x�N�g�������s(�O�ς�0)�������瓖�����ĂȂ�
	if (lslepm_cross == 0){ return 0; }
	//�����̎n�_����_�ւ̃x�N�g��
	Vector2D<double> lsp_vec(p - ls);
	//����,�_�ړ��x�N�g����̏Փ˒n�_�̓����_(t1:1-t1)(t2:1-t2)
	double t1, t2;
	//�����̎n�_����_�x�N�g���Ɛ����x�N�g���̊O��
	double lsplsle_cross = Vector2D<double>::Cross(lsp_vec, lsle_vec);
	t1 = Vector2D<double>::Cross(lsp_vec, pm) / lslepm_cross;
	t2 = lsplsle_cross / lslepm_cross;
	//t1��t2��0�ȏ�1�ȉ��������瓖������
	if ((0 <= t1 && t1 <= 1) && (0 <= t2 && t2 <= 1)){
		//fix�ɃZ�b�g����Ă���␳�l�����߂�
		if (fix != NULL){
			double lppm_dis = Vector2D<double>::Cross(lsle_vec, p + pm - ls) / lsle_vec.Length(); //�ړ���̓_�Ɛ����̋���
			*fix = -Vector2D<double>::GetVerticalUnitVector(lsle_vec) * lppm_dis; //�␳�x�N�g�������߂�
		}
		//�����x�N�g���Ɛ����̎n�_����_�x�N�g���̊O��
		double lslelsp_cross(-lsplsle_cross);
		//�O�ς����Ȃ�\���A���Ȃ痠���A0�Ȃ������(�_�̏����ʒu��)
		if (lslelsp_cross > 0){
			return 1;
		}
		else if (lslelsp_cross < 0){
			return 2;
		}
		else{
			//�����ゾ������ړ���Ŕ��f
			Vector2D<double> lspmd_vec(p + pm - ls);
			double lslelspmd_cross(Vector2D<double>::Cross(lsle_vec, lspmd_vec));
			if (lslelspmd_cross>0){
				return 2;
			}
			else if (lslelspmd_cross<0){
				return 1;
			}
			else{
				//�ړ��O��Ƃ��ɐ����ゾ�����瓖�����ĂȂ��Ƃ݂Ȃ�
				return 0;
			}
		}

	}
	else{
		return 0;
	}
}

//�����~�ƐÎ~���������̓����蔻��
int CollisionDetect::CollisionDetectdCsL(const Vector2D<double>& sp, double sr, const Vector2D<double>& sm, const Vector2D<double>& ls, const Vector2D<double>& le){
	return -1;
}

//�Î~�����{�b�N�X���m�̏Փ˔���
int CollisionDetect::CollisionDetectsRsR(const Vector2D<double>& b0p, const Vector2D<double>& b0s, const Vector2D<double>& b1p,const  Vector2D<double>& b1s){
	//b0p.x <= b1p.x + b1s.x&&b1p.x <= b0p.x + b0s.x&&b0p.y <= b1p.y + b1s.y&&b1p.y <= b0p.y + b0s.y;
	if (b0p.x > b1p.x + b1s.x){ return 0; }
	if (b1p.x > b0p.x + b0s.x){ return 0; }
	if (b0p.y > b1p.y + b1s.y){ return 0; }
	if (b1p.y > b0p.y + b0s.y){ return 0; }
	return 1;
}

//�Î~�����~���m�̏Փ˔���
int CollisionDetect::CollisionDetectsCsC(const Vector2D<double>& sp0, double sr0, const Vector2D<double>& sp1, double sr1, Vector2D<double>*){
	double dis_2(Vector2D<double>::Length_Square(sp0 - sp1));
	double r_sum_2(pow(sr0 + sr1, 2));
	if (dis_2 <= r_sum_2){ return 1; }
	return 0;
}