#pragma once

#include<string>
#include<unordered_map>
#include<exception>

const std::string InitFileName("wireplanet.ini");

namespace InitDatas{
	int LoadInitFile();
	std::unordered_map<std::string, std::string> GetInitDatas(const std::string&);
	//—áŠO
	class init_datas_error : public std::domain_error{
	public:
		init_datas_error(const std::string&);
	};
}
