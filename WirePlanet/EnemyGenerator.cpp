#include "EnemyGenerator.h"
#include "DataScript.h"
#include"boost/lexical_cast.hpp"
#include"CharacterManager.h"
#include"CharacterController.h"
//#include"CreateEffectFuncs.h"
#include"Enemy.h"
#include<thread>
#include"FileLoadUtility.h"

using namespace std;

const string Enemy_GenData_File("EnemyGenData.das");
const int Enemy_GenData_Short_ParamNum(5); //敵生成データの省略パラメータ数
const int Enemy_GenData_Full_ParamNum(7); //敵生成データのフルパラメータ数

EnemyGenerator::GenData::GenData(double mt, double mr,double md,double ms, const string& miid):t(mt),r(mr),init_id(miid),dir(md),speed(ms),num(1),interval(0){}
EnemyGenerator::GenData::GenData(double mt, double mr, double md, double ms,int mn,int mi, const string& miid) : t(mt), r(mr), init_id(miid), dir(md), speed(ms), num(mn), interval(mi){}

EnemyGenerator::EnemyGenerator() : counter(0), current_gennum(-1) ,gen_state(GenerateState::Idoling)
{
	printf("Enemygeneratorのインスタンスが作成されました。\n");
}


EnemyGenerator::~EnemyGenerator()
{
	if (gen_thrd && gen_thrd->joinable()){ gen_thrd->detach(); }
	printf("Enemygeneratorのインスタンスが破棄されました。\n");
}

EnemyGenerator& EnemyGenerator::GetInstance(){
	static EnemyGenerator eg;
	return eg;
}

int EnemyGenerator::Init(){
	if (LoadGenData()){
		printf("EnemyGeneratorの初期化に失敗しました。\n");
		return -1; 
	}
	StopGenerate();
	printf("EnemyGeneratorが初期化されました。\n");
	return 0;
}

int EnemyGenerator::Reload(){
	StopGenerate();
	if (LoadGenData()){
		printf("EnemyGenerator::Reload 敵生成データの再読み込みに失敗しました。\n");
		return -1;
	}
	else{
		printf("EnemyGenerator::Reload 敵生成データを再読み込みしました。\n");
	}
	return 0;
}

int EnemyGenerator::Reset(){
	counter = 0;
	current_gennum = -1;
	if (gen_thrd && gen_thrd->joinable()){ gen_thrd->join(); }
	gen_state = GenerateState::Idoling;
	current_genqueue.clear();
	player.reset();
	return 0;
}

int EnemyGenerator::Process(){
	if (gen_state == GenerateState::Generating){
		if (Generate() == 0){
			current_gennum = -1;
			gen_state = GenerateState::Idoling;
			printf("EnemyGenerator 敵の生成が終了しました。\n");
		}
		else{
			++counter;
		}
	}
	return 0;
}

int EnemyGenerator::SetGenerateList(int gn){
	if (gen_state == GenerateState::Prepairing){ return -2; }
	auto it = gen_list.find(gn);
	if (it == gen_list.end()){ return -1; }
	SetGenQueue((*it).second);
	current_gennum = gn;
	gen_state = GenerateState::Ready;
	return 0;
}

void EnemyGenerator::StopGenerate(){
	if (gen_state == GenerateState::Prepairing){
		gen_thrd->join();
	}
	current_genqueue.clear();
	current_gennum = -1;
	counter = 0;
	gen_state = GenerateState::Idoling;
}

int EnemyGenerator::SetGenerateListOnBackGround(int gn){
	if (gen_state == GenerateState::Prepairing){ return -2; }
	auto it = gen_list.find(gn);
	if (it == gen_list.end()){ return -1; }
	const GenQueue& gq = (*it).second;
	if (gen_thrd!=nullptr && gen_thrd->joinable()){ gen_thrd->join(); }
	gen_state = GenerateState::Prepairing;
	gen_thrd = make_unique<thread>([this, &gq, gn]()->void{
		this->SetGenQueue(gq);
		this->current_gennum = gn;
		this->gen_state = GenerateState::Ready; 
	});
	return 0;
}

int EnemyGenerator::StartGenerate(){
	if (gen_state != GenerateState::Ready){ return -1; }
	gen_state = GenerateState::Generating;
	printf("EnemyGenerator 敵の生成を開始しました。\n");
	return 0;
}

void EnemyGenerator::SetGenQueue(const GenQueue& gq){
//	std::lock_guard<mutex> lock(_setgenqueue_mtx);
	printf("EnemyGenerator 生成リストのコピー及び敵のインスタンスの作成を開始します...\n");
	current_genqueue.clear();
	//このループで何らかのエラー。スレッドセーフでない関数を呼び出してるから？
	for (const auto& gd : gq){
		for (int i = 0; i < gd.second.num; ++i){
			int ii = 0;
//			printf("%d", ii++);
			shared_ptr<Enemy> e = static_pointer_cast<Enemy>(CharacterManager::GetInstance().CreateObject(ENEMY, gd.second.init_id)); //**
//			printf("%d", ii++);
			e->SetBelongingPlanet(planet);
//			printf("%d", ii++);
			e->SetTarget(player);
//			printf("%d", ii++);
			e->SetTR(gd.second.t, gd.second.r);
//			printf("%d", ii++);
			e->SetVhVr(gd.second.speed*cos(gd.second.dir / 180.0*DX_PI), gd.second.speed*sin(gd.second.dir / 180.0*DX_PI));
//			printf("%d", ii++);
//			_test_mtx.lock();
			current_genqueue.emplace(make_pair(gd.first + gd.second.interval*i, e));
//			_test_mtx.unlock();
//			printf("%d\n", ii++);
		}
	}
	printf("EnemyGenerator 敵生成の準備が完了しました!\n");
}

int EnemyGenerator::CheckGaneratingState()const{
	return gen_state == GenerateState::Idoling ? 0 : gen_state == GenerateState::Generating ? 1 : gen_state==GenerateState::Ready? 3 : 2;
}

int EnemyGenerator::GetCurrectGenNumber()const{
	return current_gennum;
}

int EnemyGenerator::GetCounterValue()const{ return counter; }

int EnemyGenerator::GetCurrentGenQueueCount()const{
//	lock_guard<recursive_mutex> lock(_test_mtx);
	return current_genqueue.size();
}

int EnemyGenerator::LoadGenData(){
	DataScript ds;
	gen_list.clear();
	if (FileLoadUtil::LoadDataScriptFromFLM(&ds,Enemy_GenData_File,true)){
		printf("\"%s\"の読み込みに失敗しました。\n",Enemy_GenData_File.c_str());
		return -1;
	}
	vector<string> names = ds.GetDataName();
	for (string& name : names){
		try{
			int gen_num = boost::lexical_cast<int>(name);
			if (gen_num < 0){
				printf("生成ナンバーが不正です。:%s\n", name.c_str());
				continue;
			}
			gen_list.insert(GenList::value_type(gen_num,GenQueue()));
			vector<vector<string>> genq;
			ds.GetDataAll(name, genq);
			for (vector<string>& gend : genq){
				using namespace boost;
				try{
					if (gend.size() == Enemy_GenData_Short_ParamNum + 1){
						int gcnt = lexical_cast<int>(gend[0]);
						if (gcnt < 0){
							printf("生成時間が不正です。::GenNum=%d(%s)\n", gcnt, name.c_str());
							continue;
						}
						GenData gd(lexical_cast<int>(gend[1]), lexical_cast<int>(gend[2]), lexical_cast<double>(gend[3]), lexical_cast<double>(gend[4]), gend[5]);
						gen_list[gen_num].insert(GenQueue::value_type(gcnt, gd));
					}
					else if (gend.size() == Enemy_GenData_Full_ParamNum + 1){
						int gcnt = lexical_cast<int>(gend[0]);
						if (gcnt < 0){
							printf("生成時間が不正です。::GenNum=%d(%s)\n", gcnt, name.c_str());
							continue;
						}
						GenData gd(lexical_cast<int>(gend[1]), lexical_cast<int>(gend[2]), lexical_cast<double>(gend[3]), lexical_cast<double>(gend[4]), lexical_cast<int>(gend[6]), lexical_cast<int>(gend[7]), gend[5]);
						gen_list[gen_num].insert(GenQueue::value_type(gcnt, gd));
					}
					else{
						printf("生成データのパラメータ数が不正です。:GenNum=%s(%d/%d)\n", name.c_str(), gend.size(), Enemy_GenData_Short_ParamNum + 1);
						continue;
					}
				}
				catch (bad_lexical_cast){
					printf("生成データのパラメータが不正です。:GenNum=%s(gcnt=%s,t=%s,r=%s,init_id=%s)\n", name.c_str(), gend[0].c_str(), gend[1].c_str(), gend[2].c_str(), gend[3].c_str());
					continue;
				}
			}

		}
		catch (boost::bad_lexical_cast){
			printf("生成ナンバーが不正です。:%s\n", name.c_str());
			continue;
		}
	}
	return 0;
}

int EnemyGenerator::Generate(){
	pair<multimap<int, shared_ptr<CharacterBase>>::iterator, multimap<int, shared_ptr<CharacterBase>>::iterator> its = current_genqueue.equal_range(counter);
	for (auto it = its.first; it != its.second; ++it){
		CharacterController::GetInstance().ResistEnemy((*it).second);
	}
	current_genqueue.erase(its.first, its.second);
	if (current_genqueue.size() > 0){
		return 1;
	}
	else{
		counter = 0;
		return 0;
	}
}

int EnemyGenerator::GetGenerateEnemyCount(int gn)const{
	auto it = gen_list.find(gn);
	if (it == gen_list.end()){ return -1; }
	int cnt = 0;
	for (auto& gd : (*it).second){
		cnt += gd.second.num;
	}
	return cnt;
}

void EnemyGenerator::SetPlanet(shared_ptr<Planet> p){ planet = p; }
void EnemyGenerator::SetPlayer(shared_ptr<CharacterBase> c){ player = c; }
