#include "Game.h"
#include"CharacterController.h"
#include"DefaultGameObjects.h"
#include"EnemyGenerator.h"
#include"EffectManager.h"
#include"InitFuncs.h"
#include"SceneManager.h"
#include"MassageWindowManager.h"
#include"InformationArea.h"
#include"LevelChangeEffect.h"
#include"EventManager.h"
#include"CameraController.h"
#include"GameScreenEffect.h"
#include"GraphicController2D.h"
#include"LaserManager.h"
#include"ScreenManager.h"
#include"player.h"
#include"ObjectHolderTemplate_WithID.h"
#include"CreateObjectFuncs.h"
#include"GameInitializer.h"
#include<algorithm>
#include"GPIUtil.h"
#include"GameCounter.h"
#include"SystemVariable.h"

/*�v���C���[�̊Ǘ��ɂ���
�v���C���[�̊Ǘ��̓C�x���g�L�����N�^�[�Ɉˑ����Ă��邽�߁A�v���C���[�Ƃ��ēo�^���ꂽ����C�x�L�������C�x�L��������폜����ƁA�������ɖ�肪�N���邩��
*/

using namespace std;

namespace{
	const double Planet_Danger_Radius(100); //�f�����댯��ԂɂȂ锼�a
	const double Planet_Destroy_Radius(50); //�f�����j�󂳂�锼�a(WirePlanet.cpp�ƍ��킹��)
	const int PAUSE_MENU_ITEM_NUM(3);
}

Game::Game() :cm(CharacterManager::GetInstance()), eg(EnemyGenerator::GetInstance()), em(EffectManager::GetInstance()), mwm(MessageWindowManager::GetInstance()), evm(EventManager::GetInstance()), cc(CharacterController::GetInstance()), camc(CameraController::GetInstance()), lm(LaserManager::GetInstance()), sm(SoundManager::GetInstance()), scrm(ScreenManager::GetInstance())
, g_scr_efct(make_unique<GameScreenEffect>()), _player_operable(true), _info_area_active(false), _bg_active(false)
, pausing(false), g_status(ShowingStageNum), frame_stop(false), mess_win_mode(false), _event_object(make_unique<ObjectHolderTemplate_WithID<string,Object>>())
,_total_score(0u),_gameover_count(0u),_eve_inc_cnt(0),_eve_func(make_pair("NULL",make_unique<function<int(void)>>()))
, _score_font(CreateFontToHandle(SysVar::GAME_FONT_NAME, 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE)), _score_font2(CreateFontToHandle("���C���I", 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE)), _draw_area_size(gc2d::GetDrawAreaSize())/*,_stop_game_process(false)*/, _pause_cur_pos(0),_pause_kind(PauseKind::Normal)
,_saved_player_exp(0),_continue_flag(0){
}


Game::~Game()
{
	DeleteFontToHandle(_score_font);
	DeleteFontToHandle(_score_font2);
}

int Game::Init(shared_ptr<DataContainer> dc){
	Vector2D<int> draw_area_size = gc2d::GetDrawAreaSize();
	//�}�l�[�W���[������
	if (ResetManager()){
		printf("Game::Init:�������Ɏ��s���܂����B�}�l�[�W���[�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -3;
	}
	//�f��
	SetupPlanet();
	ActivateGameComponent("Planet", true);

	//���G���A
	SetupInformationArea();
	ActivateGameComponent("InformationArea", true);

	//�v���C���[������
	_player = static_pointer_cast<Player>(CharacterManager::GetInstance().CreateAndResistObject(CharaAtrribtute::PLAYER, "Player"));
	_player->SetBelongingPlanet(planet);
	bool cont_flag;
	dc->GetData("ContinueFlag", cont_flag);
	if (cont_flag){
		unsigned int player_exp;
		unsigned int ts;
		dc->GetData("PlayerEXP", player_exp);
		dc->GetData("Score", ts);
		_player->AddExperiencePoint(player_exp);
		_saved_player_exp = player_exp;
		_total_score.set(ts / 2); //�R���e�B�j���[����2���̈��
		_continue_flag = cont_flag;
	}
	ResistPlayer("Player", "NULL");
	cc.ChangePlayer("Player");
	info_area->ChangePlayer("Player");
	//���x���ύX�G�t�F�N�g
	l_chng_efct = make_shared<LevelChangeEffect>();
	l_chng_efct->SetPosition(Vector2D<int>(0,240));
	l_chng_efct->SetSize(Vector2D<int>(draw_area_size.x,60));
	l_chng_efct->SetFontData(SysVar::GAME_FONT_NAME, 3, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	l_chng_efct->Init();
	//�Q�[����ʃG�t�F�N�g������
	g_scr_efct->Init();

	//�C�x���g�}�l�[�W��
	evm.SetGameScenePointer(this);

	//�V�[���̏�����
	string initializer_id;
	string start_event;
	int start_event_param=0;
	dc->GetData("GameInitializerID", initializer_id);
	dc->GetData("GameStartEvent", start_event);
	dc->GetData("GameStartEventParam", start_event_param);
	if (GameInitializer::GetInstance()(this, initializer_id, start_event, start_event_param) < 0){
		return -1;
	}

	//�X�R�A�W�v���Z�b�g
	GameCounter::LevelCounter::ResetLevelCount();

	GetNoActiveState(true);

	Scene::Init(dc);
	return 0;
}

int Game::End(){
	eg.StopGenerate();
	Scene::End();
	evm.AllRecycle();
	SoundManager::GetInstance().StopMusic();
	InitFuncs::ClearObjectInManager();
	planet.reset();
	bg.reset();
	info_area.reset();
	l_chng_efct.reset();
	g_scr_efct.reset();
	pausing = false;
	_player_operable = true;
	_event_character.clear();
	_event_object->clear();
	frame_stop = false;
//	_stop_game_process = false;
	_saved_player_exp = 0;
	_continue_flag = false;

	_eve_inc_cnt = 0;
	_eve_func.first = "NULL";
	_eve_func.second.reset();

	_info_area_active = false;
	_bg_active = false;
	_planet_active = false;

	_pause_kind = PauseKind::Normal;

	/*DeleteFontToHandle(_score_font);
	DeleteFontToHandle(_score_font2);*/
	ResetManager();
	return 0;
}

void Game::Update(){
	if (SysVar::DEBUG_LEVEL >= 3){
		debug_proc();
	}
	if (!frame_stop || key.GetKeyPush(KEY_INPUT_P) != 0){
		if (key.GetCustomInputPush(CUSTOM_INPUT_START)){
			sm.PlaySoundByID(pausing ? "Cancel" : "Define");
			pausing = !pausing;
			_pause_cur_pos = 0;
		}
		if (!pausing){
			ProcessInput();

			if(bg!=nullptr && _bg_active)bg->Update();
			if(planet!=nullptr && _planet_active)planet->Update();
			/*if (IsCountingResult() == false){ cm.Update(); }*/
			cm.Update();
			lm.Update();
			em.Update();
			cm.CollisionDetect();
			_event_object->do_all(&Object::Update);
			mwm.Update();
			l_chng_efct->Update();
			if(info_area!=nullptr && _info_area_active)info_area->Update();
			//	planet->DebugDraw();
			evm.Update();

			g_scr_efct->Update();
			camc.Update();

			//�X�R�A�X�V
			info_area->SetPoint(_total_score.get() + GameCounter::LevelCounter::GetBaseScore());

			eg.Process();
			em.Process();
			cm.Process();
			cc.Process();
			lm.Process();
			mwm.Process();
			evm.Process();

			//�L�[�{�[�h1�ŃJ�����ύX
			if (key.GetKeyPush(KEY_INPUT_1)){
				switch (camc.GetCameraMode()){
				case CameraController::CameraMode::Normal:
					camc.ChangeCameraMode(CameraController::CameraMode::Wide);
					break;
				case CameraController::CameraMode::Wide:
					camc.ChangeCameraMode(CameraController::CameraMode::Zoom);
					break;
				case CameraController::CameraMode::Zoom:
					camc.ChangeCameraMode(CameraController::CameraMode::FocusPlanet);
					break;
				case CameraController::CameraMode::FocusPlanet:
					camc.ChangeCameraMode(CameraController::CameraMode::Normal);
					break;
				default:
					break;
				}
			}
			//�E�C���h�E����A�N�e�B�u�ɂȂ�����|�[�Y����B
			if (GetNoActiveState(true))pausing = true;
		}
		else{ //�|�[�Y
			if (key.GetCustomInputPush(CUSTOM_INPUT_DOWN)){
				++_pause_cur_pos;
				if (_pause_cur_pos >= PAUSE_MENU_ITEM_NUM){ _pause_cur_pos = 0; }
				sm.PlaySoundByID("Select");
			}
			else if (key.GetCustomInputPush(CUSTOM_INPUT_UP)){
				--_pause_cur_pos;
				if (_pause_cur_pos < 0){ _pause_cur_pos = PAUSE_MENU_ITEM_NUM - 1; }
				sm.PlaySoundByID("Select");
			}
			if (key.GetCustomInputPush(CUSTOM_INPUT_A)){
				switch (_pause_kind==PauseKind::OnlyContinue ? 0 : _pause_cur_pos){
				case 0: //continue
					pausing = false;
					sm.PlaySoundByID("Define");
					break;
				case 1: //restart
					if (_pause_kind == PauseKind::CantReturn){
						sm.PlaySoundByID("Cancel");
						break;
					}
					GameCounter::LevelCounter::ApplyLevelCountToGPI();
					dc->SetData("GameInitializerID", string("NormalStageInitializer"));
					dc->SetData("GameStartEvent", string("StartEvent"));
					dc->SetData("GameStartEventParam", 1);
					dc->SetData("ContinueFlag", false);
					sm.PlaySoundByID("Define");
					SceneManager::GetInstance().CreateAndChangeScene("Game");
					break;
				case 2: //quit
					if (_pause_kind == PauseKind::CantReturn){
						sm.PlaySoundByID("Cancel");
						break;
					}
					GameCounter::LevelCounter::ApplyLevelCountToGPI();
					sm.PlaySoundByID("Define");
					SceneManager::GetInstance().CreateAndChangeScene("Title");
					break;
				default:
					break;
				}
			}
		}
	}
}
void Game::Draw(){
	Vector2D<int> draw_size_area = gc2d::GetDrawAreaSize();
	ScreenManager::GetInstance().PushScreen();
	if(bg!=nullptr && _bg_active)bg->Draw();

	if(planet!=nullptr && _planet_active)planet->Draw();
	/*Vector2D<double> ppp = planet->GetPositionXY();
	gc2d::DrawCircle(ppp.x, ppp.y, Planet_Danger_Radius, GetColor(255, 100, 100), false);
	gc2d::DrawCircle(ppp.x, ppp.y, Planet_Destroy_Radius, GetColor(0, 0, 0), false);*/

	cm.Draw();
	lm.Draw();
	_event_object->do_all(&Object::Draw);
	em.Draw();

	scrm.ApplyEffectToTopScreen(ScreenManager::ScreenEffect::Broom, 1025);
	g_scr_efct->Draw();
//	scrm.PushScreen();
	mwm.Draw();

	if (_eve_func.second){
		if (*_eve_func.second){
			if ((*_eve_func.second)()){
				_eve_func.first = "NULL";
				_eve_func.second.reset();
			}
		}
	}

	l_chng_efct->Draw();
	if(info_area!=nullptr && _info_area_active)info_area->Draw();
	if (pausing){
		const int font_size(32);
		//������@
		const int help_top_pos_y(30);
		const unsigned int hs_num = 3;
		char* help_str[hs_num] = { "[Move:LEFTorRIGHT-Key] [Jump:C-Key]", "[Shot:A-Key] [HorizontalShot:X-Key]", "[Barrier:D-Key] [MakeClone:Z-Key]" };
		//���j���[
		const int menu_top_pos_y(210);
		char* menu_strs[PAUSE_MENU_ITEM_NUM] = { "Error!", "Error!", "Error!" };
		int color[PAUSE_MENU_ITEM_NUM] = { GetColor(255, 0, 0), GetColor(255, 0, 0), GetColor(255, 0, 0) };
		switch (_pause_kind){
		case PauseKind::Normal:
			color[0] = GetColor(255, 255, 255);
			color[1] = GetColor(255, 255, 255);
			color[2] = GetColor(255, 255, 255);
			menu_strs[0] = "Continue";
			menu_strs[1] = "Restart";
			menu_strs[2] = "Quit";
			break;
		case PauseKind::OnlyContinue:
			color[0] = GetColor(255, 255, 255);
			color[1] = GetColor(255, 255, 255);
			color[2] = GetColor(255, 255, 255);
			menu_strs[0] = "Continue";
			menu_strs[1] = "Continue!";
			menu_strs[2] = "Continue!!";
			break;
		case PauseKind::CantReturn:
			color[0] = GetColor(255, 255, 255);
			color[1] = GetColor(100, 100, 100);
			color[2] = GetColor(100, 100, 100);
			menu_strs[0] = "Continue";
			menu_strs[1] = "Restart";
			menu_strs[2] = "Quit";
			break;
		default:
			break;
		}
		int sw=0;
		//�|�[�Y�̈�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawFillBox(0, 0, draw_size_area.x, draw_size_area.y, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		//�w���v
		sw = GetDrawFormatStringWidthToHandle(_score_font, "HELP");
		DrawFormatStringToHandle((draw_size_area.x - sw) / 2, help_top_pos_y + font_size * 0, GetColor(180, 180, 180), _score_font, "HELP");
		for (unsigned int i = 0; i < hs_num; ++i){
			sw = GetDrawFormatStringWidthToHandle(_score_font, help_str[i]);
			DrawFormatStringToHandle((draw_size_area.x - sw) / 2, help_top_pos_y + font_size * (i+2), GetColor(180, 180, 180), _score_font, help_str[i]);
		}
		//�|�[�Y
		sw = GetDrawStringWidthToHandle("PAUSE", 5, _score_font);
		DrawFormatStringToHandle((draw_size_area.x - sw) / 2, menu_top_pos_y+font_size*0, GetColor(255, 255, 255), _score_font, "PAUSE");
		//���j���[����
		for (int i = 0; i < PAUSE_MENU_ITEM_NUM; ++i){
			sw = GetDrawFormatStringWidthToHandle(_score_font, menu_strs[i]);
			DrawFormatStringToHandle((draw_size_area.x - sw) / 2, menu_top_pos_y + font_size*(0+2+i), color[i], _score_font, menu_strs[i]);
		}
		//�J�[�\��
		{
			sw = GetDrawFormatStringWidthToHandle(_score_font, menu_strs[_pause_cur_pos]);
			int bh = (draw_size_area.x - sw) / 2;
			int bt = menu_top_pos_y + font_size*(_pause_cur_pos + 2);
			DrawLineBox(bh, bt, bh + GetDrawFormatStringWidthToHandle(_score_font, menu_strs[_pause_cur_pos]), bt + font_size, GetColor(255, 255, 255));
		}
//		DrawFormatStringToHandle((draw_size_area.x - sw) / 2 - font_size, top_pos_y+font_size*(_pause_cur_pos+2), GetColor(255, 255, 255), _score_font, "");
	}
}

int Game::ResetManager(){
	if (cm.Reset()){
		printf("game::ResetManager:CharacterManager�̃��Z�b�g�Ɏ��s���܂����B\n");
		return - 1;
	}
	if (eg.Reset()){
		printf("game::ResetManager:EnemyGenerator�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -2;
	}
	if (em.Reset()){
		printf("game::ResetManager:EffectManager�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -3;
	}
	if (mwm.Reset()){
		printf("game::ResetManager:MessageWindowManager�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -4;
	}
	if (evm.Reset()){
		printf("game::ResetManager:EventManager�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -5;
	}
	if (cc.Reset()){
		printf("game::ResetManager:CharacterController�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -6;
	}
	if (camc.Reset()){
		printf("game::ResetManager:CameraController�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -7;
	}
	if (lm.Reset()){
		printf("game::ResetManager:LaserManager�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -8;
	}
	if (sm.Reset()){
		printf("game::ResetManager:SoundManager�̃��Z�b�g�Ɏ��s���܂����B\n");
		return -9;
	}
	return 0;
}

void Game::debug_proc(){
	//�f�o�b�O�p����
	if (key.GetKeyPush(KEY_INPUT_Y)){ frame_stop = !frame_stop; }
	if (key.GetKeyPush(KEY_INPUT_G)){ if (planet)planet->AddForce(0, -10000); }
	if (key.GetKeyPush(KEY_INPUT_T)){ if (planet)planet->AddForce(0, 10000); }
	if (key.GetKeyPush(KEY_INPUT_R)){ if (planet) planet->Reset(); }
	if (key.GetKeyPush(KEY_INPUT_F)){ if (planet)planet->RandomTransform(10); }
	if (key.GetKeyPush(KEY_INPUT_V)){ if (planet)planet->RandomTransform(100); }
	if (key.GetKeyStatus(KEY_INPUT_B)){ _player->AddExperiencePoint(1000); }
	if (key.GetKeyPush(KEY_INPUT_2)){
		shared_ptr<Laser> ls = lm.CreateLaser("test");
		ls->SetBelongingPlanet(planet);
		//				ls->SetWVr(10.0, 0);
		ls->SetDirection(270);
		ls->SetColor(0, 0, 255);
		ls->SetTR(_player->GetT(), _player->GetR());
		ls->SetObjectBelongingTo(ObjectBelongingTo::Friend);
	}
}

//�C�x���g�L�����֘A
int Game::setEventCharacter(const string& name,weak_ptr<Character> chara){
	if (isExixtEventCharacter(name) == true){
		return -1;
	}
	_event_character.insert(make_pair(name, chara));
	return 0;
}

Character* Game::getEventCharacter(const string& name){
	if (isExixtEventCharacter(name)==false){
		return nullptr;
	}
	return (*_event_character.find(name)).second.lock().get();
}
shared_ptr<Character> Game::getEventCharacterBySP(const string& name){
	if (isExixtEventCharacter(name) == false){
		return nullptr;
	}
	return (*_event_character.find(name)).second.lock();
}

bool Game::isExixtEventCharacter(const string& name){
	auto it = _event_character.find(name);
	if (it == _event_character.end()){
		return false;
	}
	if ((*it).second.expired()){
		_event_character.erase(it);
		return false;
	}
	return true;
}

void Game::clearEventCharacter(){
	_event_character.clear();
}


//�v���C���[�؂�ւ�(�v���C���[��̎��̃v���C���[�ɕύX���āA�J�����ύX�A�ʒu�������s��)
void Game::SwitchPlayer(){
	//string old_plr = _players.front();
	//_players.pop_front();
	//_players.push_back(old_plr);
	//auto ptr = getEventCharacterBySP(_players.front());
	////_players�ɓo�^����Ă���ID�������L�����N�^��Player�^�łȂ���΂Ȃ�Ȃ��̂�static�L���X�g�ł���
	//usg_player = make_pair(_players.front(), static_pointer_cast<Player>(ptr));
	//cc.ChangePlayer(_players.front());
	//if (info_area){ info_area->ChangePlayer(_players.front()); }
	//eg.SetPlayer(ptr);
	//camc.SetTarget(ptr);
	//ptr->SetMode(Character::Mode::StandAlone);
	//auto old_ptr = getEventCharacterBySP(old_plr);
	//ptr->CopyStatus(*old_ptr);
}

//���͊֘A����
void Game::ProcessInput(){
	ProcessMessageWindowInput();
	if (_player_operable){ ProcessPlayerInput(); }
}
//���b�Z�[�W�E�C���h�E����
void Game::ProcessMessageWindowInput(){
	if (key.GetCustomInputPush(CUSTOM_INPUT_A)){ mwm.ButtonPushed(MessageWindowManager::Select); }
	if (key.GetCustomInputPush(CUSTOM_INPUT_UP)){ mwm.ButtonPushed(MessageWindowManager::Up); }
	if (key.GetCustomInputPush(CUSTOM_INPUT_DOWN)){ mwm.ButtonPushed(MessageWindowManager::Down); }
}
//�v���C���[����
void Game::ProcessPlayerInput(){
	unsigned int input(0);
	if (key.GetCustomInputState(CUSTOM_INPUT_UP)){ input |= PlayerInput::UP; }
	if (key.GetCustomInputState(CUSTOM_INPUT_DOWN)){ input |= PlayerInput::DOWN; }
	if (key.GetCustomInputState(CUSTOM_INPUT_LEFT)){ input |= PlayerInput::LEFT; }
	if (key.GetCustomInputState(CUSTOM_INPUT_RIGHT)){ input |= PlayerInput::RIGHT; }
	if (key.GetCustomInputState(CUSTOM_INPUT_A)){ input |= PlayerInput::A; }
	if (key.GetCustomInputState(CUSTOM_INPUT_B)){ input |= PlayerInput::B; }
	if (key.GetCustomInputState(CUSTOM_INPUT_X)){ input |= PlayerInput::X; }
	if (key.GetCustomInputState(CUSTOM_INPUT_Y)){ input |= PlayerInput::Y; }
	if (key.GetCustomInputState(CUSTOM_INPUT_L)){ input |= PlayerInput::L; }
	if (key.GetCustomInputState(CUSTOM_INPUT_R)){ input |= PlayerInput::R; }
	_player->SetInput(input);
}

int Game::SetEveFunc(const string& fn, function<int(void)> f){
	_eve_inc_cnt = 0;
	_eve_func.first = fn;
	_eve_func.second = make_unique<function<int(void)>>(f);
	return 0;
}

//�f�o�b�O�֐�
//�G���[�o��
void Game::PrintErrorMessage(const char* str){
	printfDx(str);
	printf(str);
}

//�Q�[���R���g���[���֐�
//�e�L�X�g�E�C���h�E�쐬
int Game::CreateTextWindow(int mess_id){
	return mwm.CreateTalkWindow(mess_id,MessageWindowManager::TalkWinPos::Bottom);
}
int Game::CreateTextWindowInTop(int mess_id){
	return mwm.CreateTalkWindow(mess_id, MessageWindowManager::TalkWinPos::Top);
}
//�P�s�E�C���h�E�쐬
int Game::CreateSingleLineWindow(const char* text,int df,const string& s_pos){
	MessageWindowManager::SingleLineWinPos slw_pos;
	if (s_pos == "Top"){ slw_pos = MessageWindowManager::SingleLineWinPos::Top; }
	else if (s_pos == "Bottom"){ slw_pos = MessageWindowManager::SingleLineWinPos::Bottom; }
	else if (s_pos == "Center"){ slw_pos = MessageWindowManager::SingleLineWinPos::Center; }
	return mwm.CreateRequestSingleLineWindowByString(text,df,slw_pos,MessageWindowManager::SingleLineWinKind::Normal);
}
int Game::CreateSingleLineWindowBS(const char* text, int df, const string& s_pos){
	MessageWindowManager::SingleLineWinPos slw_pos;
	if (s_pos == "Top"){ slw_pos = MessageWindowManager::SingleLineWinPos::Top; }
	else if (s_pos == "Bottom"){ slw_pos = MessageWindowManager::SingleLineWinPos::Bottom; }
	else if (s_pos == "Center"){ slw_pos = MessageWindowManager::SingleLineWinPos::Center; }
	return mwm.CreateRequestSingleLineWindowByString(text, df, slw_pos, MessageWindowManager::SingleLineWinKind::BlackStream);
}
int Game::CreateSingleLineWindowWS(const char* text, int df, const string& s_pos){
	MessageWindowManager::SingleLineWinPos slw_pos;
	if (s_pos == "Top"){ slw_pos = MessageWindowManager::SingleLineWinPos::Top; }
	else if (s_pos == "Bottom"){ slw_pos = MessageWindowManager::SingleLineWinPos::Bottom; }
	else if (s_pos == "Center"){ slw_pos = MessageWindowManager::SingleLineWinPos::Center; }
	return mwm.CreateRequestSingleLineWindowByString(text, df, slw_pos, MessageWindowManager::SingleLineWinKind::WhiteStream);
}
//���b�Z�[�W�E�C���h�E�O���[�o���ϐ��擾
string Game::GetMWGlobalVariable(const string& v_name)const{
	return mwm.GetMWValue(v_name, -1);
}
//MW�ł��E��ɑI�΂ꂽ�C���f�b�N�X
int Game::GetLastSelectedIndexMW()const{
	return mwm.GetLastSelectedIndex();
}
//���x���ύX�G�t�F�N�g�J�n
void Game::StartLevelChangeEffect(int l){
	l_chng_efct->SetStageNumber(l);
	l_chng_efct->StartEffect();
}
//�G�����Z�b�g
int Game::SetEnemyGenaration(int no){
	return eg.SetGenerateList(no); //�b��I�ɃV���O���X���b�h�ł��g�p
//	return eg.SetGenerateListOnBackGround(no);
}
//�G�����J�n
int Game::StartEnemyGeneration(){
	return eg.StartGenerate();
}
//�P��C�x���g�g���K�[�쐬
int Game::CreateSingleEventTrigger(const char* tn, const char* en){
	shared_ptr<Trigger> trg = evm.CreateTrigger(tn);
	if (trg == nullptr){ return -1; }
	trg->ResistEvent(en);
	return 0;
}
int Game::CreateSingleEventTrigger(const char* tn, const char* en,int p){
	shared_ptr<Trigger> trg = evm.CreateTrigger(tn);
	if (trg == nullptr){ return -1; }
	trg->ResistEvent(en,p);
	return 0;
}
//���b�Z�[�W�E�C���h�E���[�h�ݒ�
void Game::SetMessageWindowMode(bool mwmf){ mess_win_mode = mwmf; }
//�w�i�ύX
void Game::ChangeBackGround(int bn){
	if(bg)bg->ChangeBackGround(bn);
}
//�Q�[����ʃG�t�F�N�g�J�n
int Game::StartGameScreenEffect(const std::string& eid){
	return g_scr_efct->StartEffect(eid);
}
//�Q�[����ʃG�t�F�N�g��~
void Game::StopGameScreenEffect(){
	g_scr_efct->StopEffect();
}

//�摜����ʂ����ς��ɐ���
int Game::AttachGraphFullScreen(const string& name){
	return g_scr_efct->StartEffect("FullScreenGraph", name);
}
//����ʉ摜�Z�b�g����
void Game::DetachGraphFullScreen(){
	if (IsGraphFullScreenDrawing()){
		g_scr_efct->StopEffect();
	}
}
//�S��ʉ摜�\������
bool Game::IsGraphFullScreenDrawing()const{
	return g_scr_efct->GetCurrentEffectName() == "FullScreenGraph" ? true : false;
}

//Event�I�u�W�F�N�g�֘A
//�摜�I�u�W�F�N�g�쐬
int Game::CreateGraphicObject(const string& oid,const string& gn, const Vector2D<double>& pos){
	std::shared_ptr<Object> obj = CreateObjFuncs::CreateGraphicObject(gn);
	if (obj == nullptr){ return -1; }
	obj->SetXY(pos);
	if (_event_object->resist_object(obj, oid) < 0){
		return -1;
	}
	_event_object->process_resist_list();
	return 0;
}
//Event�I�u�W�F�N�g�擾
Object* Game::GetEventObject(const string& oid){
	return _event_object->find_object(oid).get();
}
//Event�I�u�W�F�N�g�폜
int Game::DeleteEventObject(const string& oid){
	if (_event_object->unresist_object(oid) < 0){ return -1; }
	_event_object->process_unresist_list();
	return 0;
}
//Event�I�u�W�F�N�g�S����
void Game::ClearEventObject(){
	_event_object->clear();
}

void Game::SetPlayerOperableFlag(int pof){ _player_operable = pof ? true : false; }

int Game::CreateEnemy(const string& char_name,const string& id,const Vector2D<double>& pos){
	shared_ptr<Character> chr = static_pointer_cast<Character>(cc.CreateAndResistEnemy(CharaAtrribtute::ENEMY, char_name));
	if (setEventCharacter(id, chr) < 0){
		return -1;
	}
	chr->SetR(pos.y);
	chr->SetT(pos.x);
	chr->SetMode(Character::Mode::Control);
	return 0;
}

int Game::CreatePlayer(const string& char_name, const string& id){
	shared_ptr<Character> chr = static_pointer_cast<Character>(cc.CreateAndResistEnemy(CharaAtrribtute::PLAYER, char_name));
	if (setEventCharacter(id, chr) < 0){
		return -1;
	}
//	chr->SetR(pos.y);
//	chr->SetT(pos.y);
	chr->SetMode(Character::Mode::Control);
	return 0;
}

Character* Game::GetEventCharacter(const std::string& name){
	return GetEventCharacterBySP(name).get();
}
shared_ptr<Character> Game::GetEventCharacterBySP(const std::string& name){
	if (name == "Player"){
		return _player;
	}
	else{
		return getEventCharacterBySP(name);
	}
	return nullptr;
}

int Game::DetachEventCharacter(const string& name){
	auto it = _event_character.find(name);
	if (it == _event_character.end()){ return -1; }
	_event_character.erase(it);
	return 0;
}

int Game::EndEventCharacter(const string& name){
	auto p = getEventCharacter(name);
	if (p == nullptr){ return -1; }
	p->End();
	return 0;
}

int Game::UnresistEventCharacterFromManager(const string& name){
	auto p = GetEventCharacter(name);
	if (p == nullptr){ return -1; }
	cm.DeleteObject(p->GetCharID());
	return 0;
}

int Game::ChangeCameraMode(const string& m,bool intp){
	if (m == "Normal"){
		camc.ChangeCameraMode(CameraController::CameraMode::Normal,intp);
	}
	else if (m == "Wide"){
		camc.ChangeCameraMode(CameraController::CameraMode::Wide, intp);
	}
	else if (m == "Zoom"){
		camc.ChangeCameraMode(CameraController::CameraMode::Zoom, intp);
	}
	else if (m == "Free"){
		camc.ChangeCameraMode(CameraController::CameraMode::Free, intp);
	}
	else{
		return -1;
	}
	return 0;
}

int Game::ChangeCameraTargetFromEventChara(const string& name,bool intp){
	auto ptr = GetEventCharacterBySP(name);
	if (ptr == nullptr){ return -1; }
	camc.SetTarget(ptr, intp);
	return 0;
}

int Game::ChangeCameraTargetFromEventObject(const string& name, bool intp){
	auto ptr = _event_object->find_object(name);
	if (ptr == nullptr){ return -1; }
	camc.SetTarget(ptr, intp);
	return 0;
}

int Game::ResistEventCharacterToManager(const string& name){
	auto ptr = GetEventCharacterBySP(name);
	if (ptr == nullptr){ return -1; }
	cm.ResistObject(ptr);
	return 0;
}

int Game::SetCameraVibration(double p){
	camc.SetVibrationPower(p);
	return 0;
}

int Game::SetCameraViewArea(int x, int y, int w, int h){
	camc.SetCameraViewArea(RectAngle<double>(x, y, w, h));
	return 0;
}

//�Q�[�����擾�֐�
//�w��ԍ��̓G�������擾
int Game::GetGenerateEnemyCount(int gn)const{
	return eg.GetGenerateEnemyCount(gn);
}

//�Q�[����Ԋm�F�֐�
//���x���ύX�G�t�F�N�g�Đ����؊m�F
bool Game::CheckLevelChangeEffectPlaying()const{
	return l_chng_efct->Playing();
}
//�G���������m�F
bool Game::CheckEnemyGenerating()const{
	return eg.CheckGaneratingState() != 0;
}

//�K�������������m�F
bool Game::CheckEnemyGeneratePrepairing()const{
	return eg.CheckGaneratingState() == 2;
}

//�G�������������ς݂��m�F
bool Game::CheckReadyToGenerateEnemy()const{
	return eg.CheckGaneratingState() == 3;
}
//���b�Z�[�W�E�C���h�E�̐����[�����m�F
bool Game::CheckTalkWindowActive()const{
	return mwm.IsTalkWindowActive();
}
//�G�̐����[�����m�F
bool Game::CheckEnemyCountZero()const{
	return cc.GetEnemyCount() == 0 ? true : false;
}
//�f�����댯��ԉ��m�F
bool Game::CheckPlanetDanger()const{
	if (planet == nullptr){ return false; }
	return planet->GetMinimamDistance() <= Planet_Danger_Radius;
}
//�f�����j�󂳂ꂽ���m�F
bool Game::CheckPlanetDestroy()const{
	if (planet == nullptr){ return false; }
	return planet->GetMinimamDistance() <= Planet_Destroy_Radius;
}
//���쒆�̃v���C���[�����񂾂�
bool Game::CheckUsingPlayerDead()const{
	if (_player->GetHP() <= 0){
		return true;
	}
	else{
		return false;
	}
}
//�R���e�B�j���[������
bool::Game::CheckContinued()const{ return _continue_flag; }

//�������֘A
int Game::SetupPlanet(){
	if (planet){ return -1; }
	planet = DefaultGameObjects::GetDefaultPlanet();
	if (planet == nullptr){ return -2; }
	if (planet->Init() < 0){ return -3; }
	//�L�����R���g���[���ɓo�^
	cc.SetBelongingPlanet(planet);
	//�G�Q�l��[���ɓo�^
	eg.SetPlanet(planet);
	return 0;
}
int Game::SetupBackGround(){
	if (bg){ return -1; }
	bg = DefaultGameObjects::GetDefaultBackGround();
	if (bg == nullptr){ return -2; }
//	if (bg->Init() < 0){ return -3; }
	return 0;
}

int Game::SetupInformationArea(){
	if (info_area){ return -1; }
	info_area = make_shared<InformationArea>();
	if (info_area == nullptr){ return -2; }
	if (info_area->Init() < 0){ return -3; }
	return 0;
}

//�L�����֐�
int Game :: ActivateGameComponent(const std::string& tar, bool af){
	if (tar == "InformationArea"){
		_info_area_active = af; 
	}
	else if (tar == "BackGround"){
		_bg_active = af;
	}
	else if (tar == "Planet"){
		_planet_active = af;
	}
	else if (tar == "All"){
		_info_area_active = af;
		_bg_active = af;
		_planet_active = af;
	}
	else{
		return -1;
	}
	return 0;
}

int Game::ResistPlayer(const string& ecid, const string& icon){
	auto chara = GetEventCharacterBySP(ecid);
	if (chara == nullptr){ return -1; }
	//�����ŃL������Player�N���X���m���߂�
	shared_ptr<Player> plr = dynamic_pointer_cast<Player>(chara);
	if (plr == nullptr){ return -2; }
	//�L�����R���g���[���ɓo�^
	cc.ResistPlayer(plr, ecid);
	//�G�Q�l��[���ɓo�^
	eg.SetPlayer(plr);
	//���G���A�ɓo�^
	if (info_area){ info_area->ResistPlayer(ecid, plr, icon); }
	return 0;
}

int Game::UnresistPlayer(const string& ecid){
	//cc.UnresistPlayer(ecid);
	////���ݑ��쒆�̃v���C���[��������
	//if (usg_player.second != nullptr &&  usg_player.first == ecid){
	//	//ID�̃��X�g��������
	//	_players.erase(find(_players.begin(),_players.end(),ecid));
	//	//�G�Q�l��[������o�^����
	//	eg.SetPlayer(nullptr);
	//	//���G���A����폜
	//	if (info_area){ info_area->UnesistPlayer(ecid); }

	//	usg_player = make_pair(-1, nullptr);
	//}
	return 0;
}

//�v���C���[�ύX(�ύX�ȊO�̏����͍s��Ȃ�)
int Game::ChangePlayer(const string& pid){
	//���łɎg�p��
	//if (_players.size()>0 && usg_player.second!=nullptr && _players.front() == pid){ return 0; }
	//auto it = find(_players.begin(), _players.end(), pid);
	////�Ȃ�
	//if (it == _players.end()){ return - 1; }
	////�v���[���[��̏�����ς���
	//while (_players.front()!=pid){
	//	string f = _players.front();
	//	_players.pop_front();
	//	_players.push_back(f);
	//}
	////���̈�ɃZ�b�g
	//if (info_area){ info_area->ChangePlayer(pid); }
	////�L�����R���g���[���[�ɒʒm
	//cc.ChangePlayer(pid);
	////_players�ɓo�^����Ă���ID�������L�����N�^��Player�^�łȂ���΂Ȃ�Ȃ��̂�static�L���X�g�ł���
	//auto ptr = static_pointer_cast<Player>(getEventCharacterBySP(pid));
	////�G�Q�l��[���ɃZ�b�g
	//eg.SetPlayer(ptr);
	////�g�p���v���[���[�Z�b�g
	//usg_player = make_pair(pid, ptr);

	//ptr->SetMode(Character::Mode::StandAlone);
	return 0;
}

int Game::SetPauseKind(int pk){
	_pause_kind = (PauseKind)pk;
	return 0;
}

//�Q�[���I�[�o�[����
int Game::GameOvered(){
	++_gameover_count.instance();
	_gameover_count.synchronize();
	GPIUtil::AddGameOverCount();
//	SceneManager::GetInstance().CreateAndChangeScene("Title");
	return 0;
}

//�Q�[���I��
int Game::GameEnd(){
	SceneManager::GetInstance().CreateAndChangeScene("Title");
	return 0;
}

//���ʏW�v����
int Game::StartResultCounting(int m){
	_player->ReceiveMessage("Muteki");
	_player->SetMode(Character::Mode::Control);
	_player->SetPolarVelocity(0, -1);
	if (m == 0){
		auto f = [this](void)->int{
			const int draw_intvl = 40;
			const int font_size = 32;
			int y_cnt = 1;

			if (_eve_inc_cnt == 0){
				sm.PlaySoundByID("ScoreDraw");
			}

				const int top_pos = 240;

				using namespace GPIUtil;
				int phs = GetHightScore();
				unsigned int ts = _total_score.get();

				{
					int sw = GetDrawFormatStringWidthToHandle(_score_font, "Final Score %d", ts);
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size, GetColor(255, 255, 255), _score_font, "Final Score %d", ts);
				}
				if (_eve_inc_cnt >= 60){
					if (phs < ts){
						if (_eve_inc_cnt == 60){ sm.PlaySoundByID("HighScore"); }
						int sw = GetDrawFormatStringWidthToHandle(_score_font, "High Score!!");
						DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos, GetColor(255, 255, 255), _score_font, "High Score!!");
					}
				}
				if (_eve_inc_cnt >= 180){
					if (_eve_inc_cnt == 180){
						sm.PlaySoundByID("ScoreDraw");
					}
					int sw = GetDrawFormatStringWidthToHandle(_score_font, "C-Key To Title");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size*3, GetColor(255, 255, 255), _score_font, "C-Key To Title");
					if (key.GetCustomInputPush(CUSTOM_INPUT_A)){
						if (phs < ts){ SetHighScore(ts); }
						AddGamePoint(ts);
						SceneManager::GetInstance().CreateAndChangeScene("Title");
						return 1;
					}
				}
			++_eve_inc_cnt;
			return 0;
		};
		SetEveFunc("ResultCounting", f);
	}
	else{
		auto f = [this](void)->int{
			const int draw_intvl = 40;
			const int font_size = 32;
			int y_cnt = 1;

			if (_eve_inc_cnt == 0){
				sm.PlaySoundByID("ScoreDraw");
			}

			if (_eve_inc_cnt <= 180){
				const int top_pos = 200;

				using namespace GPIUtil;
				int phs = GetHightScore();
				unsigned int ts = _total_score.get();

				{
					if (!_continue_flag){
						if (_eve_inc_cnt == 0){ 
							_total_score.set(_total_score.get() * 2);
							ts *=2; 
							GPIUtil::AddAllClearCount();
						}
						int sw = GetDrawFormatStringWidthToHandle(_score_font, "No continue clear!!! Score*2");
						DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size, GetColor(255, 255, 255), _score_font, "No continue clear!!! Score*2");
						sw = GetDrawFormatStringWidthToHandle(_score_font, "Final Score %d", ts);
						DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size*2, GetColor(255, 255, 255), _score_font, "Final Score %d", ts);
					}
					else{
						int sw = GetDrawFormatStringWidthToHandle(_score_font, "Final Score %d", ts);
						DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size, GetColor(255, 255, 255), _score_font, "Final Score %d", ts);
					}
				}
				if (_eve_inc_cnt >= 60){
					if (phs < ts){
						if (_eve_inc_cnt == 60){ sm.PlaySoundByID("HighScore"); }
						int sw = GetDrawFormatStringWidthToHandle(_score_font, "High Score!!");
						DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos, GetColor(255, 255, 255), _score_font, "High Score!!");
					}
				}
				if (_eve_inc_cnt == 180){
					if (_eve_inc_cnt == 180){
						sm.PlaySoundByID("ScoreDraw");
						sm.PlayMusicByID("Ending");
						bg->ChangeBackGround(7);

					}
					if (phs < ts){ SetHighScore(ts); }
					AddGamePoint(ts);
				}
			}
			else{
				int top_pos = 80;
				if (_eve_inc_cnt <= 540){
					int sw = GetDrawFormatStringWidthToHandle(_score_font, "Thank you for playing!!");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos, GetColor(255, 255, 255), _score_font, "Thank you for playing!!");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "--Material provided--");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 3, GetColor(255, 255, 255), _score_font, "--Material provided--");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "BGM");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 4, GetColor(255, 255, 255), _score_font, "BGM");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "<(C)PANICPUMPKIN>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 5, GetColor(255, 255, 255), _score_font, "<(C)PANICPUMPKIN>");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "Font");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 6, GetColor(255, 255, 255), _score_font, "Font");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "<Font AC>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 7, GetColor(255, 255, 255), _score_font, "<Font AC>");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "SoundEffect");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 8, GetColor(255, 255, 255), _score_font, "SoundEffect");
					sw = GetDrawFormatStringWidthToHandle(_score_font2, "<On-Jin�`���l�`>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 9, GetColor(255, 255, 255), _score_font2, "<On-Jin�`���l�`>");
					sw = GetDrawFormatStringWidthToHandle(_score_font2, "<�t���[���y�f�� Senses Circuit>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 10, GetColor(255, 255, 255), _score_font2, "<�t���[���y�f�� Senses Circuit>");
					sw = GetDrawFormatStringWidthToHandle(_score_font2, "<�t���[���ʉ��f�� ���炰�H��>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 11, GetColor(255, 255, 255), _score_font2, "<�t���[���ʉ��f�� ���炰�H��>");
					sw = GetDrawFormatStringWidthToHandle(_score_font2, "<���ʉ����{>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 12, GetColor(255, 255, 255), _score_font2, "<���ʉ����{>");
					sw = GetDrawFormatStringWidthToHandle(_score_font2, "<���X��>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 13, GetColor(255, 255, 255), _score_font2, "<���X��>");
					sw = GetDrawFormatStringWidthToHandle(_score_font2, "<���y�f��/������>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 14, GetColor(255, 255, 255), _score_font2, "<���y�f��/������>");
				}
				else{
					if(_eve_inc_cnt==541)sm.PlaySoundByID("ScoreDraw");
					int top_pos = 135;
					int sw = GetDrawFormatStringWidthToHandle(_score_font, "Thank you for playing!!");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos, GetColor(255, 255, 255), _score_font, "Thank you for playing!!");
					sw = GetDrawFormatStringWidthToHandle(_score_font, "Produced by <Cdec>");
					DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 3, GetColor(255, 255, 255), _score_font, "Produced by <Cdec>");
					if (_eve_inc_cnt > 840){
						if (_eve_inc_cnt == 841)sm.PlaySoundByID("ScoreDraw");
						sw = GetDrawFormatStringWidthToHandle(_score_font, "C-Key To Title");
						DrawFormatStringToHandle((_draw_area_size.x - sw) / 2, top_pos + font_size * 6, GetColor(255, 255, 255), _score_font, "C-Key To Title");
						if (key.GetCustomInputPush(CUSTOM_INPUT_A)){
							//				SceneManager::GetInstance().CreateAndChangeScene("Title");
							return 1;
						}
					}
				}
			}


			++_eve_inc_cnt;
			return 0;
		};
		SetEveFunc("ResultCounting", f);
	}
	return 0;
}

//���ʏW�v����
bool Game::IsCountingWaveEndScore()const{
	return _eve_func.first == "WaveScoreCounting";
}

//�g�I���X�R�A�W�v�J�n
int Game::StartWaveEndScoreCounting(){
	auto f = [this]()->int{
		const int draw_intvl=40;
		const int top_pos = 100;
		const int font_size = 32;
		int y_cnt=1;
		//�X�R�A�v�Z
		int score = GameCounter::LevelCounter::GetBaseScore();
		if (_eve_inc_cnt >= draw_intvl){
			if (_eve_inc_cnt == draw_intvl)sm.PlaySoundByID("ScoreDraw");
			//�m�[�_����������X�R�A��{
			int ed = GameCounter::LevelCounter::GetReceivedDamage();
			int sw = GetDrawFormatStringWidthToHandle(_score_font, "Recieved Damage %d", ed);
			int pos_x = (_draw_area_size.x - sw) / 2;
			DrawFormatStringToHandle(pos_x, font_size*y_cnt + top_pos, GetColor(255, 255, 255), _score_font, "Recieved Damage %d", ed);
			if (ed == 0){
				DrawFormatStringToHandle(pos_x+sw, font_size*y_cnt + top_pos, GetColor(255, 255, 255), _score_font, " Score *2");
				score *= 2;
			}
			++y_cnt;
		}
		if (_eve_inc_cnt >= draw_intvl * 2){
			if (_eve_inc_cnt == draw_intvl*2)sm.PlaySoundByID("ScoreDraw");
			int ee = GameCounter::LevelCounter::GetExplodedEnemy();
			int sw = GetDrawFormatStringWidthToHandle(_score_font, "Exploded Enemy %d", ee);
			int pos_x = (_draw_area_size.x - sw) / 2;
			DrawFormatStringToHandle(pos_x, font_size*y_cnt + top_pos, GetColor(255, 255, 255), _score_font, "Exploded Enemy %d", ee);
			//���������G�����Ȃ�������X�R�A*1.2
			if (ee == 0){
				DrawFormatStringToHandle(pos_x+sw, font_size*y_cnt + top_pos, GetColor(255, 255, 255), _score_font, " Score *1.2");
				score *= 1.2;
			}
			++y_cnt;
		}
		if (_eve_inc_cnt >= draw_intvl * 3){
			if (_eve_inc_cnt == draw_intvl*3)sm.PlaySoundByID("ScoreDraw");
			int mc = GameCounter::LevelCounter::GetMadeClone();
			int sw = GetDrawFormatStringWidthToHandle(_score_font, "Made Clone %d", mc);
			int pos_x = (_draw_area_size.x - sw) / 2;
			DrawFormatStringToHandle(pos_x, font_size*y_cnt+top_pos, GetColor(255, 255, 255), _score_font, "Made Clone %d", mc);
			//�N���[�����쐬���Ă��Ȃ�������X�R�A*1.5
			if (mc == 0){
				DrawFormatStringToHandle(pos_x+sw, font_size*y_cnt + top_pos, GetColor(255, 255, 255), _score_font, " Score *1.5");
				score *= 1.5;
			}
			++y_cnt;
		}
		if (_eve_inc_cnt >= draw_intvl * 5){
			if (_eve_inc_cnt == draw_intvl*5)sm.PlaySoundByID("ScoreDraw");
			unsigned int ts = _total_score.get() + score;
			int sw = GetDrawFormatStringWidthToHandle(_score_font, "Total Score %d", ts);
			int pos_x = (_draw_area_size.x - sw) / 2;
			++y_cnt;
			DrawFormatStringToHandle(pos_x, font_size* y_cnt + top_pos, GetColor(255, 255, 255), _score_font, "Total Score %d", ts);
			++y_cnt;
		}
		if (_eve_inc_cnt == 0)sm.PlaySoundByID("ScoreDraw");
		DrawFormatStringToHandle((_draw_area_size.x - GetDrawFormatStringWidthToHandle(_score_font, "Score %d", score))/2, top_pos, GetColor(255, 255, 255), _score_font, " Score %d", score);
		if (_eve_inc_cnt >= draw_intvl * 8){
			_total_score.instance() += score;
			_total_score.synchronize();
			GameCounter::LevelCounter::ApplyLevelCountToGPI();
			GameCounter::LevelCounter::ResetLevelCount();
			return 1;
		}
		++_eve_inc_cnt;
		if (key.GetCustomInputPush(CUSTOM_INPUT_A)){
			if (_eve_inc_cnt < draw_intvl * 5){ _eve_inc_cnt = draw_intvl * 5; }
			else{ _eve_inc_cnt = draw_intvl * 8; }
		}
		return 0;
	};
	SetEveFunc("WaveScoreCounting",f);
	return 0;
}

//�g�X�R�A���ƒ���
bool Game::IsCountingResult()const{
	return _eve_func.first=="ResultCounting";
}

int Game::SavePlayerEnergy(){
	_saved_player_exp = _player->GetEXP() > _saved_player_exp ? _player->GetEXP() : _saved_player_exp;
	dc->SetData("PlayerEXP", _saved_player_exp);
	return 0;
}

//�R���e�B�j���[
int Game::ContinueGame(int stage_num){
	dc->SetData("GameInitializerID", string("NormalStageInitializer"));
	dc->SetData("GameStartEvent", string("StartEvent"));
	dc->SetData("GameStartEventParam", stage_num);
	SceneManager::GetInstance().CreateAndChangeScene("Game");
	GPIUtil::AddContinueCount();
	dc->SetData("ContinueFlag", true);
	Game::SavePlayerEnergy();
	dc->SetData("Score", _total_score.get());
	return 0;
}

//�Q�[���N���A
int Game::GameCleared(){
	GPIUtil::AddNormalClearCount();
	SceneManager::GetInstance().CreateAndChangeScene("Title");
	return 0;
}


