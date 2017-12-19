#ifndef DEF_PLAYER_H //��dinclude�h�~

#define DEF_PLAYER_H

#include "character.h"
#include "DxLib.h"

#include"SecureObject.h"

//namespace secure_obj{
//	class BitInvertEncrypter;
//	template < typename T, class E = BitInvertEncrypter > 
//	class SecureObject;
//}

class LowStateIndivisualManager;
namespace PlayerInput{
	static const unsigned int UP(1);
	static const unsigned int DOWN(2);
	static const unsigned int LEFT(4);
	static const unsigned int RIGHT(8);
	static const unsigned int A(16);
	static const unsigned int B(32);
	static const unsigned int X(64);
	static const unsigned int Y(128);
	static const unsigned int L(256);
	static const unsigned int R(512);
	enum MultiInput{OR,AND};
}

//Player�N���X
class Player : public Character{
public:
	//�R���X�g���N�^(���C,HP,�ő�ړ����x,���G����,ActionData��)
	Player(double,long,double,long,const std::string&);
	//�f�X�g���N�^
	virtual ~Player()override;
	//������
	int Init()override;
	//�`��
	void Draw();
	//���b�Z�[�W�󂯎��
	bool ReceiveMessage(const std::string&, double=0)override;

	//�o���l�擾
	void AddExperiencePoint(int);
	//���x���擾
	int GetLevel()const;
	//�o���l�擾
	unsigned int GetEXP()const;
	//�o���l���擾(���̃��x���ƌ��݂̃��x����)
	int GetExpDifferenceToNextLevel()const;
	//���̃��x���܂łɕK�v�Ȍo���l�擾
	int GetExpRemainingToNextLevel()const;
	//���͏��Z�b�g
	void SetInput(unsigned int);
	void ClearInput();
protected:
	double maximum_speed; //�ő�ړ����x
	bool bullet_flag; //true�Œe���ˍς� �ڒn���Ƀ��Z�b�g
	secure_obj::SecureObject<int> _total_exp_s; //�݌v�o���l(�s���΍�ς�)
	int _level; //���x��
	int _level_resistance; //���x���̏オ��ɑ�

	//���x���ɉ������p�����[�^�Z�b�g(���x��)
	virtual void SetParameterByLevel(int);

	//���x���v�Z�@(�݌v�o���l�A���x���̏オ��ɂ���)
	static int LevelCalculater(int, int);
	//�o���l�t�Z�@(���x���A���x���̏オ��ɂ���)
	static int ExpCaluculater(int, int);

	//��ʏ󋵊֐�
	void NormalState()override;
	void DieState()override;

	////NormalState���ʏ�ԕϐ�
	std::vector<int> ns_vars; //���ʏ�ԕύX���Ƀ��Z�b�g
	//NormalState���ʏ�Ԋ֐��}�b�v�Z�b�g
	virtual void SetNormalStateLowStateFuncMap();
	virtual void UniversalLowStateProc(); //���ʏ�ԋ��ʏ���
	//NormalState���ʏ�ԊǗ�
	std::unique_ptr < LowStateIndivisualManager > ns_ls_mgr;

	//���͊֌W
	bool ButtonPush(unsigned int,PlayerInput::MultiInput = PlayerInput::AND)const; //�{�^���������ꂽ��(�m�F����{�^���A�����̏ꍇ�̏������@)
	bool ButtonStatus(unsigned int, PlayerInput::MultiInput = PlayerInput::AND)const;  //�{�^���̏��(�m�F����{�^���A�����̏ꍇ�̏������@)
private:
	unsigned int _input; //����
	unsigned int _pinput; //�O��̓���
	unsigned int _pushed_input; //�������ꂽ�{�^��
	void ChangeLevel(int); //���x���ύX����
};

#endif