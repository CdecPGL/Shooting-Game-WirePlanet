#pragma once

#include"INIData.h"

class INISaver{
public:
	INISaver();
	~INISaver();
	static int SaveINIData(const std::string&,const INIData&);
};