#pragma once
#include<map>
#include<memory>
//#include<mutex>

class Planet;
class CharacterBase;
namespace std{ class thread; }

//シングルトン

class EnemyGenerator
{
public:
	~EnemyGenerator();
	EnemyGenerator(const EnemyGenerator&) = delete;
	const EnemyGenerator& operator=(const EnemyGenerator&) = delete;
	static EnemyGenerator& GetInstance();
	int Init(); //初期化
	int Reset(); //リセット
	int Reload();
	int Process(); //処理
	int SetGenerateList(int); //生成列セット(生成No.)
	int	SetGenerateListOnBackGround(int); //別スレッドで生成列セット
	int StartGenerate(); //生成開始
	void StopGenerate(); //生成停止
	int CheckGaneratingState()const; //生成状態確認(0:停止中 1:生成中 2:生成準備中 3:準備完了)
	int GetCurrectGenNumber()const; //現在の生成Noを確認。なかったら-1
	int GetCounterValue()const; //カウンタの数値取得
	int GetCurrentGenQueueCount()const; //現在の生成Qにたまっている数
	int GetGenerateEnemyCount(int)const; //指定番号の敵生成数取得
	void SetPlanet(std::shared_ptr<Planet>);
	void SetPlayer(std::shared_ptr<CharacterBase>);
private:
	EnemyGenerator();
	//生成データ
	struct GenData{
		//コンストラクタ(惑星極座標t,r,発射方向,発射早さ,初期化ID)
		GenData(double, double,double,double, const std::string&);
		//コンストラクタ(惑星極座標t, r, 発射方向, 発射早さ, 発射数,発射間隔,初期化ID)
		GenData(double, double, double, double,int,int, const std::string&);
		//惑星極座標
		double t;
		double r;
		//発射情報
		double dir; //発射方向(度)
		double speed; //発射速さ
		int num; //発射数
		int interval; //発射間隔
		//初期化ID
		std::string init_id;
	};
	using GenQueue = std::multimap<int, GenData>; //生成データ列型<生成時刻、敵ポインタ>
	using GenList = std::map<int, GenQueue>; //生成データリスト型<生成No.、生成データ列>
	GenList gen_list; //生成データリスト
	int counter; //カウンター
	std::shared_ptr<Planet> planet;
	std::shared_ptr<CharacterBase> player;

	int current_gennum; //現在処理中の生成番号
	std::multimap<int, std::shared_ptr<CharacterBase>> current_genqueue; //現在未処理の生成データ列

	int LoadGenData(); //生成データロード
	int Generate(); //生成処理(1で処理中 0で処理対象がない)
	void SetGenQueue(const GenQueue&); //生成準備
//	std::mutex _setgenqueue_mtx;

//	mutable std::recursive_mutex _test_mtx;

	std::unique_ptr<std::thread> gen_thrd;
	enum class GenerateState{Generating,Idoling,Prepairing,Ready};
	GenerateState gen_state;
};

