#pragma once

#include"Effect.h"

namespace CreateEffectFuncs
{
	//�G�����G�t�F�N�g�쐬(�f���Ǎ��pT,R�A[�f���A�v���C���A]��������G�̏�����ID,��[0�Ŏ���]=0)
	std::shared_ptr<Effect> CreateEnemySpawnEffect(double,double/*,std::shared_ptr<Planet>,std::shared_ptr<Character>*/,const std::string&,int=0);
	//�X�R�A�G�t�F�N�g�쐬(�f���Ǎ��pT,R�A�\���X�R�A,�����f��,�F)
	std::shared_ptr<Effect> CreateScoreEffect(double, double, int, std::shared_ptr<Planet>, int);
	std::shared_ptr<Effect> CreateScoreEffect(double, double, int, std::shared_ptr<Planet>);
	//�G���ŃG�t�F�N�g�쐬(�f���Ǎ��pT,R,�����f��)
	std::shared_ptr<Effect> CreateEnemyTerminatedEffect(double,double,std::shared_ptr<Planet>);
	//�ʏ�G�t�F�N�g�쐬(�G�t�F�N�g��,�f���Ǎ��pT,R,�����f��)
	std::shared_ptr<Effect> CreateNormalEffect(const std::string&,double, double, std::shared_ptr<Planet>);
};

