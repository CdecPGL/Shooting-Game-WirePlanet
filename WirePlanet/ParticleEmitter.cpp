#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter()
{
}


ParticleEmitter::~ParticleEmitter()
{
}

int ParticleEmitter::Init(){

	return 0;
}

int ParticleEmitter::End(){
	return 0;
}

void ParticleEmitter::Update(){

}

void ParticleEmitter::SetParticleObject(std::shared_ptr<Object> obj){
	_original_particle_object = obj;
}
