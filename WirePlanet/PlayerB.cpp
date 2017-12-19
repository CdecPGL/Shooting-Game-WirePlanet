#include "PlayerB.h"
#include"LowStateIndivisualManager.h"
#include"SoundManager.h"
#include"Planet.h"
#include"CreateCharaFuncs.h"
#include"CharacterManager.h"
#include"PlayerClone.h"
#include<algorithm>
#include"GameCounter.h"

using namespace std;

extern const int CHARACTER_DEFAULT_HIT_STOP_FRAME; //�q�b�g�X�g�b�v�̒���
namespace{
	const int UD_THROW_POS_DISTANCE(32); //�㉺�������̓����n�߂̃L�����Ƃ̋���
	const int LR_THROW_POS_DISTANCE(27); //���E�������̓����n�߂̃L�����Ƃ̋���
	const double PLAYER_JUMP = 5; //�v���C���[�̃W�����v�����x
	const double CLONE_EXP_RATE(0.2); //�N���[����EXP���[�g
	const string MAKE_CLONE_SOUND("PutClone");
	const unsigned int CREATE_CLONE_INTERVAL(30); //�N���[���쐻�Ԋu
}

PlayerB::PlayerB(double m_f, long m_hp, double m_s, long m_mt, const std::string& m_an) :Player(m_f, m_hp, m_s, m_mt, m_an), _jump_suspend(0), _jumping_frame(0), _barrier_flag(false), _cntr_from_create_clone(CREATE_CLONE_INTERVAL)
{
}


PlayerB::~PlayerB()
{
}

void PlayerB::SetParameterByLevel(int l){
	_shoot_interval = 7 - l / 5;
	_shoot_speed = 10.0 + (double)l/2.5;
	_shoot_power = 1.0 + (l - 1) / 15;
	if (_shoot_power > 3){ _shoot_power = 3; }
	_throw_speed = 3.0 + (double)l / 5;
	_throw_power = 1.0 + (l - 1) / 10;
	_h_shoot_suspend = 10 + l / 2.5;
	_max_throw_num = 1 + (l - 1) / 5;
	_walk_acceralation = 0.3 + (double)l / 30;
	_jump_suspend = 15 + l/3.5;
	//HP1�����茻�݂̃��x���Ǝ��̃��x���̌o���l���̎O�\���̈�������
	_exp_to_hp_rate = (double)GetExpDifferenceToNextLevel() / 30.0;
	//��������1/800
	_barrier_exp_consumption = (double)GetExpDifferenceToNextLevel() / 800.0;
}

void PlayerB::ProcessCollisionWithCharacter(){
	for (CollisionDataWithCharacter& c : col_data_c){
		//�Փˑ��肪����������������
		auto it = find(throwed_sword_id.begin(), throwed_sword_id.end(), c.col_chara_id);
		if (it != throwed_sword_id.end()){
			CharacterManager::GetInstance().DeleteObject(*it);
			throwed_sword_id.erase(it);
			continue;
		}
		//�G����_���[�W���󂯂���
		if (c.receive_damage>0 && !_barrier_flag){
			matchless_time = c.matchless_time;
			hit_stop_cnt = CHARACTER_DEFAULT_HIT_STOP_FRAME;
			ReceivedAttack(c.receive_damage);
			_col_revers_flag = true;
			if (c.receive_damage > 0){
				GameCounter::LevelCounter::AddReceivedDamage(c.receive_damage);
			}
			SoundManager::GetInstance().PlaySoundByID("Damaged");
		}
		//�G�Ƀ_���[�W��^������
		if (c.give_damage){ hit_stop_cnt = CHARACTER_DEFAULT_HIT_STOP_FRAME; }
	}
	CharacterBase::ProcessCollisionWithCharacter();
}

void PlayerB::DieState(){
	Character::DieState();
	SoundManager::GetInstance().PlaySoundByID("PlayerDead");
}

void PlayerB::SetNormalStateLowStateFuncMap(){
	//���ʏ�ԌʊǗ��ҍ쐬
	LowStateIndivisualManagerC<PlayerB>* ns_ls_mgr_c = new LowStateIndivisualManagerC<PlayerB>(this);
	ns_ls_mgr_c->ResistLowState("normal", &PlayerB::NormalLStateEnter, &PlayerB::NormalLStateUpdate, nullptr);
	ns_ls_mgr_c->ResistLowState("hshoot", &PlayerB::HShootLStateEnter, &PlayerB::HShootLStateUpdate, nullptr);
	ns_ls_mgr_c->ResistLowState("barrier", &PlayerB::BarrierLStateEnter, &PlayerB::BarrierLStateUpdate, &PlayerB::BarrierLStateLeave);
	ns_ls_mgr_c->ResistLowState("squat", &PlayerB::SquatLStateEnter, &PlayerB::SquatLStateUpdate, &PlayerB::SquatLStateLeave);
	ns_ls_mgr_c->ResistLowState("create_clone", &PlayerB::CreateCloneLStateEnter, &PlayerB::CreateCloneLStateUpdate, &PlayerB::CreateCloneLStateLeave);
	// ���ʏ�ԌʊǗ��҃Z�b�g
	ns_ls_mgr.reset(ns_ls_mgr_c);
	//������Ԃ�"idol"�ɃZ�b�g
	ns_ls_mgr->ChangeLowState("normal");
}

///NormalStat���ʏ�Ԋ֐�
//���ʏ���
void PlayerB::UniversalLowStateProc(){
	//L�Ōo���l��̗͂ɕϊ�
	if (ButtonStatus(PlayerInput::L)){
		if (_total_exp_s.get() > 0 && p_HP < HP){
			AddExperiencePoint(-_exp_to_hp_rate);
			++p_HP;
		}
	}
	++_cntr_from_create_clone;
}

void PlayerB::NormalLStateUpdate(){
	//���s
	bool walking=false;
	if (ButtonStatus(PlayerInput::LEFT)){
		if (ground_flag){ vh -= _walk_acceralation; }
		else{ vh -= _walk_acceralation / 2; }
		reverse = true;
		walking = true;
	}
	if (ButtonStatus(PlayerInput::RIGHT)){
		if (ground_flag){ vh += _walk_acceralation; }
		else{ vh += _walk_acceralation / 2; }
		reverse = false;
		walking = true;
	}
	if (fabs(vh) > maximum_speed){ vh = (vh / fabs(vh)) * maximum_speed; }
	//���V���b�g
	if (ButtonPush(PlayerInput::Y)){ ns_ls_mgr->ChangeLowState("hshoot"); }
	//��V���b�g
	if (ns_vars[0] >= _shoot_interval){
		if (ButtonStatus(PlayerInput::B)){
			if (_level < 10){
				CreateCharaFuncs::FireBullet("PlayerBullet", 90.0, _shoot_speed, _shoot_power, this);
				GameCounter::LevelCounter::AddShootedBullet(1);
				SoundManager::GetInstance().PlaySoundByID("PlayerShoot");
			}
			else if (_level < 20){
				CreateCharaFuncs::FireBullet("PlayerBullet", 92.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 88.0, _shoot_speed, _shoot_power, this);
				GameCounter::LevelCounter::AddShootedBullet(2);
				SoundManager::GetInstance().PlaySoundByID("PlayerShoot");
			}
			else if (_level < 30){
				CreateCharaFuncs::FireBullet("PlayerBullet", 95.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 90.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 85.0, _shoot_speed, _shoot_power, this);
				GameCounter::LevelCounter::AddShootedBullet(3);
				SoundManager::GetInstance().PlaySoundByID("PlayerShoot");
			}
			else{
				CreateCharaFuncs::FireBullet("PlayerBullet", 100.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 95.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 90.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 85.0, _shoot_speed, _shoot_power, this);
				CreateCharaFuncs::FireBullet("PlayerBullet", 80.0, _shoot_speed, _shoot_power, this);
				GameCounter::LevelCounter::AddShootedBullet(5);
				SoundManager::GetInstance().PlaySoundByID("PlayerShoot");
			}
			ns_vars[0] = 0;
		}
	}
	++ns_vars[0];
	//���C(�����Ă��Ĉړ��L�[��������ĂȂ�������)
	if (!walking){ Friction(); }
	//�W�����v
	if (ButtonPush(PlayerInput::A)&&ground_flag){
		vr += PLAYER_JUMP;
		_jumping_frame = 0;
		SoundManager::GetInstance().PlaySoundByID("PlayerJump");
	}
	else if (ButtonStatus(PlayerInput::A) && !ground_flag && _jumping_frame<_jump_suspend){
		++_jumping_frame;
	}else{
		//�d��(UPKEY��������Ă���キ����)
		Gravity(ButtonStatus(PlayerInput::UP) ? 0.5 : 1.0);
	}
	//R�Ōo���l���g���ăo���A
	if (ButtonPush(PlayerInput::R)){
		ns_ls_mgr->ChangeLowState("barrier");
	}
	//���Ⴊ��
	if (ButtonStatus(PlayerInput::DOWN)){
		ns_ls_mgr->ChangeLowState("squat");
	}
	//�N���[���쐬
	if (ButtonPush(PlayerInput::X)){
		ns_ls_mgr->ChangeLowState("create_clone");
	}
}

void PlayerB::NormalLStateEnter(){
	action->StartActionByTag("idol");
	ns_vars[0] = 0;
}

void PlayerB::HShootLStateUpdate(){
	ns_ls_mgr->ChangeLowState("normal");
}

void PlayerB::HShootLStateEnter(){
	ns_vars[0] = 0;
	CreateCharaFuncs::FireBulletAssignSuspend("PlayerBullet", 0.0, _shoot_speed, _throw_power, this,_h_shoot_suspend);
	CreateCharaFuncs::FireBulletAssignSuspend("PlayerBullet", 180.0, _shoot_speed, _throw_power, this,_h_shoot_suspend);
	SoundManager::GetInstance().PlaySoundByID("PlayerShoot2");
	GameCounter::LevelCounter::AddShootedBullet(2);
}

void PlayerB::BarrierLStateUpdate(){
	if (!ButtonStatus(PlayerInput::R) || _total_exp_s.get()<=0){
		ns_ls_mgr->ChangeLowState("normal");
	}
	AddExperiencePoint(-_barrier_exp_consumption);
	Friction();
	Gravity();
	if (ns_vars[0] % 3 == 0)SoundManager::GetInstance().PlaySoundByID("PlayerBarrier");
	++ns_vars[0];
}

void PlayerB::BarrierLStateEnter(){
	action->StartActionByTag("barrier");
	_barrier_flag = true;
	ns_vars[0] = 0;
}

void PlayerB::BarrierLStateLeave(){
	action->StartActionByTag("idol");
	_barrier_flag = false;
}

void PlayerB::SquatLStateEnter(){
	action->StartActionByTag("squat");
	SoundManager::GetInstance().PlaySoundByID("PlayerSquatS");
}

void PlayerB::SquatLStateUpdate(){
	Gravity(2);
	Friction();
	if (!ButtonStatus(PlayerInput::DOWN)){ 
		ns_ls_mgr->ChangeLowState("normal");
		SoundManager::GetInstance().PlaySoundByID("PlayerSquatE");
	}
}

void PlayerB::SquatLStateLeave(){
	action->StartActionByTag("normal");
}

void PlayerB::CreateCloneLStateUpdate(){
	if (!ButtonStatus(PlayerInput::X) || _total_exp_s.get() <= 0 || _cntr_from_create_clone < CREATE_CLONE_INTERVAL){
		ns_ls_mgr->ChangeLowState("normal");
		return;
	}
	int m = GetExpDifferenceToNextLevel()/100;
	ns_vars[2] += (_total_exp_s.get() >= m ? m : _total_exp_s.get())/CLONE_EXP_RATE;
	AddExperiencePoint(-m);
	Gravity();
	Friction();
	if(ns_vars[0]%5==0)_col_revers_flag = true;
	++ns_vars[0];
}

void PlayerB::CreateCloneLStateEnter(){
//	if (_cntr_from_create_clone < CREATE_CLONE_INTERVAL){ ns_ls_mgr->ChangeLowState("normal");}
	ns_vars[2] = 0;
}

void PlayerB::CreateCloneLStateLeave(){
	if (ns_vars[2] <= 0){ return; }
	shared_ptr < Player >  p = make_shared<PlayerClone>(120+sqrt(ns_vars[2])*5, matchless_time, "Player");
	p->SetBelongingPlanet(myplanet);
	p->SetTR(t, r);
	p->Init();
	p->AddExperiencePoint(ns_vars[2]);
	CharacterManager::GetInstance().ResistObject(p);
	SoundManager::GetInstance().PlaySoundByID(MAKE_CLONE_SOUND);
	GameCounter::LevelCounter::AddMadeClone(1);
	_cntr_from_create_clone = 0;
}
