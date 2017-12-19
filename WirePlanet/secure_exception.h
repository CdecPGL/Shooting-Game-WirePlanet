#pragma once

#include<exception>
#include<string>

namespace secure_obj{
	class cheat_detected : public std::exception{
	public:
		cheat_detected(const std::string& err) :std::exception(err.c_str()){};
	};
}