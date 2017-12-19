#pragma once
#include "player.h"
//�v���[���[�j

class PlayerB :
	public Player
{
public:
	//�R���X�g���N�^(���C,HP,�ő�ړ����x,���G����,ActionData��)
	PlayerB(double, long, double, long,const std::string&);
	virtual ~PlayerB()override;
protected:
	void DieState()override;
	//�\�̓p�����[�^
	int _shoot_interval; //�e�̔��ˊԊu
	int _shoot_speed; //�e�̔��ˑ��x
	double _shoot_power; //�e�̍U����
	int _throw_speed; //���̔��ˑ��x
	double _throw_power; //���̍U����
	int _h_shoot_suspend; //���V���b�g�e�̎�������
	int _max_throw_num; //�����ɓ������錕�̐�
	double _walk_acceralation; //�����Ƃ��̉����x
	int _jump_suspend; //�W�����v�̎�������
	double _exp_to_hp_rate; //�o���l��HP�ɕϊ�����Ƃ��̃��[�g
	double _barrier_exp_consumption; //�o���A��EXP�����
	//���݂̏�ԃp�����[�^
	int _jumping_frame; //�W�����v�����Ă���t���[����
	bool _barrier_flag;
	unsigned int _cntr_from_create_clone;
	//�p�����[�^�Z�b�g�֐�
	void SetParameterByLevel(int)override;
	//���ʏ�ԋ��ʊ֐�
	void UniversalLowStateProc()override;

	void ProcessCollisionWithCharacter()override;

	std::vector<int> throwed_sword_id; //����������ID

	std::vector<std::shared_ptr<Player>> _clones;

	//NormalState���ʏ�Ԋ֐��}�b�v�Z�b�g
	void SetNormalStateLowStateFuncMap()override;
	//NormalState���ʏ�Ԋ֐�
	void NormalLStateUpdate(); //�ʏ�
	void NormalLStateEnter();
	void HShootLStateUpdate(); //���e
	void HShootLStateEnter();
	void BarrierLStateUpdate(); //�o���A
	void BarrierLStateEnter();
	void BarrierLStateLeave();
	void SquatLStateEnter(); //���Ⴊ��
	void SquatLStateUpdate();
	void SquatLStateLeave();
	void LaserLStateEnter(); //���[�U�[(������)
	void CreateCloneLStateUpdate(); //�N���[���쐬
	void CreateCloneLStateEnter(); //�N���[���쐬
	void CreateCloneLStateLeave(); //�N���[���쐬

};

