#pragma once

#include"Planet.h"
#include"background.h"

namespace DefaultGameObjects
{
	std::shared_ptr<Planet> GetDefaultPlanet();
	std::shared_ptr<BackGround> GetDefaultBackGround();
};
