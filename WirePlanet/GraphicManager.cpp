#include "GraphicManager.h"
#include"SVLoader.h"
#include"FileLoadUtility.h"

using namespace std;

const string GRAPHIC_FILE_LIST("graph_list.csv");

GraphicManager::GraphicManager()
{
	printf("GraphicManager�̃C���X�^���X���쐬����܂����B\n");
}

GraphicManager::~GraphicManager()
{
	printf("GraphicManager�̃C���X�^���X���j������܂����B\n");
}

GraphicManager& GraphicManager::GetInstance(){
	static GraphicManager gm;
	return gm;
}

int GraphicManager::Init(){
	int res = LoadFile();
	if (res){
		printf("GraphicManager�̏������Ɏ��s���܂����B(%d)\n", res);
		return res;
	}
	printf("GraphicManager������������܂����B\n");
	return 0;
}

int GraphicManager::GetGraphHandle(const string& gn)const{
	auto it = _ghandles.find(gn);
	if (it == _ghandles.end()){
		printf("GraphicManager::GetGraphHandle ���݂��Ȃ��摜���w�肳��܂����B(%s)\n",gn.c_str());
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
	//���X�g���擾�A�ǂݍ���
	CSVLoader csvl;
	int res;
	if ((res = FileLoadUtil::LoadCSVFromFLM(&csvl, GRAPHIC_FILE_LIST, true))){
		printf("���X�g��ǂݍ��߂܂���ł����B(reason:%d,name:%s)\n", res, GRAPHIC_FILE_LIST.c_str());
		return -1;
	}
	for (vector<string>& s : csvl){
		if (s.size() != 2){
			printf("���X�g�̍��ڐ����s���ł��B(%d/2)\n", s.size());
			continue;
		}
		int gh = FileLoadUtil::CreateGraphFromFLM(s[1], true);
		if (gh < 0){
			printf("�w�肳�ꂽ�摜���ǂݍ��߂܂���ł����B(reason:%d ID:%s pass:%s)\n", gh, s[0].c_str(), (/*dir + */s[1]).c_str());
			continue;
		}
		_ghandles.insert(unordered_map<string, int>::value_type(s[0], gh));
	}
	return 0;
}