#pragma once

#include<string>
#include<unordered_map>
#include"DxLib.h"
#include<list>
#include"boost/bimap.hpp"

/*音楽読み込みモードについて
AllInInit:初期化時にすべて読み込み。
InPlay:再生時に読み込み。GetMusicHandleByIDを変更。
*/

class SoundManager
{
public:
	~SoundManager();
	SoundManager(const SoundManager&) = delete;
	static SoundManager& GetInstance();
	enum class LoadMode{ AllInInit, InPlay };
	void SetLoadMode(LoadMode); //初期化前に呼ぶ
	int Init();
	int End();
	int Reset();
	void Update();
	//効果音
	int PlaySoundByID(const std::string&); //IDでサウンド再生。(ないときは-1)(サウンド名,ループするか[,再生周波数])
	//効果音再生ハンドル
	int CreateSoundPlayHandle(const std::string&);//IDでサウンド再生ハンドル作成。(ないときは-1)(サウンド名)
	int DeleteSoundPlayHandle(int);
	int PlaySoundByHandle(int,bool,int=-1); //サウンド再生ハンドルからサウンド再生(再生ハンドル、ループするか[、再生周波数])
	int StopSoundPlay(int); //サウンド停止
	bool CheckSoundPlaying(int)const; //サウンド再生中か確認
	void ClearSoundPlayHandle();
	//音楽
	int PlayMusicByID(const std::string&,bool = true); //IDでミュージック再生開始。ループするか指定。(成功で0,失敗で-1)
	bool CheckMusicPlaying()const; //ミュージック再生中か確認
	bool CheckAssignedMusicPlaying(const std::string&)const; //ミュージック再生中か確認
	void StopMusic(); //音楽終了
	void FadeoutMusic(int); //ミュージックをフェードアウト(フレーム数)
private:
	int GetSoundHandle(const std::string&)const; //IDでサウンドハンドルを取得(ないときは-1)
	int GetMusicHandle(const std::string&); //IDでミュー軸ハンドルを取得(ないときは-1)

	enum LoadCategory{Sound,Music}; //読みむカテゴリ
	int LoadFile(LoadCategory); //ファイルから読み込み
	SoundManager();
	std::unordered_map<std::string, int> sound_handles; //サウンドハンドルリスト
	boost::bimap<int, int> _sound_play_handles;
	int _sound_play_handle_cnter;
	int _max_sound_play_num;
	void _check_sound_play_handle_num();

	struct MusicInfo{
		int handle;
		std::string file_name;
		int volume;
	};
	std::unordered_map<std::string, MusicInfo> music_handles; //ミュージックハンドルリスト
	int _playing_music; //再生中の音楽
	int _cur_msc_vol;
	std::string _playing_music_id;

	LoadMode _load_mode;
};

