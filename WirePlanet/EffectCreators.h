#pragma once

#include"NormalEffectCreator.h"

class EnemySpawnEffectCreator :
	public CreatorTemplate < Effect >
{
public:
	EnemySpawnEffectCreator() = default;
	~EnemySpawnEffectCreator() = default;
	std::shared_ptr<Effect> Create(const std::string&)override;
};

class ScoreEffectCreator :
	public CreatorTemplate < Effect >
{
public:
	ScoreEffectCreator() = default;
	~ScoreEffectCreator() = default;
	std::shared_ptr<Effect> Create(const std::string&)override;
};
