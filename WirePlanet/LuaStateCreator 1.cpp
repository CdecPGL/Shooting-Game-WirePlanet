#include "LuaStateCreator.h"
#include"Enemy.h"
#include"SoundManager.h"
#include"MassageWindowManager.h"
#include"Game.h"

lua_State* LuaStateCreator::CreateEnemyMoverState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
	using namespace luabind;
	module(L)[
		namespace_("Manager")[
			def("GetSoundManager",&SoundManager::GetInstance)
		],
		class_<SoundManager>("SoundManager")
			.def("PlaySound",&SoundManager::PlaySoundByID),
		class_<Enemy>("Enemy")
			.enum_("Direction")[
				value("LEFT",0),
				value("RIGHT",1)
			]
			.def("AddGravity", &Enemy::Gravity)
			.def("AddGravity",&Enemy::AddDefaultGravity)
			.def("StartAction", &Enemy::StartAction)
			.def("StartReverseAction",&Enemy::StartReverseAction)
			.def("CheckActionPlaying", &Enemy::CheckActionPlaying)
			.def("FireBullet", &Enemy::FireBullet)
			.def("GetPlayerT",&Enemy::GetTargetT)
			.def("GetPlayerR",&Enemy::GetTargetR)
			.def("GetPlayerDistanceT", &Enemy::GetPlayerDistanceT)
			.def("GetPlayerDistanceR", &Enemy::GetPlayerDistanceR)
			.def("GetPlayerDistance",&Enemy::GetTargetDistance)
			.def("GetPlayerTFromMe",&Enemy::GetPlayerTFromMe)
			.def("ChangeDirection",&Enemy::ChangeDirection)
			.def("AddForceToPlanet",&Enemy::AddForceToPlanet)
			.property("t", &Enemy::GetT, &Enemy::SetT)
			.property("r",&Enemy::GetR,&Enemy::SetR)
			.property("angle",&Enemy::GetAngle,&Enemy::SetAngle)
			.property("w", &Enemy::GetW, &Enemy::SetW)
			.property("vr", &Enemy::GetVR, &Enemy::SetVR)
			.property("ground",&Enemy::GetGroundFrag)
	];
	return L;
}

lua_State* LuaStateCreator::CreateTriggerState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);

	return L;
}

lua_State* LuaStateCreator::CreateEventState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
	using namespace luabind;
	module(L)[
		namespace_("Test")[def("test", &Game::CreateTextWindow)],
		class_<Game>("Game")
		.def("CreateTextWindow", &Game::CreateTextWindow)
		
	];
	return L;
}
