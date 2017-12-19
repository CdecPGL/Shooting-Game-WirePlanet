#pragma once

#include"Effect.h"

class Enemy;

class EnemySpawnEffect : public Effect
{
public:
	//�R���X�g���N�^(�s�N�Z���T�C�Y��)
	EnemySpawnEffect();
	~EnemySpawnEffect();
	int Init()override;
	void Update()override;
	void Draw()override;

	void SetSpawnEnemyInitID(const std::string&);
private:
	int cnt; //�J�E���^�[
	double param;
	int ghandle; //�摜�n���h��
	Vector2D<int> g_size; //�摜�T�C�Y
	double d_ext; //�`��g�嗦

	std::shared_ptr<Enemy> spawn_enemy;

	std::string enemy_iid;
};
