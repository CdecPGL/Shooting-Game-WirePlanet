#pragma once

#include"Effect.h"

namespace CreateEffectFuncs
{
	//敵性性エフェクト作成(惑星局座用T,R、[惑星、プレイヤ、]生成する敵の初期化ID,幅[0で自動]=0)
	std::shared_ptr<Effect> CreateEnemySpawnEffect(double,double/*,std::shared_ptr<Planet>,std::shared_ptr<Character>*/,const std::string&,int=0);
	//スコアエフェクト作成(惑星局座用T,R、表示スコア,所属惑星,色)
	std::shared_ptr<Effect> CreateScoreEffect(double, double, int, std::shared_ptr<Planet>, int);
	std::shared_ptr<Effect> CreateScoreEffect(double, double, int, std::shared_ptr<Planet>);
	//敵消滅エフェクト作成(惑星局座用T,R,所属惑星)
	std::shared_ptr<Effect> CreateEnemyTerminatedEffect(double,double,std::shared_ptr<Planet>);
	//通常エフェクト作成(エフェクト名,惑星局座用T,R,所属惑星)
	std::shared_ptr<Effect> CreateNormalEffect(const std::string&,double, double, std::shared_ptr<Planet>);
};

