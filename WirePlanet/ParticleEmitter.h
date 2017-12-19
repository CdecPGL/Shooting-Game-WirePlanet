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

	void SetParticleObject(std::shared_ptr<Object>); //パーてぅくるオブジェクトセット
private:
	std::shared_ptr<Object> _original_particle_object; //元の粒子オブジェクト

	int _suspend; //持続時間

	int _inc_cnt; //増加カウンタ
};
