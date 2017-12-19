#ifndef DEF_BULLET_H
#define DEF_BULLET_H

#include "CharacterBase.h"

class BulletMover;

class Bullet : public CharacterBase{
	friend class BulletMover;
public:
	//�R���X�g���N�^(���Z�u�����h���[�h�A�u�����h�p�����[�^�A�F(RGB��16�i���ŕ\����������)�AAction�t�@�C����)
	Bullet(int,int,int,const std::string&);
	//�f�X�g���N�^
	virtual ~Bullet() = default;
	//�`��
	void Draw()override;
	//�X�V
	void Update()override final;

	//�Z�b�g(w,vr)
	void SetVelocity(double,double);
	void SetSuspendFrame(int); //�����t���[���ݒ�(-1�Ńf�t�H���g�l���Z�b�g)
protected:
	virtual void move() = 0;
	int _inc_cntr;
	int _suspend_frame; //�e���������鎞��
private:
	int blend_mode;
	int blend_param;
	int col[3]; //�FRGB

	//�Փˏ���
	void ProcessCollisionWithPlanet()override;
	void ProcessCollisionWithCharacter()override;

	static int Convert16ColToRGBColor(int, int*, int*, int*); //�F16�i���l�AR�AG�AB
};

#endif DEF_BULLET_H