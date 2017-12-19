#pragma once
#include "object.h"
#include "Action.h"

enum CharaAtrribtute{ PLAYER, ENEMY, BULLET, ITEM };

class CharacterBase :
	public Object
{
public:
	//�R���X�g���N�^(���C,�ő�HP�A�G��,ActionData�̎Q��)
	CharacterBase(double, int,ObjectBelongingTo, const std::string&);
	virtual ~CharacterBase()override;

	void Update()override = 0;
	virtual void Draw()override;
	void ProcessCollision()override final;
	/*void ResistToManager()override;
	void UnresistFromManager()override;*/

	int GetGHandle()const;
	bool GetReverseFlag()const; //���]�t���O�擾
	bool GetCollisionWithFriendFlag()const; //���ԂƂ̏Փ˃t���O�擾

	int StartAction(const std::string&); //�A�N�V�����̊J�n(-1�Ŏ��s)

	//�ϐ��擾�֐�
	int GetHP()const;
	int GetMaxHP()const;

	//�ϐ��ݒ�֐�
	void SetAttackPowerMagnification(double); //�U���͔{���ݒ�
	void SetReverse(bool); //�ѓX�̐ݒ�

	//�A�N�V�����T�C�Y�̎擾
	int GetActionSize()const;
	virtual bool GetCollisionObject(CollisionDatas::CharacterCollisionObject*)const;

	void CopyStatus(const CharacterBase&); //�L�����̏�Ԃ��R�s�[

	//�L���������擾
	CharaAtrribtute GetCharaAttribute()const;

	//CharID
	void SetCharID(int);
	int GetCharID()const;
protected:
	int _current_ghandle; //���݂̉摜�n���h��
	std::shared_ptr<Action> action; //�A�N�V����
	bool reverse; //���]���肩(����)(0�Ŗ���(�E����))
	bool col_with_friend;
	std::vector<HitAreaData*> hitarea_data;
	//�L�����萔
	long HP; //HP
	double _atk_pow_magnification; //�U���͔{��
	//�p�����[�^
	int p_HP; //���݂�HP

	CharaAtrribtute _attribute; //�L��������

	bool _bright_flag; //����t���O�B
	bool _col_revers_flag; //�F���]�t���O
	int _revers_draw_gh; //���]�\���p�摜�n���h��

	//���C����
	void Friction();

	virtual void ProcessCollisionWithPlanet();
	virtual void ProcessCollisionWithCharacter();

	void DrawHitArea();

	void UpdateHitAreaData(); //�Փ˔���p�f�[�^�̍X�V
	void UpdateCurrentGHandle(); //���݂̉摜�n���h���񂨍X�V
	void CommonUpdateProcess(); //���ʂ̍X�V����
private:
	//ID
	int char_ID; //�L�����N�^ID
	bool id_settable;
};

