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
		string e = "�����f�[�^��`�t�@�C��\"";
		e += InitFileName;
		e += "\"��ǂݍ��߂܂���ł����B";
		throw init_datas_error(e);
		return -1;
	}
	return 0;
}

unordered_map<string, string> InitDatas::GetInitDatas(const string& n){
	INILoader& inil = InitDataHolder::inil;
	if (inil.CheckSectionExistence(n) == false){
		string e = "�w�肳�ꂽ�Z�N�V����\"";
		e += n;
		e += "\"�͑��݂��܂���B";
		throw init_datas_error(e);
	}
	return inil.GetSection(n);
}

InitDatas::init_datas_error::init_datas_error(const string& e) :domain_error(e){}