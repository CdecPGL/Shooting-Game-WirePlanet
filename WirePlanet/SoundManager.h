#pragma once

#include<string>
#include<unordered_map>
#include"DxLib.h"
#include<list>
#include"boost/bimap.hpp"

/*���y�ǂݍ��݃��[�h�ɂ���
AllInInit:���������ɂ��ׂēǂݍ��݁B
InPlay:�Đ����ɓǂݍ��݁BGetMusicHandleByID��ύX�B
*/

class SoundManager
{
public:
	~SoundManager();
	SoundManager(const SoundManager&) = delete;
	static SoundManager& GetInstance();
	enum class LoadMode{ AllInInit, InPlay };
	void SetLoadMode(LoadMode); //�������O�ɌĂ�
	int Init();
	int End();
	int Reset();
	void Update();
	//���ʉ�
	int PlaySoundByID(const std::string&); //ID�ŃT�E���h�Đ��B(�Ȃ��Ƃ���-1)(�T�E���h��,���[�v���邩[,�Đ����g��])
	//���ʉ��Đ��n���h��
	int CreateSoundPlayHandle(const std::string&);//ID�ŃT�E���h�Đ��n���h���쐬�B(�Ȃ��Ƃ���-1)(�T�E���h��)
	int DeleteSoundPlayHandle(int);
	int PlaySoundByHandle(int,bool,int=-1); //�T�E���h�Đ��n���h������T�E���h�Đ�(�Đ��n���h���A���[�v���邩[�A�Đ����g��])
	int StopSoundPlay(int); //�T�E���h��~
	bool CheckSoundPlaying(int)const; //�T�E���h�Đ������m�F
	void ClearSoundPlayHandle();
	//���y
	int PlayMusicByID(const std::string&,bool = true); //ID�Ń~���[�W�b�N�Đ��J�n�B���[�v���邩�w��B(������0,���s��-1)
	bool CheckMusicPlaying()const; //�~���[�W�b�N�Đ������m�F
	bool CheckAssignedMusicPlaying(const std::string&)const; //�~���[�W�b�N�Đ������m�F
	void StopMusic(); //���y�I��
	void FadeoutMusic(int); //�~���[�W�b�N���t�F�[�h�A�E�g(�t���[����)
private:
	int GetSoundHandle(const std::string&)const; //ID�ŃT�E���h�n���h�����擾(�Ȃ��Ƃ���-1)
	int GetMusicHandle(const std::string&); //ID�Ń~���[���n���h�����擾(�Ȃ��Ƃ���-1)

	enum LoadCategory{Sound,Music}; //�ǂ݂ރJ�e�S��
	int LoadFile(LoadCategory); //�t�@�C������ǂݍ���
	SoundManager();
	std::unordered_map<std::string, int> sound_handles; //�T�E���h�n���h�����X�g
	boost::bimap<int, int> _sound_play_handles;
	int _sound_play_handle_cnter;
	int _max_sound_play_num;
	void _check_sound_play_handle_num();

	struct MusicInfo{
		int handle;
		std::string file_name;
		int volume;
	};
	std::unordered_map<std::string, MusicInfo> music_handles; //�~���[�W�b�N�n���h�����X�g
	int _playing_music; //�Đ����̉��y
	int _cur_msc_vol;
	std::string _playing_music_id;

	LoadMode _load_mode;
};

