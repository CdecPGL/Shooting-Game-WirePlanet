#include"INISaver.h"

#include<fstream>

INISaver::INISaver(){}
INISaver::~INISaver(){}

int INISaver::SaveINIData(const std::string& fn, const INIData& inid){
	using namespace std;
	ofstream ofs(fn);
	if (ofs.fail()){ return -1; }
	for (const auto& section : inid){
		ofs << "[" << section.first << "]" << endl;
		for (const auto& dat : section.second){
			ofs << dat.first << " = " << dat.second << endl;
		}
	}
	return 0;
}