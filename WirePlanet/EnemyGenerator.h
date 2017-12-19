#pragma once
#include<map>
#include<memory>
//#include<mutex>

class Planet;
class CharacterBase;
namespace std{ class thread; }

//�V���O���g��

class EnemyGenerator
{
public:
	~EnemyGenerator();
	EnemyGenerator(const EnemyGenerator&) = delete;
	const EnemyGenerator& operator=(const EnemyGenerator&) = delete;
	static EnemyGenerator& GetInstance();
	int Init(); //������
	int Reset(); //���Z�b�g
	int Reload();
	int Process(); //����
	int SetGenerateList(int); //������Z�b�g(����No.)
	int	SetGenerateListOnBackGround(int); //�ʃX���b�h�Ő�����Z�b�g
	int StartGenerate(); //�����J�n
	void StopGenerate(); //������~
	int CheckGaneratingState()const; //������Ԋm�F(0:��~�� 1:������ 2:���������� 3:��������)
	int GetCurrectGenNumber()const; //���݂̐���No���m�F�B�Ȃ�������-1
	int GetCounterValue()const; //�J�E���^�̐��l�擾
	int GetCurrentGenQueueCount()const; //���݂̐���Q�ɂ��܂��Ă��鐔
	int GetGenerateEnemyCount(int)const; //�w��ԍ��̓G�������擾
	void SetPlanet(std::shared_ptr<Planet>);
	void SetPlayer(std::shared_ptr<CharacterBase>);
private:
	EnemyGenerator();
	//�����f�[�^
	struct GenData{
		//�R���X�g���N�^(�f���ɍ��Wt,r,���˕���,���ˑ���,������ID)
		GenData(double, double,double,double, const std::string&);
		//�R���X�g���N�^(�f���ɍ��Wt, r, ���˕���, ���ˑ���, ���ː�,���ˊԊu,������ID)
		GenData(double, double, double, double,int,int, const std::string&);
		//�f���ɍ��W
		double t;
		double r;
		//���ˏ��
		double dir; //���˕���(�x)
		double speed; //���ˑ���
		int num; //���ː�
		int interval; //���ˊԊu
		//������ID
		std::string init_id;
	};
	using GenQueue = std::multimap<int, GenData>; //�����f�[�^��^<���������A�G�|�C���^>
	using GenList = std::map<int, GenQueue>; //�����f�[�^���X�g�^<����No.�A�����f�[�^��>
	GenList gen_list; //�����f�[�^���X�g
	int counter; //�J�E���^�[
	std::shared_ptr<Planet> planet;
	std::shared_ptr<CharacterBase> player;

	int current_gennum; //���ݏ������̐����ԍ�
	std::multimap<int, std::shared_ptr<CharacterBase>> current_genqueue; //���ݖ������̐����f�[�^��

	int LoadGenData(); //�����f�[�^���[�h
	int Generate(); //��������(1�ŏ����� 0�ŏ����Ώۂ��Ȃ�)
	void SetGenQueue(const GenQueue&); //��������
//	std::mutex _setgenqueue_mtx;

//	mutable std::recursive_mutex _test_mtx;

	std::unique_ptr<std::thread> gen_thrd;
	enum class GenerateState{Generating,Idoling,Prepairing,Ready};
	GenerateState gen_state;
};

