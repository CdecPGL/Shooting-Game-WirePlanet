#include "GraphicManager.h"
#include"SVLoader.h"
#include"FileLoadUtility.h"

using namespace std;

const string GRAPHIC_FILE_LIST("graph_list.csv");

GraphicManager::GraphicManager()
{
	printf("GraphicManagerのインスタンスが作成されました。\n");
}

GraphicManager::~GraphicManager()
{
	printf("GraphicManagerのインスタンスが破棄されました。\n");
}

GraphicManager& GraphicManager::GetInstance(){
	static GraphicManager gm;
	return gm;
}

int GraphicManager::Init(){
	int res = LoadFile();
	if (res){
		printf("GraphicManagerの初期化に失敗しました。(%d)\n", res);
		return res;
	}
	printf("GraphicManagerが初期化されました。\n");
	return 0;
}

int GraphicManager::GetGraphHandle(const string& gn)const{
	auto it = _ghandles.find(gn);
	if (it == _ghandles.end()){
		printf("GraphicManager::GetGraphHandle 存在しない画像が指定されました。(%s)\n",gn.c_str());
		return -1;
	}
	else{
		return it->second;
	}
}

bool GraphicManager::IsGraphExist(const std::string& name)const{
	return _ghandles.find(name) == _ghandles.end() ? false : true;
}

const unordered_map<string, int>& GraphicManager::GetGhandleList()const{ return _ghandles; }

int GraphicManager::LoadFile(){
	//リストを取得、読み込み
	CSVLoader csvl;
	int res;
	if ((res = FileLoadUtil::LoadCSVFromFLM(&csvl, GRAPHIC_FILE_LIST, true))){
		printf("リストを読み込めませんでした。(reason:%d,name:%s)\n", res, GRAPHIC_FILE_LIST.c_str());
		return -1;
	}
	for (vector<string>& s : csvl){
		if (s.size() != 2){
			printf("リストの項目数が不正です。(%d/2)\n", s.size());
			continue;
		}
		int gh = FileLoadUtil::CreateGraphFromFLM(s[1], true);
		if (gh < 0){
			printf("指定された画像が読み込めませんでした。(reason:%d ID:%s pass:%s)\n", gh, s[0].c_str(), (/*dir + */s[1]).c_str());
			continue;
		}
		_ghandles.insert(unordered_map<string, int>::value_type(s[0], gh));
	}
	return 0;
}