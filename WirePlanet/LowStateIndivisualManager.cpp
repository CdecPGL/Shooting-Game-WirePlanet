#include "LowStateIndivisualManager.h"

LowStateIndivisualManager::LowStateIndivisualManager() :current_lowstate(nullptr){}

void LowStateIndivisualManager::Update(){
	if (current_lowstate){ (*current_lowstate)->Update(); }
}

int LowStateIndivisualManager::ChangeLowState(const std::string& n){
	if (current_lowstate){ (*current_lowstate)->Leave(); }
	auto it = ls_map.find(n);
	if (it == ls_map.end()){ return -1; }
	current_lowstate = &(it->second);
	(*current_lowstate)->Enter();
	return 0;
}