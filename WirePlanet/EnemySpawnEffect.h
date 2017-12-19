#pragma once

#include"Effect.h"

class Enemy;

class EnemySpawnEffect : public Effect
{
public:
	//コンストラクタ(ピクセルサイズ幅)
	EnemySpawnEffect();
	~EnemySpawnEffect();
	int Init()override;
	void Update()override;
	void Draw()override;

	void SetSpawnEnemyInitID(const std::string&);
private:
	int cnt; //カウンター
	double param;
	int ghandle; //画像ハンドル
	Vector2D<int> g_size; //画像サイズ
	double d_ext; //描画拡大率

	std::shared_ptr<Enemy> spawn_enemy;

	std::string enemy_iid;
};
