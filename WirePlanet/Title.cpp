#include "Title.h"
#include"DefaultGameObjects.h"
#include"Key.h"
#include"SceneManager.h"
#include"SoundManager.h"
#include"ScreenManager.h"
#include"boost/lexical_cast.hpp"
#include"InitDatas.h"
#include"GraphicManager.h"
#include"GraphicController2D.h"
#include"MenuItems.h"
#include"GPIUtil.h"
#include"SystemVariable.h"
#include"FileIOUtilFuncs.h"

using namespace std;

/*Title�V�[���O���[�o���V�[���ϐ�
TitleCurPos:�O��I�����̃J�[�\���ʒu
TitleState:�^�C�g���̏��(0:�^�C�g�� 1:���j���[�Z���N�g)
*/

namespace{
	const string LOGO_GRAPH("TitleLogo");
}

Title::Title() :_title_logo_gh(-1), _inc_cntr(0), _start_stage_num(0), menu_items(make_shared<MenuItems>()), _state(0), _total_point(0), _nmode_high_score(0), _state_cntr(0)
{
}


Title::~Title()
{
	for (auto it : _actions){
		delete it;
	}
	_actions.clear();
}

int Title::Init(shared_ptr<DataContainer> dc){
	title_font_0 = CreateFontToHandle("���C���I", 72, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	title_font_1 = CreateFontToHandle("���C���I", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	planet = DefaultGameObjects::GetDefaultPlanet();
	bg = DefaultGameObjects::GetDefaultBackGround();
	bg->ChangeBackGround(GPIUtil::GetNormalClearCount() > 0 ? 7 : 0, true); //�m�[�}���N���A�ς݂�������w�i���
	_draw_area_size.x = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawWidth"));
	_draw_area_size.y = boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawHeight"));
	_title_logo_gh = GraphicManager::GetInstance().GetGraphHandle(LOGO_GRAPH);
	planet->SetXY(Vector2D<double>(_draw_area_size.x/2, _draw_area_size.y/2+200));
	gc2d::GetCamera2D().Reset();
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("Player")));
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("GreenEnemy")));
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("SkyblueEnemy")));
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("RedEnemy")));
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("PurpleEnemy")));
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("PinkEnemy")));
	_actions.push_back(new Action(ActionDataAdmin::GetInstance().GetActionData("WhiteEnemy")));
	//���j���[������
	menu_items->ClearMenuItem();
	menu_items->SetLineHeight(16);
	menu_items->SetMargine(3);
	menu_items->SetPosition(Vector2D<int>(70, 300));
	menu_items->SetSize(Vector2D<int>(100,100));
	//���j���[���ڒǉ�
	menu_items->AddMenuItem(0, "�͂��߂�");
	//	menu_items->AddMenuItem(1,"�G���h���X");
//	menu_items->AddMenuItem(1, "�V���b�v");
	menu_items->AddMenuItem(1, "�}�j���A��");
	menu_items->AddMenuItem(2, "�v���C�L�^");
	//	menu_items->AddMenuItem(4,"���܂�");
	//	menu_items->AddMenuItem(5, "�A�b�v�f�[�g");
	//	menu_items->AddMenuItem(6,"�ݒ�");
	menu_items->AddMenuItem(3, "�I��");
	if (SysVar::DEBUG_LEVEL == 0){
		menu_items->AddMenuItem(4, "�o�[�W����");
	}
	else if (SysVar::DEBUG_LEVEL >= 2){
		menu_items->AddMenuItem(4, "�e�X�g�X�e�[�W");
		menu_items->AddMenuItem(5, "�X�e�[�W�w��");
	}
	else{

	}
	//���̑����擾
	_total_point = GPIUtil::GetGamePoint();
	_nmode_high_score = GPIUtil::GetHightScore();

	_inc_cntr = 0;
	_state_cntr = 0;
	_state = 0;

	int cp=0;
	int ts = 0;
	if (dc->GetData("TitleState", ts) >= 0){
		if (ts == 1){
			if (dc->GetData("TitleCurPos", cp) >= 0){
				while (menu_items->GetCurrentCursorPosition() != cp){
					menu_items->ProgressCursor();
				}
			}
			_state = 1;
		}
	}

	Scene::Init(dc);
	return 0;
}

int Title::End(){
	SoundManager::GetInstance().StopMusic();
	DeleteFontToHandle(title_font_0);
	DeleteFontToHandle(title_font_1);
	for (auto it : _actions){
		delete it;
	}
	_actions.clear();

	dc->SetData("TitleCurPos", menu_items->GetCurrentCursorPosition());

	Scene::End();
	return 0;
}

void Title::Update(){
	if (SoundManager::GetInstance().CheckAssignedMusicPlaying("Title") == 0){
		SoundManager::GetInstance().PlayMusicByID("Title");
	}
	bg->Update();
	planet->Update();
	switch (_state){
	case 0:
		if (Key::GetInstance().GetCustomInputPush(CUSTOM_INPUT_START) || Key::GetInstance().GetCustomInputPush(CUSTOM_INPUT_A)){
			SoundManager::GetInstance().PlaySoundByID("TitleSelect");
			_state_cntr = 0;
			_state = 2;
		}
		break;
	case 2:
		if (_state_cntr > 30){
			_state_cntr = 0;
			_state = 3; 
		}
		break;
	case 3:
		if (_state_cntr > 30){
			_state_cntr = 0;
			_state = 1;
		}
	case 4:
		if (_state_cntr > 30){
			_state_cntr = 0;
			_state = 0;
		}
		break;
	case 1:
		if (key.GetCustomInputPush(CUSTOM_INPUT_A)){
			int cp = menu_items->GetCurrentCursorPosition();
			switch (cp)
			{
			case 4: //�o�[�W�����A�e�X�g�X�e�[�W
				if (SysVar::DEBUG_LEVEL == 0){
					SoundManager::GetInstance().PlaySoundByID("Cancel");
					break; 
				}
				//�Q�[���J�n
			case 0:case 5: //�Q�[���J�n�A�X�e�[�W�w��
			{
				string game_init_id("NormalStageInitializer"); //�Q�[���V�[��������ID
				string game_start_event("StartEvent"); //�Q�[���V�[���J�n�C�x���g
				int game_start_event_param = 0; //�Q�[���V�[���J�n�C�x���g�p�����[�^
				if (cp == 0){  //�m�[�}�����[�h�B�X�e�[�W1����
					game_start_event_param = 1;
				}
				else if (cp == 4){ game_start_event_param = -1; } //�e�X�g�X�e�[�W
				else if (cp == 5){ game_start_event_param = _start_stage_num; } //�X�e�[�W�w��
				dc->SetData("GameInitializerID", game_init_id);
				dc->SetData("GameStartEvent", game_start_event);
				dc->SetData("GameStartEventParam", game_start_event_param);
				dc->SetData("ContinueFlag", false);
				SceneManager::GetInstance().CreateAndChangeScene("Game");
				SoundManager::GetInstance().PlaySoundByID("Define");
				break;
			}
			case 2: //�v���C����
				SceneManager::GetInstance().CreateAndChangeScene("ViewPlayInformations");
				SoundManager::GetInstance().PlaySoundByID("Define");
				break;
			case 3: //�I��
				SceneManager::GetInstance().EndGameRequest();
				break;
			case 1: //Manual
				{
					int ret = MessageBox(GetMainWindowHandle(), "����̃u���E�U�Ń}�j���A�����J���܂��B", "�m�F", MB_OKCANCEL | MB_DEFBUTTON1 | MB_ICONINFORMATION | MB_APPLMODAL);
					if (ret == IDOK){
						printf("Title::Update Manual���J���܂����B\n");
						if (FileIOUtilFuncs::ExcuteFile("manual\\index.html") >= 0){
							SoundManager::GetInstance().PlaySoundByID("Define");
						}
						else{
							MessageBox(GetMainWindowHandle(), "�}�j���A�����J�����Ƃ��ł��܂���ł����Bmanual�t�H���_��index.html�𒼐ڊJ���Ă��������B", "�G���[", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_APPLMODAL);
							SoundManager::GetInstance().PlaySoundByID("Cancel");
						}
					}
					else if (ret = IDCANCEL){
						printf("Title::Update Manual���J���܂���ł����B\n");
						SoundManager::GetInstance().PlaySoundByID("Cancel");
					}
					else{
						printf("Title::Update ���b�Z�[�W�{�b�N�X�̖߂�l���s���ł��B\n");
					}
				}
				break;
			default:
				SoundManager::GetInstance().PlaySoundByID("Cancel");
				break;
			}
		}
		else if (key.GetCustomInputPush(CUSTOM_INPUT_B)){
			SoundManager::GetInstance().PlaySoundByID("Cancel");
			_state_cntr = 0;
			_state = 4;
		}
		if (key.GetCustomInputPush(CUSTOM_INPUT_UP)){
//			miis->ChangeSelectedItem(menu_items->BackCursor());
			menu_items->BackCursor();
			SoundManager::GetInstance().PlaySoundByID("Select");
		}
		if (key.GetCustomInputPush(CUSTOM_INPUT_DOWN)){
//			miis->ChangeSelectedItem(menu_items->ProgressCursor());
			menu_items->ProgressCursor();
			SoundManager::GetInstance().PlaySoundByID("Select");
		}
		if (key.GetCustomInputPush(CUSTOM_INPUT_LEFT)){
			--_start_stage_num;
		}
		if (key.GetCustomInputPush(CUSTOM_INPUT_RIGHT)){
			++_start_stage_num;
		}
		menu_items->Update();
//		miis->Update();
		break;
	}
	++_inc_cntr;
	++_state_cntr;
	return ;
}

void Title::Draw(){
	ScreenManager::GetInstance().PushScreen();
	bg->Draw();
	planet->Draw();
	draw_chara();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	switch (_state){
	case 0:
		if (GetNowCount() % 1000 >= 500){
			DrawStringToHandle((_draw_area_size.x - GetDrawStringWidthToHandle("PUSH SPACE OR C-KEY", 19, title_font_1)) / 2, 360, "PUSH SPACE OR C-KEY", GetColor(255, 255, 255), title_font_1);
		}
		break;
	case 2:
		if (GetNowCount() % 60 >= 30){
			DrawStringToHandle((_draw_area_size.x - GetDrawStringWidthToHandle("PUSH SPACE OR C-KEY", 19, title_font_1)) / 2, 360, "PUSH SPACE OR C-KEY", GetColor(255, 255, 255), title_font_1);
		}
		break;
	case 3:
	{
		auto pb = menu_items->GetPosition();
		double rat = pow(cos(DX_PI / 2 * (double)_state_cntr / 30), 2);
		const int f_size = 24;
		menu_items->SetPosition(pb - Vector2D<int>((int)(250 * rat), 0));
		menu_items->Draw();
		menu_items->SetPosition(pb);
		DrawFormatStringToHandle(0, (int)(0-rat*f_size), GetColor(255, 255, 255), title_font_1, "HighScore %d", (int)_nmode_high_score);
		DrawFormatStringToHandle(_draw_area_size.x - GetDrawFormatStringWidthToHandle(title_font_1, "GamePoint %d", (int)_total_point), (int)(0 - f_size * rat), GetColor(255, 255, 255), title_font_1, "GamePoint %d", (int)_total_point);
		break;
	}
	case 4:
	{
		auto pb = menu_items->GetPosition();
		double rat = pow(cos(DX_PI / 2 * (double)(30-_state_cntr) / 30), 2);
		const int f_size = 24;
		menu_items->SetPosition(pb - Vector2D<int>((int)(250 * rat), 0));
		menu_items->Draw();
		menu_items->SetPosition(pb);
		DrawFormatStringToHandle(0, (int)(0 - rat * f_size), GetColor(255, 255, 255), title_font_1, "HighScore %d", (int)_nmode_high_score);
		DrawFormatStringToHandle(_draw_area_size.x - GetDrawFormatStringWidthToHandle(title_font_1, "GamePoint %d", (int)_total_point), (int)(0 - f_size * rat), GetColor(255, 255, 255), title_font_1, "GamePoint %d", (int)_total_point);
		break;
	}
	case 1:
		menu_items->Draw();
		DrawFormatStringToHandle(0, 0, GetColor(255, 255, 255), title_font_1, "HighScore %d", (int)_nmode_high_score);
		DrawFormatStringToHandle(_draw_area_size.x-GetDrawFormatStringWidthToHandle(title_font_1,"GamePoint %d",(int)_total_point), 0, GetColor(255, 255, 255), title_font_1, "GamePoint %d", (int)_total_point);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	}
	ScreenManager::GetInstance().ApplyEffectToTopScreen(ScreenManager::ScreenEffect::Broom, 1025);
	if (menu_items->GetCurrentCursorPosition() == 5){
		DrawFormatString(100, 100, GetColor(255, 255, 255), "start_stage_num=%d", _start_stage_num);
	}
//	DrawStringToHandle((_draw_area_size.x - GetDrawStringWidthToHandle("WirePlanet v0.5", 15, title_font_0)) / 2, 50, "WirePlanet v0.5", GetColor(255, 255, 255), title_font_0);
	DrawExtendGraph(0, 50, _draw_area_size.x, 300, _title_logo_gh, true);
	if (SysVar::DEBUG_LEVEL>0 || (SysVar::DEBUG_LEVEL == 0 && menu_items->GetCurrentCursorPosition()==4))DrawStringToHandle((_draw_area_size.x - GetDrawFormatStringWidthToHandle(title_font_1, SysVar::VERSION_STRING)), 300, SysVar::VERSION_STRING, GetColor(255, 255, 255), title_font_1);
}

void Title::draw_chara(){
	Vector2D<double> p_pos = planet->GetPositionXY();
	double p_r = planet->GetRadius()+16;
	for (int i = 0; i < (signed)_actions.size(); ++i){
		Vector2D<double> pos = p_pos;
		double t = 2.0 * DX_PI / _actions.size()*i + (DX_PI / 240)*_inc_cntr;
		pos += Vector2D<double>(p_r*cos(t), p_r*sin(t));
		gc2d::DrawRotaGraphF((float)pos.x, (float)pos.y, 1.0, t+DX_PI/2, _actions[i]->GetCurrentGraph(), true);
	}
}
