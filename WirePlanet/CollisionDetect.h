#ifndef DEF_COLLISIONDETECT_H
#define DEF_COLLISIONDETECT_H

//Version1.0 2013/12/18
//Version1.1 2014/2/18 �Î~�����~���m�̏Փ˔���CollisionDetectsSsS()��ǉ�
//Version2.0 2014/4/22 NULL��nullptr�ɕύX�B�~��\��������S����C�ɕύX�B��`(�{�b�N�X)��B����R�ɁBsCsC�ŉ����o���x�N�g���擾�\�ɁB
//Version2.1 2014/4/25 �֐��̃x�N�^�󂯎���l�n������const�Q�Ɠn���ɕύX�B���������m�̏Փ˔���ǉ��B

#include"Vector2D.h"
//#include"boost/rational.hpp"

namespace CollisionDetect{
	//���̂Ƃ���͐����̎n�_����I�_���������ɍ����𗠁A�E����\�Ƃ��Ă���B
	//���\�����m�F�ς�:dPsL

	//�Î~�����~�Ɛ����̓����蔻��
	//����:�~�̒��S���W�A���a�A�����̎n�_�A�I�_(,�ʒu�␳�l)
	//�߂�l:0�Փ˂��ĂȂ� 1�Փ˂���
	/////�����̐����̒l�������̎��s�?
	int CollisionDetectsCsL(const Vector2D<double>&, const double, const Vector2D<double>&, const Vector2D<double>&, Vector2D<double>* = nullptr); //�����̗��[�̊O�ł̈ʒu�␳�l�v����
	//�Î~�����������m�̓����蔻��
	//����:����1�̎n�_�I�_�A����2�̎n�_�I�_
	//�߂�l:0�Փ˂��ĂȂ� 1�\�Փ˂���
	int CollisionDetectsLsL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	//�Î~�����_�ƐÎ~���������̈ʒu�֌W�𔻒�
	//����:�_�̈ʒu�A�����̎n�_�I�_
	//�߂�l:�����̎n�_����I�_���������ɍ�����\�A�E���𗠂��A0������ 1�\�� 2����(�v���� �E���\�ɂȂ��Ă�)
	int DetectsPsL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	/**
	* @brief �����_�ƐÎ~���������̓����蔻��(�ʒu�␳�l�v����)
	* @param �_�̈ʒu(�ړ��O)�A�ړ��x�N�g���A�����̎n�_�I�_(,�����o���x�N�g���i�[��)
	* @return 0:�Փ˂��ĂȂ� 1:�\����Փ˂��� 2:������Փ˂���(�v����)
	*/
	int CollisionDetectdPsL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&,Vector2D<double>* = nullptr);
//	int CollisionDetectdPsL(const Vector2D<int>&, const Vector2D<int>&, const Vector2D<int>&, const Vector2D<int>&, Vector2D<int>* = nullptr);
	/**
	* @brief �����_�ƐÎ~�����������̓����蔻��(�ʒu�␳�l�v����)
	* @param �_�̈ʒu(�ړ��O)�A�ړ��x�N�g���A�����̎n�_�A�����x�N�g��(,�����o���x�N�g���i�[��)
	* @return 0:�Փ˂��ĂȂ� 1:�\����Փ˂��� 2:������Փ˂���(�v����)
	*/
	int CollisionDetectdPsHL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, Vector2D<double>* = nullptr);
	//�����~�ƐÎ~���������̓����蔻��
	//����:�~�̈ʒu�A���a�A�ړ��x�N�g���A�����̎n�_�I�_
	//�߂�l:0�Փ˂��ĂȂ� 1�\����Փ˂��� 2������Փ˂���(�v����)
	int CollisionDetectdCsL(const Vector2D<double>&, double, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	/**
	*@brief �Î~������`���m�̓����蔻��
	*@param ��`0�̈ʒu�A�T�C�Y,��`1�̈ʒu�A�T�C�Y(�ʒu�͍�����W)
	*@return 0�Փ˂��ĂȂ� 1:�Փ˂���
	*/
	int CollisionDetectsRsR(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
	/**
	*@brief ������`�ƐÎ~����`�̓����蔻��
	*@param ������`�̈ʒu(�ړ��O)�A�T�C�Y�A�ړ��x�N�g���A�Î~���Ă����`�̈ʒu�A�T�C�Y�A�����o���x�N�g���i�[�ϐ�
	*@return 0:�Փ˂��ĂȂ� 1:�Փ˂���
	*/
	int CollisionDetectdRsR(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, Vector2D<double>* = nullptr);
	/**
	*@brief �Î~�����~���m�̓����蔻��
	*@param �~0�̈ʒu�A���a,�~1�̈ʒu�A���a
	*@return 0�Փ˂��ĂȂ� 1:�Փ˂���
	*/
	int CollisionDetectsCsC(const Vector2D<double>&, double, const Vector2D<double>&, double,Vector2D<double>* = nullptr);
	/**
	*@brief �Î~�������������m�̏Փ˔���
	*@param ������1�̎n�_�A�����x�N�g���A������2�̎n�_�A�����x�N�g��
	*@return false:�Փ˂��ĂȂ� true:�Փ˂��Ă���
	*/
	bool CollisionDetectsHLsHL(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&);
}

#endif