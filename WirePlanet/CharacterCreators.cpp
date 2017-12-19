#include"CharacterCreators.h"
#include"PlayerB.h"
#include"Enemy.h"
#include"define.h"
#include"SVLoader.h"
#include"boost/lexical_cast.hpp"
#include"DropedEnergyBall.h"
#include"FileLoadUtility.h"

const char* PlayerB_Action_ID = "Player"; //�v���C���[�̃A�N�V�����t�@�C��
const double Player_Friction(2.0); //�v���C���[�̖��C
const int Player_MatchlessTime(60); //�v���C���[�̖��G����
const double PLAYER_MAX_SPEED(5); //�v���C���[�̍ő呬�x
const std::string Enemy_Init_Data_File("EnemyData.csv"); //�G�������f�[�^�t�@�C��
const int Enemy_Init_Param_Count(6); //�G�̏������f�[�^�v�f��

using namespace std;

//�v���C���[�N���G�[�^
int PlayerCreator::Init(){
	LoadInitData();
	printf("PlayerCreator�����������܂����B\n");
	CreatorTemplate<CharacterBase>::Init();
	return 0;
}

shared_ptr<CharacterBase> PlayerCreator::Create(const string& init_id){
	auto it = init_datas.find(init_id);
	if (it == init_datas.end()){ return nullptr; }
	InitData& ind = (*it).second;
	if (init_id == "Player"){
		return make_shared<PlayerB>(ind.friction,ind.HP,  ind.max_speed, ind.matchless_time, ind.action_file);
	}
	return nullptr;
}

bool PlayerCreator::CheckInitIDExistence(const string& init_id)const{
	if (init_datas.find(init_id) == init_datas.end()){ return false; }
	return true;
}

int PlayerCreator::LoadInitData(){
	using IniDatP = unordered_map<string, InitData>::value_type;
	init_datas.insert(IniDatP("Player", InitData(100,  Player_Friction, PLAYER_MAX_SPEED, Player_MatchlessTime, PlayerB_Action_ID)));

	return 0;
}

PlayerCreator::InitData::InitData(int m_hp, double m_f,double m_ms, int m_mt, const string& m_af)
	:HP(m_hp), friction(m_f),max_speed(m_ms), matchless_time(m_mt), action_file(m_af){};

//�G�N���G�[�^
int EnemyCreator::Init(){
	int error = 0;
	if (LoadInitData()){ error = -1; }
	if (error == 0){
		printf("EnemyCreator�����������܂����B\n");
		CreatorTemplate<CharacterBase>::Init();
	}
	else if (error == -1){
		printf("EnemyCreator�̏������Ɏ��s���܂����B\n");
	}
	return error;
}

shared_ptr<CharacterBase> EnemyCreator::Create(const string& init_id){
	auto it = init_datas.find(init_id);
	if (it == init_datas.end()){ return nullptr; }
	InitData& ind = (*it).second;
	return make_shared<Enemy>(ind.HP, ind.friction, ind.matchless_time,ind.drop_energy,ind.mover_id, ind.action_file);
}

bool EnemyCreator::CheckInitIDExistence(const string& init_id)const{
	if (init_datas.find(init_id) == init_datas.end()){ return false; }
	return true;
}

int EnemyCreator::LoadInitData(){
	CSVLoader csvl;
	if (FileLoadUtil::LoadCSVFromFLM(&csvl,Enemy_Init_Data_File,true)){ return -1; }
	for (auto it = csvl.begin() + 1; it != csvl.end(); ++it){
		if ((*it).size() != Enemy_Init_Param_Count + 1){ 
			printf("EnemyCreator::LoadInitData()�ŃG���[���������܂����B: �t�@�C������ǂݍ��񂾃f�[�^�̃p�����[�^������v���܂���B�X�L�b�v���Ď��̃f�[�^��ǂݍ��݂܂��B(%s:%d/%d)\n",(*it).size()==0?"NULL":(*it)[0].c_str(), (*it).size(), Enemy_Init_Param_Count + 1);
			continue;
		}
		vector<string>& i = (*it);
		using namespace boost;
		try{
			InitData ninid(lexical_cast<int>(i[1]), lexical_cast<double>(i[2]), lexical_cast<int>(i[3]), lexical_cast<int>(i[4]), i[5], i[6]);
			init_datas.insert(pair<string, InitData>(i[0], ninid));
		}
		catch (bad_lexical_cast&){
			printf("EnemyCreator::LoadInitData()�ŃG���[���������܂����B: �������f�[�^�̕ϊ��Ɏ��s���܂����B\n");
			return -1;
		}
	}
	return 0;
}

EnemyCreator::InitData::InitData(int m_hp, double m_f, int m_mt,int m_de,const string& m_mvr, const string& m_af)
:HP(m_hp),friction(m_f),matchless_time(m_mt),mover_id(m_mvr),action_file(m_af),drop_energy(m_de){};

//�A�C�e�����肦�[��
int ItemCreator::Init(){
	LoadInitData();
	printf("ItemCreator�����������܂����B\n");
	CreatorTemplate<CharacterBase>::Init();
	return 0;
}

shared_ptr<CharacterBase> ItemCreator::Create(const string& init_id){
	auto it = init_datas.find(init_id);
	if (it == init_datas.end()){ return nullptr; }
	InitData& ind = (*it).second;
	if (init_id == "DropedEnergyBall"){
		return make_shared<DropedEnergyBall>();
	}
	return nullptr;
}

bool ItemCreator::CheckInitIDExistence(const string& init_id)const{
	if (init_datas.find(init_id) == init_datas.end()){ return false; }
	return true;
}

int ItemCreator::LoadInitData(){
	using IniDatP = unordered_map<string, InitData>::value_type;
	init_datas.insert(IniDatP("DropedEnergyBall", InitData("NULL")));

	return 0;
}

ItemCreator::InitData::InitData(const string& m_af)
	:action_file(m_af){};
