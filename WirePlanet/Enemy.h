#ifndef DEF_ENEMY_H
#define DEF_ENEMY_H

#include "character.h"

class EnemyMoverManager;

class Enemy :public Character{
public:
	//�R���X�g���N�^(�̗́A���C�A���G���ԁA�h���b�v�G�i�W�[�A���[�oID�AAction�t�@�C����)
	Enemy(int,double,int,int,const std::string&,const std::string&);
	//�f�X�g���N�^
	~Enemy(void)override;

	int Init()override;

	//�Փˏ���
	virtual void ProcessCollisionWithCharacter()override;

	//���[�o�p
	int StartReverseAction(const std::string&); //�A�N�V�����̋t�Đ�
	bool CheckActionPlaying()const; //�A�N�V�����Đ��������ׂ�
	bool GetGroundFrag()const;
	void AddDefaultGravity();
	void ProcessFriction(); //���C�̏���
	//�^�[�Q�b�g(�v���C���[)�Ƃ̋����擾
	double GetTargetDistance()const;
	double GetTargetDistanceT()const;
	double GetTargetDistanceR()const;
	void AddForceToPlanet(double,double); //�f���ɗ͂�������
	int GetBelongingPlanetRadius()const; //�����f���̔��a�擾(�Ȃ�������-1��Ԃ�)
	void Landed(); //�㗤����
	double GetHPPercentage()const;
	//���[�o�[���쐬�����I�u�W�F�N�g�̎擾�B(�I�u�W�F�N�gID)
	Object* GetMCObject(int);
	//�I�u�W�F�N�g�̏I��
	int EndMCObject(int);
	//�I�u�W�F�N�g�쐬�n�֐�
	int FireBullet(const std::string&, double, double, double); //�e����(������ID�A�����A���x�A�U����)
	int FireRotatingBullet(const std::string&, double, double, double, double); //��]�e����(������ID�A�����A���x�A�U���́A��]���x)
	int FireLaser(int, int, int, int, int, int, double); //���[�U�[����(�����A�x���A���Ԏ��ԁAr,g,b,����(�x))(������)
	int CreateCharacter(const std::string&);

private:
	//����
	void move();
	//�ϐ�
	double distance_toplayer; //�v���C���[�Ƃ̋���
	EnemyMoverManager& emm;
	int _mvr_id;
	std::string mvr_name;
	int mvr_state; //���[�o�̏��(0:������s 1:���ʎ��s 2:�I��)
	bool _land_flag; //�㗤�t���O
	int _drop_energy;

	int _explosion_cntr; //�����J�E���^�[
	
	std::unordered_map<int ,std::pair<bool,std::weak_ptr<Object>>> _mvr_created_objects; //map<id,<���񂾎��ɏI���������s����,�I�u�W�F�N�g>>
	int _mvr_crt_obj_cnt;
	int setMvrCrtObj(std::weak_ptr<Object>,bool); //�I�u�W�F�N�g�A���񂾎��ɏI���������s����
	void endAllMCObject(); //���[�o�[���쐬�����I�u�W�F�N�g�����ׂďI������(End()��Ăяo��)

	void ReceivedAttack(int)override;

	//��Ԃ��Ƃ̏����֐�
	void NormalState()override;
	void DieState()override;
};

#endif