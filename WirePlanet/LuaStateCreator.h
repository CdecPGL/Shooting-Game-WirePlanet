#pragma once

#include"lua.hpp"
#include"luabind/luabind.hpp"

namespace LuaStateCreator
{
	lua_State* CreateEnemyMoverState();
	lua_State* CreateTriggerState();
	lua_State* CreateEventState();
	lua_State* CreateGameInitializerState();
};

