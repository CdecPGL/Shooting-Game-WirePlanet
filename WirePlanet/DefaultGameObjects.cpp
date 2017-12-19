#include "DefaultGameObjects.h"
#include"WirePlanet.h"

using namespace std;

const double PLANET_MASS(2000.0);
const double PLANET_FRICTION(1.0);
const double PLANET_RADIUS(180.0);
const int PLANET_SEPARATION(120);
const int PLANET_COLOR_R(255);
const int PLANET_COLOR_G(255);
const int PLANET_COLOR_B(255);

shared_ptr<Planet> DefaultGameObjects::GetDefaultPlanet(){
	shared_ptr<WirePlanet> planet = make_shared<WirePlanet>(PLANET_MASS, PLANET_FRICTION, PLANET_RADIUS, PLANET_SEPARATION);
	planet->SetXY(Vector2D<double>(400, 400));
	planet->SetColor(PLANET_COLOR_R, PLANET_COLOR_G, PLANET_COLOR_B);
	if (planet->Init()){ return nullptr; }
	return planet;
}

shared_ptr<BackGround> DefaultGameObjects::GetDefaultBackGround(){
	return make_shared<BackGround>();
}
