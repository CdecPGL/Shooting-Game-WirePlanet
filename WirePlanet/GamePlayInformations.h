#pragma once
#include"SecureObject.h"
#include"boost/serialization/access.hpp"
#include"boost/serialization/serialization.hpp"
#include"boost/serialization/version.hpp"
#include"boost/serialization/split_member.hpp"

class GamePlayInformations
{
public:
	~GamePlayInformations();
	static GamePlayInformations& GetInstance();

	secure_obj::SecureObject<unsigned int> _points; //�|�C���g
	secure_obj::SecureObject<unsigned int> _hight_score; //�n�C�X�R�A
	secure_obj::SecureObject<unsigned long long> _play_frame; //�N������(�t���[��)
	secure_obj::SecureObject<unsigned long long> _play_second; //�N������(�b)
	secure_obj::SecureObject<unsigned int> _play_count; //�N����
	secure_obj::SecureObject<unsigned int> _first_play_year; //�ŏ��ɋN�������N
	secure_obj::SecureObject<unsigned int> _first_play_month; //�ŏ��ɋN��������
	secure_obj::SecureObject<unsigned int> _first_play_day; //�ŏ��ɋN��������
	secure_obj::SecureObject<unsigned int> _first_play_hour; //�ŏ��ɋN��������
	secure_obj::SecureObject<unsigned int> _first_play_minuits; //�ŏ��ɋN��������
	secure_obj::SecureObject<unsigned int> _first_play_second; //�ŏ��ɋN�������b
	secure_obj::SecureObject<unsigned int> _defeated_enemy; //�|�����G�̐�
	secure_obj::SecureObject<unsigned int> _shooted_bullet_num; //���˂����ʂ̐�
	secure_obj::SecureObject<unsigned int> _made_clone_num; //�쐬�������g�̐�
	secure_obj::SecureObject<unsigned int> _game_over_count; //�Q�[���I�[�o�[��
	secure_obj::SecureObject<unsigned int> _continue_count; //�R���e�B�j���[��
	secure_obj::SecureObject<unsigned int> _normal_clear_count; //�ʏ�N���A��
	secure_obj::SecureObject<unsigned int> _all_clear_count; //���S�N���A��
	secure_obj::SecureObject<unsigned int> _cheat_count; //�`�[�g���o��
	secure_obj::SecureObject<unsigned int> _error_count; //�G���[�ɂ��Q�[�������f���ꂽ��
private:
	GamePlayInformations();
	friend boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER();
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		//v1
		ar&_made_clone_num.get();
		//v0
		ar & _points.get() & _hight_score.get();
		ar&_play_frame.get()&_play_second.get()&_play_count.get();
		ar&_first_play_year.get()&_first_play_month.get()&_first_play_day.get()&_first_play_hour.get()&_first_play_minuits.get()&_first_play_second.get();
		ar&_defeated_enemy.get()&_shooted_bullet_num.get();
		ar&_game_over_count.get()&_continue_count.get()&_normal_clear_count.get()&_all_clear_count.get();
		ar&_cheat_count.get()&_error_count.get();
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		switch (version){
		case 1:
			ar&_made_clone_num.instance();
			_made_clone_num.synchronize();
		case 0:
			ar & _points.instance() & _hight_score.instance();
			ar&_play_frame.instance()&_play_second.instance()&_play_count.instance();
			ar&_first_play_year.instance()&_first_play_month.instance()&_first_play_day.instance()&_first_play_hour.instance()&_first_play_minuits.instance()&_first_play_second.instance();
			ar&_defeated_enemy.instance()&_shooted_bullet_num.instance();
			ar&_game_over_count.instance()&_continue_count.instance()&_normal_clear_count.instance()&_all_clear_count.instance();
			ar&_cheat_count.instance()&_error_count.instance();
			_points.synchronize(); _hight_score.synchronize();
			_play_frame.synchronize(); _play_second.synchronize(); _play_count.synchronize();
			_first_play_year.synchronize(); _first_play_month.synchronize(); _first_play_day.synchronize(); _first_play_hour.synchronize(); _first_play_minuits.synchronize(); _first_play_second.synchronize();
			_defeated_enemy.synchronize(); _shooted_bullet_num.synchronize();
			_game_over_count.synchronize(); _continue_count.synchronize(); _normal_clear_count.synchronize(); _all_clear_count.synchronize();
			_cheat_count.synchronize(); _error_count.synchronize();
		}
	}
};

BOOST_CLASS_VERSION(GamePlayInformations, 1);