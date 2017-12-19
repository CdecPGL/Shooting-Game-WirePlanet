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

	//デバッグ関数
	void PrintErrorMessage(const char*); //エラーメッセージ出力
	//ゲームコントロール関数
	int CreateTextWindow(int); //テキストウインドウ作成(テキスト番号)
	int CreateTextWindowInTop(int); //テキストウインドウを画面上部に作成(テキスト番号)
	int CreateSingleLineWindow(const char*,int,const std::string&); //単行ウインドウ作成(テキスト,表示フレーム時間,位置)
	int CreateSingleLineWindowBS(const char*, int, const std::string&); //BlackStreamの単行ウインドウ作成(テキスト,表示フレーム時間,位置)
	int CreateSingleLineWindowWS(const char*, int, const std::string&); //WhiteStreamの単行ウインドウ作成(テキスト,表示フレーム時間,位置)
	std::string GetMWGlobalVariable(const std::string&)const; //メッセージウインドウグローバル変数を取得する
	int GetLastSelectedIndexMW()const; //メッセージウインドウの選択肢で最後に選ばれたインデックス
	void StartLevelChangeEffect(int); //レベル変更エフェクト開始
	int SetEnemyGenaration(int); //生成する敵のセット
	int StartEnemyGeneration(); //敵生成開始

	//class TriggerResister{
	//public:
	//	int CreateEventSet(); //新規イベントセット作成(イベントセットIDを返す)
	//	int SetEvent(int,const char*); //イベントセットにイベント追加(イベントセットID,イベント名)
	//	int SetEvent(int, const char*, int); //イベントセットにイベント追加(イベントセットID,イベント名,イベント引数)
	//	int CreateTrigger(const char*, int); //指定のイベントセットでトリガー作成(トリガー名,イベントセットID)
	//private:
	//	struct EventData{
	//		std::string event_name;
	//		int parameter;
	//	};
	//	std::unordered_map<int, std::vector<EventData>> eventset;
	//};

	int CreateSingleEventTrigger(const char*,const char*); //単一イベントトリガー作成
	int CreateSingleEventTrigger(const char*, const char*, int); //単一イベントトリガー作成
	void SetMessageWindowMode(bool); //メッセージウインドウモード設定(今のところ効果なし)
	void ChangeBackGround(int); //背景変更(背景番号)
	int StartGameScreenEffect(const std::string&); //ゲーム画面エフェクト開始
	void StopGameScreenEffect(); //ゲーム画面エフェクト停止

	int AttachGraphFullScreen(const std::string&); //画面いっぱいに画像をセット
	void DetachGraphFullScreen(); //画面いっぱいに画像をセットしたのを解除
	bool IsGraphFullScreenDrawing()const; //画面一派うに画像を表示中か
	void SetPlayerOperableFlag(int); //プレイヤー操作可能フラグセット
	int CreateEnemy(const std::string&,const std::string&,const Vector2D<double>&); //敵キャラ作成。(キャラ名、任意のID、位置)
	int CreatePlayer(const std::string&, const std::string&); //プレイヤーキャラ作成。(キャラ名、任意のID)
	Character* GetEventCharacter(const std::string&); //指定名のキャラ取得(UsingPlayerは最初から入ってる)
	int DetachEventCharacter(const std::string&); //イベントキャラクターを監視対象外にする
	shared_ptr<Character> GetEventCharacterBySP(const std::string&); //指定名のキャラ取得(UsingPlayerは最初から入ってる)(luaでは使えない)
	int EndEventCharacter(const std::string&); //指定名のイベントキャラの終了処理を実行する
	int UnresistEventCharacterFromManager(const std::string&); //指定名のイベントキャラをマネージャーから削除する
	int ChangeCameraMode(const std::string&,bool=true); //カメラモード変更(モード[,補間するか])
	int ChangeCameraTargetFromEventChara(const std::string&,bool=true); //カメラのターゲット変更(イベントキャラクタから選択[,補間するか])
	int ChangeCameraTargetFromEventObject(const std::string&, bool = true); //カメラのターゲット変更(イベントオブj区とから選択[,補間するか])
	int ResistEventCharacterToManager(const std::string&);
	int SetCameraVibration(double); //カメラの振動をセット
	int SetCameraViewArea(int, int, int, int); //カメラの視野範囲を設定
	int SetupPlayer(int); //プレイヤーのセットアップ(セットアップパラメータ。0:通常の二人)
	enum class PauseKind{ Normal = 0, CantReturn = 1, OnlyContinue = 2}; //ポーズの種類(通常、戻れない、続けるのみ)
	int SetPauseKind(int);

	int GameOvered(); //ゲームオーバーした
	int GameEnd(); //ゲーム終了
	int ContinueGame(int); //ゲームコンティニュー
	int GameCleared();

	//スコア関連
	int StartWaveEndScoreCounting(); //波終了スコア集計を
	bool IsCountingWaveEndScore()const; //波終了スコア集計中か
	int StartResultCounting(int); //結果集計開始(0:げむお 1:げむく)
	bool IsCountingResult()const; //結果集計中か

	//プレイヤー関連
	int SavePlayerEnergy(); //ぷえりやーのエネルギー量を保存

	//イベントオブジェクト関連
	int CreateGraphicObject(const std::string&,const std::string&,const Vector2D<double> & = Vector2D<double>(0,0)); //画像オブジェクト作成(任意のID,画像名[,位置])
	Object* GetEventObject(const std::string&); //イベントオブジェクト取得(オブジェクトID)
	int DeleteEventObject(const std::string&); //Eventオブジェクトを削除する(オブジェクトID)
	void ClearEventObject(); //Eventオブジェクトをすべて削除する

	//ゲーム情報取得関数
	int GetGenerateEnemyCount(int)const; //指定番号の敵生成数取得
	//ゲーム状態確認
	bool CheckLevelChangeEffectPlaying()const; //レベル変更エフェクト再生中か確認
	bool CheckEnemyGenerating()const; //敵生成中か確認
	bool CheckEnemyGeneratePrepairing()const; //敵生成準備中か確認
	bool CheckReadyToGenerateEnemy()const; //敵生成準備ができてるか確認
	bool CheckTalkWindowActive()const; //会話ウインドウがアクティブ化確認
	bool CheckEnemyCountZero()const; //敵の数がゼロか確認
	bool CheckPlanetDanger()const; //惑星が危険常態化確認
	bool CheckPlanetDestroy()const; //惑星が破壊されたか確認
	bool CheckUsingPlayerDead()const; //使用中のプレイヤーが死んだか確認
	bool CheckContinued()const; //コンティニューしたか

	//初期化関連関数(必要な場合は必ず他の関数の前に呼び出す)
	int SetupPlanet(); //惑星のセット
	int SetupBackGround(); //背景のセット
	int SetupInformationArea();

	//有効化関数(param:操作対象(Allで全てに適用),有効化か。falseで無効化)
	int ActivateGameComponent(const std::string&,bool);

	//プレイヤー関連
	int ResistPlayer(const std::string&,const std::string&); //プレイヤーを登録(セットするイベントキャラクタ名、アイコン名)
	int UnresistPlayer(const std::string&); //プレイヤーの登録を解除(プレイヤーのイベントキャラクタ名)
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
	bool _planet_active; //惑星が有効化
	std::shared_ptr<Planet> planet;
	bool _bg_active; //背景が有効化
	std::shared_ptr<BackGround> bg; //背景
	bool _info_area_active; //情報画面が有効化
	std::shared_ptr<InformationArea> info_area; //情報画面
	std::shared_ptr<LevelChangeEffect> l_chng_efct; //レベル変更エフェクト
	std::unique_ptr<GameScreenEffect> g_scr_efct; //ゲーム画面エフェクト
	bool pausing; //ポーズ中華
	PauseKind _pause_kind;
	int _pause_cur_pos; //ポーズカーソル位置
	int current_stage; //現在のすてーぎNo.
	enum GameStatus{ShowingStageNum,Gaming}; //ShowingStageNum:ステージ名表示中 Gaming:ゲーム中
	GameStatus g_status;
	bool mess_win_mode; //メッセージウインドウモード
	bool _continue_flag;
	
	Vector2D<int> _draw_area_size;

	//プレイヤー
	bool _player_operable;
	std::shared_ptr<Player> _player;

	//イベントキャラクター(イベントにより作成されたキャラクター)
	std::unordered_map<std::string, std::weak_ptr<Character>> _event_character;
	int setEventCharacter(const std::string&,std::weak_ptr<Character>);
	Character* getEventCharacter(const std::string&);
	std::shared_ptr<Character> getEventCharacterBySP(const std::string&);
	bool isExixtEventCharacter(const std::string&);
	void clearEventCharacter();

	//スコア関連
	secure_obj::SecureObject<unsigned int> _total_score;
	secure_obj::SecureObject<unsigned int> _gameover_count;
	int _score_font;
	int _score_font2;
	
	int _eve_inc_cnt;
	std::pair<std::string,std::unique_ptr<std::function<int(void)>>> _eve_func;
	int SetEveFunc(const std::string&, std::function<int(void) >);

	unsigned int _saved_player_exp; //保存されたのプレイヤーの経験値

	//イベントオブジェクト
	std::unique_ptr<ObjectHolderTemplate_WithID<std::string, Object>> _event_object;

	//入力関連処理
	void ProcessInput();
	//メッセージウインドウ入力
	void ProcessMessageWindowInput();
	//プレイヤー入力
	void ProcessPlayerInput();

	//プレイヤー切り替え
	void SwitchPlayer();

	//マネージャーリセット
	int ResetManager();

//	bool _stop_game_process; //ゲーム進行停止フラグ

	//デバッグ
	bool frame_stop; //フレーム更新停止
	void debug_proc(); //デバッグ処理
};

