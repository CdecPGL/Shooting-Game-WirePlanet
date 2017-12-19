#ifndef DEF_CHARACTER_H //��dinclude�h�~

#define DEF_CHARACTER_H

#include "CharacterBase.h"
#include<memory>

class Planet;

const std::string ANIM_TAG_CHAR_ATTACKED("attacked");
const std::string ANIM_TAG_CHAR_IDOL("idol");

struct CollisionDatas::CharacterCollisionObject;

//�L�����̏��(�U���󂯒��A�ʏ��ԁA����)
enum CharacterState{ ATTACKED,  NORMAL,DIE};

//Character�N���X
class Character : public CharacterBase{
public:
	//�R���X�g���N�^(���C,HP,�G��,���G����,ActionData�̎Q��)
	Character(double,long,ObjectBelongingTo,long,const std::string&);
	 //�f�X�g���N�^
	virtual ~Character()override;

	//�ϐ��擾�֐�

	//CharacterManager�ɌĂяo�����

	//������
	virtual int Init();

	void Update()override final;

	virtual bool ReceiveMessage(const std::string&, double)override;

	//�ڒn�t���O�ύX(true�Őڒn)
	void ChangeGroundFlag(bool);
	//�ꎞ��~�t���O�ύX
	void ChangePauseFlag(bool);
	//�Փ˃I�u�W�F�N�g�̎擾(����������false)
	bool GetCollisionObject(CollisionDatas::CharacterCollisionObject*)const override;

	//���[�h(�����A�O������Control)
	enum class Mode{ StandAlone=0,Control=1 };
	void SetMode(Mode);
	void SetMode(int);

	enum Direction { LEFT, RIGHT };
	void ChangeDirection(Direction); //�����ύX
protected:
	//���[�h
	Mode _mode;
	void UpdateInStandAloneMode();
	void UpdateInControlMode();
	//�L�����萔
	long matchless_time; //���G����(�萔)

	//�L�����p�����[�^
	bool matchless; //true�Ŗ��G
	long time_from_attacked; //�U�����󂯂Ă���̌o�ߎ���

	bool ground_flag; //�ڒn�t���O(1�Őڒn)
	bool pause_flag; //�ꎞ��~�t���O
	int hit_stop_cnt; //�q�b�g�X�g�b�v�J�E���^�[

	//�U�����󂯂Ȃ��t���O(�C�x���g���Ȃ�)
	bool _muteki;

	//�Փˏ���
	virtual void ProcessCollisionWithCharacter()override;
	virtual void ProcessCollisionWithPlanet()override;

	//�L�����̏�Ԃ��Ƃ̏����֐�
	virtual void NormalState();
	virtual void AttackedState();
	virtual void DieState();

	//�󋵕ύX�֐�
	//�U�����󂯂�(�_���[�W�̑傫��)
	virtual void ReceivedAttack(int);
	//�ʏ�ɕ��A
	virtual void ReturnNormal();
	//����
	virtual void Died();
private:

	CharacterState state;
	//�󋵂Ɋ֌W���Ȃ����ʏ���
	void CommonState();
};

#endif