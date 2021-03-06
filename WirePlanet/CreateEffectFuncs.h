#pragma once

#include"Effect.h"

namespace CreateEffectFuncs
{
	//G««GtFNgì¬(f¯ÇÀpT,RA[f¯AvCA]¶¬·éGÌú»ID,[0Å©®]=0)
	std::shared_ptr<Effect> CreateEnemySpawnEffect(double,double/*,std::shared_ptr<Planet>,std::shared_ptr<Character>*/,const std::string&,int=0);
	//XRAGtFNgì¬(f¯ÇÀpT,RA\¦XRA,®f¯,F)
	std::shared_ptr<Effect> CreateScoreEffect(double, double, int, std::shared_ptr<Planet>, int);
	std::shared_ptr<Effect> CreateScoreEffect(double, double, int, std::shared_ptr<Planet>);
	//GÁÅGtFNgì¬(f¯ÇÀpT,R,®f¯)
	std::shared_ptr<Effect> CreateEnemyTerminatedEffect(double,double,std::shared_ptr<Planet>);
	//ÊíGtFNgì¬(GtFNg¼,f¯ÇÀpT,R,®f¯)
	std::shared_ptr<Effect> CreateNormalEffect(const std::string&,double, double, std::shared_ptr<Planet>);
};

