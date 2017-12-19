#pragma once
#include "object.h"
class ParticleEmitter :
	public Object
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	int Init()override;
	int End()override;
	void Update()override;

	void SetParticleObject(std::shared_ptr<Object>); //�p�[�Ă�����I�u�W�F�N�g�Z�b�g
private:
	std::shared_ptr<Object> _original_particle_object; //���̗��q�I�u�W�F�N�g

	int _suspend; //��������

	int _inc_cnt; //�����J�E���^
};
