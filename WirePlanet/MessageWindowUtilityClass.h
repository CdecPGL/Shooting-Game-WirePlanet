#pragma once
#include<string>
#include<unordered_map>

struct SpeakerData{
	SpeakerData(const std::string&);
	std::string name;
	std::unordered_map<std::string, int> graphics;
	int default_graphic;
};
