#include"CharacterBase.h"

namespace CreateCharaFuncs{
	//�e����(������ID�A����(��)�A�����A�U���͔{���A���˃L�����A[��]���x])
	std::shared_ptr<CharacterBase> FireBullet(const std::string&, double, double,double, const CharacterBase*,double=0.0);
	//�������Ԏw��Œe����(������ID�A����(��)�A�����A�U���͔{���A���˃L�����A�������ԁA[��]���x])
	std::shared_ptr<CharacterBase> FireBulletAssignSuspend(const std::string&, double, double, double, const CharacterBase*, int, double = 0.0);
	//���𓊂���(����(��),����,�A�U���͔{���A�������L������ID[,�������߂̃L��������̋���])
	std::shared_ptr<CharacterBase> ThrowSword(double, double,double, int, int = 0);
	//�h���b�v�G�l���M�[�{�[���쐬(�G�l���M�[�ʁA�������A�^�[�Q�b�g�Ɍ����킹�邩)
	std::shared_ptr<CharacterBase> CreateDropEnergyBall(int,const CharacterBase*,bool);
	//�h���b�v�G�l���M�[�{�[���U�z(�G�l���M�[�ʁA�������A�^�[�Q�b�g�Ɍ����킹�邩)
	void SprayDropEnergyBall(int, const CharacterBase*, bool);
}