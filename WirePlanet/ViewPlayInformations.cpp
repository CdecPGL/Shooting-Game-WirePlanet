#include "ViewPlayInformations.h"
#include"SceneManager.h"
#include"GPIUtil.h"
#include"ScreenManager.h"
#include"SoundManager.h"
#include"GPIUtil.h"
#include"background.h"

ViewPlayInformations::ViewPlayInformations() :_play_frame(0), _play_second(0), _cntr(Counter::GetInstance()), _pre_c_play_second(0), _ave_fps(60), bg(make_unique<BackGround>())
{
}

ViewPlayInformations::~ViewPlayInformations()
{
}

int ViewPlayInformations::Init(shared_ptr<DataContainer> dc){
	Scene::Init(dc);
	bg->ChangeBackGround(GPIUtil::GetNormalClearCount() > 0 ? GetRand(6) + 1 : 0, true); //�m�[�}���N���A�ς݂�������w�i�������_����
	_play_frame = GPIUtil::GetPlayFrame();
	_play_second = GPIUtil::GetPlaySecond();
	return Scene::Init(dc);
}

int ViewPlayInformations::End(){
	dc->SetData("TitleState", 1);
	return Scene::End();
}

void ViewPlayInformations::Update(){
	if (SoundManager::GetInstance().CheckAssignedMusicPlaying("Information") == 0){
		SoundManager::GetInstance().PlayMusicByID("Information");
	}
	if (key.GetCustomInputPush(CUSTOM_INPUT_B)){
		SceneManager::GetInstance().CreateAndChangeScene("Title");
		SoundManager::GetInstance().PlaySoundByID("Cancel");
	}
	bg->Update();
}

void ViewPlayInformations::Draw(){
	ScreenManager::GetInstance().PushScreen();
	using namespace GPIUtil;
	int line_height(20);
	int left_margin(20);
	int top_margin(20);
	bg->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	int l = 0;
	{
		DateTime dt = GetFirstPlayDateTime();
		DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�ŏ��ɋN���������� : ����%04d�N%02d��%02d��%02d��%02d��%02d�b", dt.year, dt.month, dt.day, dt.hour, dt.minuits, dt.second);

	}
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�N���� : %d ��", GetPlayCount());
	{
		auto c_play_frame = _play_frame + _cntr.GetFrameFromStart();
		auto c_play_second = _play_second + _cntr.GetTimeFromStartBySecond();
		unsigned int h, m, s;
		if (_pre_c_play_second != c_play_second){
			_ave_fps = (double)c_play_frame / c_play_second;
		}
		_pre_c_play_second = c_play_second;
		_calc_hms_from_s(c_play_second, &h, &m, &s);
		DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "���v�N������ : %d����%d��%d�b , %d�t���[��",(int)h,(int)m,(int)s, (int)c_play_frame);
		DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "����N��������̕��σt���[�����[�g : %lf FPS", _ave_fps);
	}
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�Q�[���|�C���g : %d Points", GetGamePoint());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "Normal���[�h�n�C�X�R�A : %d", GetHightScore());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�|�����G�̑��� : %d ��", GetDefeatedEnemy());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "���˂����e�̑��� : %d ��", GetShootedBullet());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�쐬�����N���[���̑��� : %d ��", GetMadeClone());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�Q�[���I�[�o�[�� : %d ��", GetGameOverCount());
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�R���e�B�j���[�� : %d ��", GetContinueCount());
	{
		int acc = GetAllClearCount();
		if (acc == 0){
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�N���A�� : %d ��", GetNormalClearCount());
		}
		else{
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�N���A�� : %d ��", GetNormalClearCount());
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�m�[�R���e�B�j���[�N���A�� : %d ��I�I�I", acc);
		}
	}
	{
		int ec = GetErrorCount();
		if (ec > 0){
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "���o�\�ȃG���[�ɂ��Q�[������~������ : %d �� "/* ���߂�Ȃ���m(_ _)m*/, ec);
		}
	}
	{
		int cc = GetCheatCount();
		if (cc > 0){
			DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "�`�[�g���o�� : %d ��  �`�[�g�͂�߂Ă�", cc);
		}
	}
	++l;
	DrawFormatString(left_margin, l++ * line_height + top_margin, GetColor(255, 255, 255), "A�L�[�Ŗ߂�");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	ScreenManager::GetInstance().ApplyEffectToTopScreen(ScreenManager::ScreenEffect::Broom, 1025);
}

void ViewPlayInformations::_calc_hms_from_s(unsigned long long s, unsigned int* hb, unsigned int* mb, unsigned int* sb){
	*hb = s / 3600;
	*mb = (s / 60) % 60;
	*sb = s % 60;
}
