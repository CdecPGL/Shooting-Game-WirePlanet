#pragma once
#include "Scene.h"
#include"Vector2D.h"
#include"SecureObject.h"
#include<functional>

class Object;
class CharacterBase;
class Character;
class CharacterManager;
class EnemyGenerator;
class Planet;
class Player;
class BackGround;
class EffectManager;
class InformationArea;
class LevelChangeEffect;
class MessageWindowManager;
class EventManager;
class CharacterController;
class CameraController;
class GameScreenEffect;
class LaserManager;
class SoundManager;
class ScreenManager;
template <typename T_id, class C_obj>
class ObjectHolderTemplate_WithID;

class Game :
	public Scene
{
public:
	Game();
	~Game();
	int Init(std::shared_ptr<DataContainer>)override;
	int End()override;
	void Update()override;
	void Draw()override;

	//�f�o�b�O�֐�
	void PrintErrorMessage(const char*); //�G���[���b�Z�[�W�o��
	//�Q�[���R���g���[���֐�
	int CreateTextWindow(int); //�e�L�X�g�E�C���h�E�쐬(�e�L�X�g�ԍ�)
	int CreateTextWindowInTop(int); //�e�L�X�g�E�C���h�E����ʏ㕔�ɍ쐬(�e�L�X�g�ԍ�)
	int CreateSingleLineWindow(const char*,int,const std::string&); //�P�s�E�C���h�E�쐬(�e�L�X�g,�\���t���[������,�ʒu)
	int CreateSingleLineWindowBS(const char*, int, const std::string&); //BlackStream�̒P�s�E�C���h�E�쐬(�e�L�X�g,�\���t���[������,�ʒu)
	int CreateSingleLineWindowWS(const char*, int, const std::string&); //WhiteStream�̒P�s�E�C���h�E�쐬(�e�L�X�g,�\���t���[������,�ʒu)
	std::string GetMWGlobalVariable(const std::string&)const; //���b�Z�[�W�E�C���h�E�O���[�o���ϐ����擾����
	int GetLastSelectedIndexMW()const; //���b�Z�[�W�E�C���h�E�̑I�����ōŌ�ɑI�΂ꂽ�C���f�b�N�X
	void StartLevelChangeEffect(int); //���x���ύX�G�t�F�N�g�J�n
	int SetEnemyGenaration(int); //��������G�̃Z�b�g
	int StartEnemyGeneration(); //�G�����J�n

	//class TriggerResister{
	//public:
	//	int CreateEventSet(); //�V�K�C�x���g�Z�b�g�쐬(�C�x���g�Z�b�gID��Ԃ�)
	//	int SetEvent(int,const char*); //�C�x���g�Z�b�g�ɃC�x���g�ǉ�(�C�x���g�Z�b�gID,�C�x���g��)
	//	int SetEvent(int, const char*, int); //�C�x���g�Z�b�g�ɃC�x���g�ǉ�(�C�x���g�Z�b�gID,�C�x���g��,�C�x���g����)
	//	int CreateTrigger(const char*, int); //�w��̃C�x���g�Z�b�g�Ńg���K�[�쐬(�g���K�[��,�C�x���g�Z�b�gID)
	//private:
	//	struct EventData{
	//		std::string event_name;
	//		int parameter;
	//	};
	//	std::unordered_map<int, std::vector<EventData>> eventset;
	//};

	int CreateSingleEventTrigger(const char*,const char*); //�P��C�x���g�g���K�[�쐬
	int CreateSingleEventTrigger(const char*, const char*, int); //�P��C�x���g�g���K�[�쐬
	void SetMessageWindowMode(bool); //���b�Z�[�W�E�C���h�E���[�h�ݒ�(���̂Ƃ�����ʂȂ�)
	void ChangeBackGround(int); //�w�i�ύX(�w�i�ԍ�)
	int StartGameScreenEffect(const std::string&); //�Q�[����ʃG�t�F�N�g�J�n
	void StopGameScreenEffect(); //�Q�[����ʃG�t�F�N�g��~

	int AttachGraphFullScreen(const std::string&); //��ʂ����ς��ɉ摜���Z�b�g
	void DetachGraphFullScreen(); //��ʂ����ς��ɉ摜���Z�b�g�����̂�����
	bool IsGraphFullScreenDrawing()const; //��ʈ�h���ɉ摜��\������
	void SetPlayerOperableFlag(int); //�v���C���[����\�t���O�Z�b�g
	int CreateEnemy(const std::string&,const std::string&,const Vector2D<double>&); //�G�L�����쐬�B(�L�������A�C�ӂ�ID�A�ʒu)
	int CreatePlayer(const std::string&, const std::string&); //�v���C���[�L�����쐬�B(�L�������A�C�ӂ�ID)
	Character* GetEventCharacter(const std::string&); //�w�薼�̃L�����擾(UsingPlayer�͍ŏ���������Ă�)
	int DetachEventCharacter(const std::string&); //�C�x���g�L�����N�^�[���Ď��ΏۊO�ɂ���
	shared_ptr<Character> GetEventCharacterBySP(const std::string&); //�w�薼�̃L�����擾(UsingPlayer�͍ŏ���������Ă�)(lua�ł͎g���Ȃ�)
	int EndEventCharacter(const std::string&); //�w�薼�̃C�x���g�L�����̏I�����������s����
	int UnresistEventCharacterFromManager(const std::string&); //�w�薼�̃C�x���g�L�������}�l�[�W���[����폜����
	int ChangeCameraMode(const std::string&,bool=true); //�J�������[�h�ύX(���[�h[,��Ԃ��邩])
	int ChangeCameraTargetFromEventChara(const std::string&,bool=true); //�J�����̃^�[�Q�b�g�ύX(�C�x���g�L�����N�^����I��[,��Ԃ��邩])
	int ChangeCameraTargetFromEventObject(const std::string&, bool = true); //�J�����̃^�[�Q�b�g�ύX(�C�x���g�I�uj��Ƃ���I��[,��Ԃ��邩])
	int ResistEventCharacterToManager(const std::string&);
	int SetCameraVibration(double); //�J�����̐U�����Z�b�g
	int SetCameraViewArea(int, int, int, int); //�J�����̎���͈͂�ݒ�
	int SetupPlayer(int); //�v���C���[�̃Z�b�g�A�b�v(�Z�b�g�A�b�v�p�����[�^�B0:�ʏ�̓�l)
	enum class PauseKind{ Normal = 0, CantReturn = 1, OnlyContinue = 2}; //�|�[�Y�̎��(�ʏ�A�߂�Ȃ��A������̂�)
	int SetPauseKind(int);

	int GameOvered(); //�Q�[���I�[�o�[����
	int GameEnd(); //�Q�[���I��
	int ContinueGame(int); //�Q�[���R���e�B�j���[
	int GameCleared();

	//�X�R�A�֘A
	int StartWaveEndScoreCounting(); //�g�I���X�R�A�W�v��
	bool IsCountingWaveEndScore()const; //�g�I���X�R�A�W�v����
	int StartResultCounting(int); //���ʏW�v�J�n(0:���ނ� 1:���ނ�)
	bool IsCountingResult()const; //���ʏW�v����

	//�v���C���[�֘A
	int SavePlayerEnergy(); //�Ղ����[�̃G�l���M�[�ʂ�ۑ�

	//�C�x���g�I�u�W�F�N�g�֘A
	int CreateGraphicObject(const std::string&,const std::string&,const Vector2D<double> & = Vector2D<double>(0,0)); //�摜�I�u�W�F�N�g�쐬(�C�ӂ�ID,�摜��[,�ʒu])
	Object* GetEventObject(const std::string&); //�C�x���g�I�u�W�F�N�g�擾(�I�u�W�F�N�gID)
	int DeleteEventObject(const std::string&); //Event�I�u�W�F�N�g���폜����(�I�u�W�F�N�gID)
	void ClearEventObject(); //Event�I�u�W�F�N�g�����ׂč폜����

	//�Q�[�����擾�֐�
	int GetGenerateEnemyCount(int)const; //�w��ԍ��̓G�������擾
	//�Q�[����Ԋm�F
	bool CheckLevelChangeEffectPlaying()const; //���x���ύX�G�t�F�N�g�Đ������m�F
	bool CheckEnemyGenerating()const; //�G���������m�F
	bool CheckEnemyGeneratePrepairing()const; //�G�������������m�F
	bool CheckReadyToGenerateEnemy()const; //�G�����������ł��Ă邩�m�F
	bool CheckTalkWindowActive()const; //��b�E�C���h�E���A�N�e�B�u���m�F
	bool CheckEnemyCountZero()const; //�G�̐����[�����m�F
	bool CheckPlanetDanger()const; //�f�����댯��ԉ��m�F
	bool CheckPlanetDestroy()const; //�f�����j�󂳂ꂽ���m�F
	bool CheckUsingPlayerDead()const; //�g�p���̃v���C���[�����񂾂��m�F
	bool CheckContinued()const; //�R���e�B�j���[������

	//�������֘A�֐�(�K�v�ȏꍇ�͕K�����̊֐��̑O�ɌĂяo��)
	int SetupPlanet(); //�f���̃Z�b�g
	int SetupBackGround(); //�w�i�̃Z�b�g
	int SetupInformationArea();

	//�L�����֐�(param:����Ώ�(All�őS�ĂɓK�p),�L�������Bfalse�Ŗ�����)
	int ActivateGameComponent(const std::string&,bool);

	//�v���C���[�֘A
	int ResistPlayer(const std::string&,const std::string&); //�v���C���[��o�^(�Z�b�g����C�x���g�L�����N�^���A�A�C�R����)
	int UnresistPlayer(const std::string&); //�v���C���[�̓o�^������(�v���C���[�̃C�x���g�L�����N�^��)
	int ChangePlayer(const std::string&);

private:
	CharacterManager& cm;
	EnemyGenerator& eg;
	EffectManager& em;
	MessageWindowManager& mwm;
	EventManager& evm;
	CharacterController& cc;
	CameraController& camc;
	LaserManager& lm;
	SoundManager& sm;
	ScreenManager& scrm;
	bool _planet_active; //�f�����L����
	std::shared_ptr<Planet> planet;
	bool _bg_active; //�w�i���L����
	std::shared_ptr<BackGround> bg; //�w�i
	bool _info_area_active; //����ʂ��L����
	std::shared_ptr<InformationArea> info_area; //�����
	std::shared_ptr<LevelChangeEffect> l_chng_efct; //���x���ύX�G�t�F�N�g
	std::unique_ptr<GameScreenEffect> g_scr_efct; //�Q�[����ʃG�t�F�N�g
	bool pausing; //�|�[�Y����
	PauseKind _pause_kind;
	int _pause_cur_pos; //�|�[�Y�J�[�\���ʒu
	int current_stage; //���݂̂��ā[��No.
	enum GameStatus{ShowingStageNum,Gaming}; //ShowingStageNum:�X�e�[�W���\���� Gaming:�Q�[����
	GameStatus g_status;
	bool mess_win_mode; //���b�Z�[�W�E�C���h�E���[�h
	bool _continue_flag;
	
	Vector2D<int> _draw_area_size;

	//�v���C���[
	bool _player_operable;
	std::shared_ptr<Player> _player;

	//�C�x���g�L�����N�^�[(�C�x���g�ɂ��쐬���ꂽ�L�����N�^�[)
	std::unordered_map<std::string, std::weak_ptr<Character>> _event_character;
	int setEventCharacter(const std::string&,std::weak_ptr<Character>);
	Character* getEventCharacter(const std::string&);
	std::shared_ptr<Character> getEventCharacterBySP(const std::string&);
	bool isExixtEventCharacter(const std::string&);
	void clearEventCharacter();

	//�X�R�A�֘A
	secure_obj::SecureObject<unsigned int> _total_score;
	secure_obj::SecureObject<unsigned int> _gameover_count;
	int _score_font;
	int _score_font2;
	
	int _eve_inc_cnt;
	std::pair<std::string,std::unique_ptr<std::function<int(void)>>> _eve_func;
	int SetEveFunc(const std::string&, std::function<int(void) >);

	unsigned int _saved_player_exp; //�ۑ����ꂽ�̃v���C���[�̌o���l

	//�C�x���g�I�u�W�F�N�g
	std::unique_ptr<ObjectHolderTemplate_WithID<std::string, Object>> _event_object;

	//���͊֘A����
	void ProcessInput();
	//���b�Z�[�W�E�C���h�E����
	void ProcessMessageWindowInput();
	//�v���C���[����
	void ProcessPlayerInput();

	//�v���C���[�؂�ւ�
	void SwitchPlayer();

	//�}�l�[�W���[���Z�b�g
	int ResetManager();

//	bool _stop_game_process; //�Q�[���i�s��~�t���O

	//�f�o�b�O
	bool frame_stop; //�t���[���X�V��~
	void debug_proc(); //�f�o�b�O����
};

