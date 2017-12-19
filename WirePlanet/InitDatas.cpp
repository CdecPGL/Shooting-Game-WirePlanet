#include "InitDatas.h"
#include"INILoader.h"

using namespace std;

class InitDataHolder{
public:
	static INILoader inil;
};

INILoader InitDataHolder::inil;

int InitDatas::LoadInitFile(){
	if (InitDataHolder::inil.LoadINI(InitFileName)){
		string e = "初期データ定義ファイル\"";
		e += InitFileName;
		e += "\"を読み込めませんでした。";
		throw init_datas_error(e);
		return -1;
	}
	return 0;
}

unordered_map<string, string> InitDatas::GetInitDatas(const string& n){
	INILoader& inil = InitDataHolder::inil;
	if (inil.CheckSectionExistence(n) == false){
		string e = "指定されたセクション\"";
		e += n;
		e += "\"は存在しません。";
		throw init_datas_error(e);
	}
	return inil.GetSection(n);
}

InitDatas::init_datas_error::init_datas_error(const string& e) :domain_error(e){}