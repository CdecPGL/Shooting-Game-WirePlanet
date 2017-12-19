#include "LuaStateCreator.h"
#include"Enemy.h"
#include"SoundManager.h"
#include"MassageWindowManager.h"
#include"Game.h"
#include"UtilityFuncsForLua.h"

lua_State* LuaStateCreator::CreateEnemyMoverState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
	using namespace luabind;
	module(L)[
		namespace_("GameSystem")[
			def("PrintToConsole",&UtilityFuncsForLua::PrintToConsole),
			def("PrintToScreen", &UtilityFuncsForLua::PrintToScreen),
			def("PrintToConsoleAndScreen", &UtilityFuncsForLua::PrintToConsoleAndScreen)
		],
		//音関連
		namespace_("GameSound")[
			def("CreateSoundPlayHandle", &UtilityFuncsForLua::CreateSoundPlayHandle),
			def("DeleteSoundPlayHandle", &UtilityFuncsForLua::DeleteSoundPlayHandle),
			def("PlaySoundByHandle", &UtilityFuncsForLua::PlaySoundBySPHandle),
			def("StopSoundByHandle", &UtilityFuncsForLua::StopSoundBySPHandle),
			def("CheckSoundPlayHandle", &UtilityFuncsForLua::CheckSoundPlayingBySPHandle)
		],
		namespace_("Manager")[
			def("GetSoundManager",&SoundManager::GetInstance)
		],
		class_<Vector2D<double>>("Vector2D")
			.def(constructor<>())
			.def(constructor<double,double>())
			.def("Set",&Vector2D<double>::Set)
			.def_readwrite("x",&Vector2D<double>::x)
			.def_readwrite("y", &Vector2D<double>::y),
		class_<vector<Vector2D<double>>>("vector")
			.def(constructor<>())
			.def("push_back",static_cast<void(vector<Vector2D<double>>::*)(const Vector2D<double>&)>(&vector<Vector2D<double>>::push_back)),
		class_<SoundManager>("SoundManager")
			.def("PlaySound",&SoundManager::PlaySoundByID),
		class_<Object>("Object")
			.def("AddGravity", &Object::Gravity)
			.def("GetTargetT", &Object::GetTargetT)
			.def("GetTargetR", &Object::GetTargetR)
			.def("GetTargetDistance", &Object::GetTargetDistance)
			.def("GetTargetTFromMe", &Object::GetTargetTFromMe)
			.def("GetTargetDirectionByDeg", &Object::GetTargetDirectionByDeg)
			.def("IsAutoMoving", &Object::IsAutoMoving)
			.def("StartLinearMove", &Object::StartLinearMove)
			.def("StartHermiteMove", &Object::StartHermiteMove)
			.def("StartBezierMove", &Object::StartBezierMove)
			.property("t", &Object::GetT, &Object::SetT)
			.property("r", &Object::GetR, &Object::SetR)
			.property("rotation", &Object::GetRotation, &Object::SetRotation)
			.property("rota_vel", &Object::GetRotationVelocity, &Object::SetRatationVelocity)
			.property("vh", &Object::GetVH, &Object::SetVH)
			.property("vr", &Object::GetVR, &Object::SetVR),
		class_<Enemy,Object>("Enemy")
			.enum_("Direction")[
				value("LEFT",0),
				value("RIGHT",1)
			]
			.def("AddGravity",&Enemy::AddDefaultGravity)
			.def("StartAction", &Enemy::StartAction)
			.def("StartReverseAction",&Enemy::StartReverseAction)
			.def("CheckActionPlaying", &Enemy::CheckActionPlaying)
			.def("FireBullet", &Enemy::FireBullet)
			.def("FireRotatingBullet",&Enemy::FireRotatingBullet)
			.def("FireLaser",&Enemy::FireLaser)
			.def("CreateCharacter",&Enemy::CreateCharacter)
			.def("GetTargetDistanceT", &Enemy::GetTargetDistanceT)
			.def("GetTargetDistanceR", &Enemy::GetTargetDistanceR)
			.def("ChangeDirection",&Enemy::ChangeDirection)
			.def("AddForceToPlanet",&Enemy::AddForceToPlanet)
			.def("Friction",&Enemy::ProcessFriction)
			.def("GetBelongingPlanetRadius",&Enemy::GetBelongingPlanetRadius)
			.def("Landed",&Enemy::Landed)
			.def("GetObject",&Enemy::GetMCObject)
			.def("EndObject",&Enemy::EndMCObject)
			.def("GetHPPercentage",&Enemy::GetHPPercentage)
			.property("ground",&Enemy::GetGroundFrag)
			.property("reverse",&Enemy::GetReverseFlag)
	];
	return L;
}

lua_State* LuaStateCreator::CreateTriggerState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
	using namespace luabind;
	module(L)[
		namespace_("GameSystem")[
			def("PrintToConsole", &UtilityFuncsForLua::PrintToConsole),
			def("PrintToScreen", &UtilityFuncsForLua::PrintToScreen),
			def("PrintToConsoleAndScreen", &UtilityFuncsForLua::PrintToConsoleAndScreen)
		],
		class_<Game>("Game")
			.def("CheckLevelChangeEffectPlaying",&Game::CheckLevelChangeEffectPlaying)
			.def("CheckEnemyGenerating",&Game::CheckEnemyGenerating)
			.def("CheckTalkWindowActive",&Game::CheckTalkWindowActive)
			.def("CheckEnemyCountZero",&Game::CheckEnemyCountZero)
			.def("CheckReadyToGenerateEnemy",&Game::CheckReadyToGenerateEnemy)
			.def("CheckPlanetDanger",&Game::CheckPlanetDanger)
			.def("CheckPlanetDestroy",&Game::CheckPlanetDestroy)
			.def("CheckFullScreenGraphDrawing",&Game::IsGraphFullScreenDrawing)
	];
	return L;
}

lua_State* LuaStateCreator::CreateEventState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
	using namespace luabind;
	module(L)[
		namespace_("GameSystem")[
			def("PrintToConsole", &UtilityFuncsForLua::PrintToConsole),
			def("PrintToScreen", &UtilityFuncsForLua::PrintToScreen),
			def("PrintToConsoleAndScreen", &UtilityFuncsForLua::PrintToConsoleAndScreen)
		],
		//音関連
		namespace_("GameSound")[
			def("PlayMusic", &UtilityFuncsForLua::PlayMusic),
			def("PlayNoLoopMusic", &UtilityFuncsForLua::PlayNoLoopMusic),
			def("StopMusic", &UtilityFuncsForLua::StopMusic),
			def("IsMusicPlaying",&UtilityFuncsForLua::IsMusicPlaying),
			def("IsAssignedMusicPlaying", &UtilityFuncsForLua::IsAssignedMusicPlaying),
			def("PlaySoundEffect", &UtilityFuncsForLua::PlaySoundEffect),
			def("CreateSoundPlayHandle", &UtilityFuncsForLua::CreateSoundPlayHandle),
			def("DeleteSoundPlayHandle", &UtilityFuncsForLua::DeleteSoundPlayHandle),
			def("PlaySoundByHandle", &UtilityFuncsForLua::PlaySoundBySPHandle),
			def("StopSoundByHandle", &UtilityFuncsForLua::StopSoundBySPHandle),
			def("CheckSoundPlayHandle", &UtilityFuncsForLua::CheckSoundPlayingBySPHandle)
		],
		class_<Vector2D<double>>("Vector2D")
			.def(constructor<>())
			.def(constructor<double, double>())
			.def("Set", &Vector2D<double>::Set)
			.def_readwrite("x", &Vector2D<double>::x)
			.def_readwrite("y", &Vector2D<double>::y),
		class_<vector<Vector2D<double>>>("std_vector")
			.def(constructor<>())
			.def("push_back", static_cast<void(vector<Vector2D<double>>::*)(const Vector2D<double>&)>(&vector<Vector2D<double>>::push_back)),
		//オブジェクトクラス
		class_<Object>("Object")
			.def("AddGravity", &Object::Gravity)
			.def("GetTargetT", &Object::GetTargetT)
			.def("GetTargetR", &Object::GetTargetR)
			.def("GetTargetDistance", &Object::GetTargetDistance)
			.def("GetTargetTFromMe", &Object::GetTargetTFromMe)
			.def("IsAutoMoving", &Object::IsAutoMoving)
			.def("StartLinearMove", &Object::StartLinearMove)
			.def("StartHermiteMove", &Object::StartHermiteMove)
			.def("StartBezierMove", &Object::StartBezierMove)
			.property("t", &Object::GetT, &Object::SetT)
			.property("r", &Object::GetR, &Object::SetR)
			.property("rotation", &Object::GetRotation, &Object::SetRotation)
			.property("rota_vel", &Object::GetRotationVelocity, &Object::SetRatationVelocity)
			.property("vh", &Object::GetVH, &Object::SetVH)
			.property("vr", &Object::GetVR, &Object::SetVR),
		//キャラクラス
		class_<Character,Object>("Character")
			.enum_("Mode")[
				value("StandAlone", 0),
				value("Control", 1)
			]
			.enum_("Direction")[
				value("LEFT", 0),
					value("RIGHT", 1)
			]
			.def("StartAction",&Character::StartAction)
			.def("ChangeMode",static_cast<void(Character::*)(int)>(&Character::SetMode))
			.def("ChangeDirection", &Character::ChangeDirection)
			.def("SetReverse",&Character::SetReverse),
		//ゲームクラス
		class_<Game>("Game")
			//状態変更関数
			.enum_("PauseKind")[
				value("Normal", (int)Game::PauseKind::Normal),
				value("CantReturn", (int)Game::PauseKind::CantReturn),
				value("OnlyContinue", (int)Game::PauseKind::OnlyContinue)
			]
			.def("SetPauseKind",&Game::SetPauseKind)
			.def("CreateTextWindow", &Game::CreateTextWindow)
			.def("CreateTextWindowInTop", &Game::CreateTextWindowInTop)
			.def("CreateSingleLineWindow",&Game::CreateSingleLineWindow)
			.def("CreateSingleLineWindowBS", &Game::CreateSingleLineWindowBS)
			.def("CreateSingleLineWindowWS", &Game::CreateSingleLineWindowWS)
			.def("GetMWGlobalVariable",&Game::GetMWGlobalVariable)
			.def("GetLastSelectedIndexMW",&Game::GetLastSelectedIndexMW)
			.def("StartLevelChangeEffect",&Game::StartLevelChangeEffect)
			.def("SetEnemyGeneration",&Game::SetEnemyGenaration)
			.def("StartEnemyGeneration",&Game::StartEnemyGeneration)
			.def("CreateSingleEventTrigger",static_cast<int(Game::*)(const char*,const char*)>(&Game::CreateSingleEventTrigger))
			.def("CreateSingleEventTriggerWithParam", static_cast<int(Game::*)(const char*, const char*,int)>(&Game::CreateSingleEventTrigger))
			.def("ChangeBackGround",&Game::ChangeBackGround)
			.def("GetGenerateEnemyCount",&Game::GetGenerateEnemyCount)
			.def("StartGameScreenEffect",&Game::StartGameScreenEffect)
			.def("StopGameScreenEffect",&Game::StopGameScreenEffect)
			.def("StartFullScreenGraphDrawing",&Game::AttachGraphFullScreen)
			.def("StopFullScreenGraphDrawing",&Game::DetachGraphFullScreen)
			.def("SetPlayerOperableFlag",&Game::SetPlayerOperableFlag)
			.def("GetEventCharacter",&Game::GetEventCharacter)
			.def("DetachEventCharacter",&Game::DetachEventCharacter)
			.def("CreateEnemy",&Game::CreateEnemy)
			.def("EndEventCharacter",&Game::EndEventCharacter)
			.def("UnresistEventCharacterFromManager",&Game::UnresistEventCharacterFromManager)
			.def("ChangeCameraMode",&Game::ChangeCameraMode)
			.def("ChangeCameraTargetFromEveChr", &Game::ChangeCameraTargetFromEventChara)
			.def("ChangeCameraTargetFromEveObj", &Game::ChangeCameraTargetFromEventObject)
			.def("ResistEventCharacterToManager",&Game::ResistEventCharacterToManager)
			.def("SetCameraVibration",&Game::SetCameraVibration)
			.def("CreateGraphicObject",&Game::CreateGraphicObject)
			.def("GetEventObject",&Game::GetEventObject)
			.def("DeleteEventObject",&Game::DeleteEventObject)
			.def("ClearEventObject",&Game::ClearEventObject)
			.def("CreatePlayer",&Game::CreatePlayer)
			.def("SetCameraViewArea", &Game::SetCameraViewArea)
			.def("GameOvered",&Game::GameOvered)
			.def("GameEnd",&Game::GameEnd)
			.def("ContinueGame",&Game::ContinueGame)
			.def("GameCleared",&Game::GameCleared)
			.def("SavePlayerEnergy",&Game::SavePlayerEnergy)
			//スコア関連
			.def("StartResultCounting", &Game::StartResultCounting)
			.def("IsCountingResult",&Game::IsCountingResult)
			.def("StartWaveEndScoreCounting", &Game::StartWaveEndScoreCounting)
			.def("IsCountingWaveEndScore",&Game::IsCountingWaveEndScore)
			//有効化関数
			.def("ActivateGameComponent", &Game::ActivateGameComponent)
			//状態確認関数
			.def("CheckLevelChangeEffectPlaying", &Game::CheckLevelChangeEffectPlaying)
			.def("CheckEnemyGenerating", &Game::CheckEnemyGenerating)
			.def("CheckEnemyGeneratePrepairing",&Game::CheckEnemyGeneratePrepairing)
			.def("CheckTalkWindowActive", &Game::CheckTalkWindowActive)
			.def("CheckEnemyCountZero", &Game::CheckEnemyCountZero)
			.def("CheckReadyToGenerateEnemy", &Game::CheckReadyToGenerateEnemy)
			.def("CheckPlanetDanger", &Game::CheckPlanetDanger)
			.def("CheckPlanetDestroy", &Game::CheckPlanetDestroy)
			.def("CheckFullScreenGraphDrawing", &Game::IsGraphFullScreenDrawing)
			.def("CheckUsingPlayerDead",&Game::CheckUsingPlayerDead)
			.def("CheckContinued",&Game::CheckContinued)
	];
	return L;
}

lua_State* LuaStateCreator::CreateGameInitializerState(){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
	using namespace luabind;
	module(L)[
		namespace_("GameSystem")[
			def("PrintToConsole", &UtilityFuncsForLua::PrintToConsole),
			def("PrintToScreen", &UtilityFuncsForLua::PrintToScreen),
			def("PrintToConsoleAndScreen", &UtilityFuncsForLua::PrintToConsoleAndScreen)
		],
		//音関連
		namespace_("GameSound")[
			def("PlayMusic", &UtilityFuncsForLua::PlayMusic)
		],
		class_<Vector2D<double>>("Vector2D")
		.def(constructor<>())
		.def(constructor<double, double>())
		.def("Set", &Vector2D<double>::Set)
		.def_readwrite("x", &Vector2D<double>::x)
		.def_readwrite("y", &Vector2D<double>::y),
		class_<vector<Vector2D<double>>>("std_vector")
		.def(constructor<>())
		.def("push_back", static_cast<void(vector<Vector2D<double>>::*)(const Vector2D<double>&)>(&vector<Vector2D<double>>::push_back)),
		//オブジェクトクラス
		class_<Object>("Object")
		.def("AddGravity", &Object::Gravity)
		.def("GetTargetT", &Object::GetTargetT)
		.def("GetTargetR", &Object::GetTargetR)
		.def("GetTargetDistance", &Object::GetTargetDistance)
		.def("GetTargetTFromMe", &Object::GetTargetTFromMe)
		.property("t", &Object::GetT, &Object::SetT)
		.property("r", &Object::GetR, &Object::SetR)
		.property("rotation", &Object::GetRotation, &Object::SetRotation)
		.property("rota_vel", &Object::GetRotationVelocity, &Object::SetRatationVelocity)
		.property("vh", &Object::GetVH, &Object::SetVH)
		.property("vr", &Object::GetVR, &Object::SetVR),
		//キャラクラス
		class_<Character, Object>("Character")
		.enum_("Mode")[
			value("StandAlone", 0),
				value("Control", 1)
		]
		.enum_("Direction")[
			value("LEFT", 0),
				value("RIGHT", 1)
		]
		.def("StartAction", &Character::StartAction)
		.def("ChangeMode", static_cast<void(Character::*)(int)>(&Character::SetMode))
		.def("ChangeDirection", &Character::ChangeDirection),
		//ゲームクラス
		class_<Game>("Game")
		//状態変更関数
		.def("CreateSingleEventTrigger", static_cast<int(Game::*)(const char*, const char*)>(&Game::CreateSingleEventTrigger))
		.def("CreateSingleEventTriggerWithParam", static_cast<int(Game::*)(const char*, const char*, int)>(&Game::CreateSingleEventTrigger))
		.def("ChangeBackGround", &Game::ChangeBackGround)
		.def("StartGameScreenEffect", &Game::StartGameScreenEffect)
		.def("StartFullScreenGraphDrawing", &Game::AttachGraphFullScreen)
		.def("SetPlayerOperableFlag", &Game::SetPlayerOperableFlag)
		.def("GetEventCharacter", &Game::GetEventCharacter)
		.def("CreateEnemy", &Game::CreateEnemy)
		.def("UnresistEventCharacterFromManager", &Game::UnresistEventCharacterFromManager)
		.def("ChangeCameraMode", &Game::ChangeCameraMode)
		.def("ChangeCameraTargetFromEveChr", &Game::ChangeCameraTargetFromEventChara)
		.def("ChangeCameraTargetFromEveObj", &Game::ChangeCameraTargetFromEventObject)
		.def("ResistEventCharacterToManager", &Game::ResistEventCharacterToManager)
		.def("SetCameraVibration", &Game::SetCameraVibration)
		.def("CreateGraphicObject", &Game::CreateGraphicObject)
		.def("GetEventObject", &Game::GetEventObject)
		.def("CreatePlayer", &Game::CreatePlayer)
		.def("ResistPlayer", &Game::ResistPlayer)
		.def("ChangePlayer",&Game::ChangePlayer)
		.def("SetCameraViewArea",&Game::SetCameraViewArea)
		//有効化関数
		.def("ActivateGameComponent", &Game::ActivateGameComponent)
		//初期化関数(必要なら必ずすべての関数の前に呼び出す)
//		.def("SetupPlanet",&Game::SetupPlanet)
		.def("SetupBackGround",&Game::SetupBackGround)
		.def("SetupInformationArea",&Game::SetupInformationArea)
	];
	return L;
}
