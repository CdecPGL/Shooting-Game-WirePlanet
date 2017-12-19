#pragma once

#include<unordered_map>
#include<string>

class GraphicManager
{
public:
	GraphicManager(const GraphicManager&) = delete;
	~GraphicManager();
	static GraphicManager& GetInstance();
	int Init();
	int GetGraphHandle(const std::string&)const;
	bool IsGraphExist(const std::string&)const;
	const std::unordered_map<std::string, int>& GetGhandleList()const;

private:
	GraphicManager();
	std::unordered_map<std::string, int> _ghandles;
	int LoadFile();
};
