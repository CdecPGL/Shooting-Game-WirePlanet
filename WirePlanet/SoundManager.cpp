#include "SoundManager.h"
#include"SVLoader.h"
#include"boost/lexical_cast.hpp"
#include"FileLoadUtility.h"

using namespace std;

namespace{
	const std::string SOUND_LIST("sound_list.csv");
	const std::string MUSIC_LIST("music_list.csv");
	const int DEFAULT_MAX_SOUND_PLAY_NUM(10);
}

SoundManager::SoundManager() :_playing_music(-1), _sound_play_handle_cnter(0), _playing_music_id("NULL"), _max_sound_play_num(DEFAULT_MAX_SOUND_PLAY_NUM), _load_mode(LoadMode::AllInInit)
{
	printf("SoundManagerのインスタンスが作成されました。\n");
}


SoundManager::~SoundManager()
{
	printf("SoundManagerのインスタンスが破棄されました。\n");
}

SoundManager& SoundManager::GetInstance(){
	static SoundManager sm;
	return sm;
}

int SoundManager::Init(){
	int error = 0;
	if (LoadFile(Sound)){
		printf("SoundManagerの初期化に失敗しました。サウンドの読み込みに失敗しました。\n");
		error = -1;
	}
	if (LoadFile(Music)){
		printf("SoundManagerの初期化に失敗しました。音楽の読み込みに失敗しました。\n");
		error = -2;
	}
	if(error){
		printf("SoundManagerが初期化されました。\n");
		return error;
	}
	printf("SoundManagerが初期化されました。\n");
	return 0;
}

int SoundManager::End(){
	ClearSoundPlayHandle();
	this->StopMusic();
	printf("SoundManagerの終了処理が実行されました。\n");
	return 0;
}

int SoundManager::Reset(){
	ClearSoundPlayHandle();
	StopMusic();
	return 0;
}

int SoundManager::GetSoundHandle(const std::string& id)const{
	auto it = sound_handles.find(id);
	if (it == sound_handles.end()){ return -1; }
	return (*it).second;
}

int SoundManager::GetMusicHandle(const std::string& id){
	auto it = music_handles.find(id);
	if (it == music_handles.end()){
		return -1; 
		printf("SoundManager::GetMusicHandle 指定された音楽は存在しません。(%s)\n", id.c_str());
	}
	int mh = (*it).second.handle;
	if (_load_mode == LoadMode::InPlay){
		DeleteSoundMem(_playing_music);
		auto it2 = music_handles.find(_playing_music_id);
		if (it2 != music_handles.end()){ (*it2).second.handle = -1; }
	}
	if (mh<0){
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
		mh = FileLoadUtil::CreateSoundMemFromFLM((*it).second.file_name, true);
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		if (mh < 0){
			printf("SoundManager::GetMusicHandle 指定された音楽を読み込めませんでした。(%s)\n", (*it).second.file_name.c_str());
		}
		ChangeVolumeSoundMem((*it).second.volume, mh);
		(*it).second.handle = mh;
		printf("SoundManager::GetMusicHandle 音楽を読み込みました。(%s)\n", (*it).second.file_name.c_str());
	}
	return mh;
}

int SoundManager::PlaySoundByID(const string& id){
	int sh = GetSoundHandle(id);
	if (sh == -1){ return -1; }
	PlaySoundMem(sh, DX_PLAYTYPE_BACK,true);
	return 0;
}

int SoundManager::CreateSoundPlayHandle(const string& id){
	auto it = sound_handles.find(id);
	if (it == sound_handles.end()){ return -1; }
	int nsh = DuplicateSoundMem(it->second);
	if (nsh < 0){ return -2; }
	int vol = GetVolumeSoundMem(it->second);
	SetVolumeSoundMem(vol,nsh);

	//サウンドプレイハンドルの最大数を超えないかチェックし、超えたら古いものを消去(サウンドプレイハンドルが最小のものを古いとみなす)
	if (_sound_play_handles.size() >= (unsigned)_max_sound_play_num){
		auto it = min_element(_sound_play_handles.right.begin(), _sound_play_handles.right.end());
		DeleteSoundMem(it->first);
		_sound_play_handles.right.erase(it);
	}

	_sound_play_handles.insert(boost::bimap<int, int>::value_type(_sound_play_handle_cnter, nsh));
	return _sound_play_handle_cnter++;
}

int SoundManager::DeleteSoundPlayHandle(int sph){
	auto it = _sound_play_handles.left.find(sph);
	if (it == _sound_play_handles.left.end()){ return -1; }
	StopSoundMem(it->second);
	DeleteSoundMem(it->second);
	_sound_play_handles.left.erase(it);
	return 0;
}

int SoundManager::PlaySoundByHandle(int sph, bool rp,int frq){
	auto it = _sound_play_handles.left.find(sph);
	if (it == _sound_play_handles.left.end()){ return -1; }
	SetFrequencySoundMem(frq, it->second);
	PlaySoundMem(it->second, rp ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, true);
	return 0;
}

int SoundManager::StopSoundPlay(int sph){
	auto it = _sound_play_handles.left.find(sph);
	if (it == _sound_play_handles.left.end()){ return -1; }
	StopSoundMem(it->second);
	return 0;
}

bool SoundManager::CheckSoundPlaying(int sph)const{
	auto it = _sound_play_handles.left.find(sph);
	if (it == _sound_play_handles.left.end()){ return false; }
	if (CheckSoundMem(it->second)){ return true; }
	else{ return false; }
}

void SoundManager::ClearSoundPlayHandle(){
	for (auto& it : _sound_play_handles){
		StopSoundMem(it.right);
		DeleteSoundMem(it.right);
	}
	_sound_play_handles.clear();
}

int SoundManager::PlayMusicByID(const string& id,bool rf){
	if (_playing_music >= 0){ StopSoundMem(_playing_music); }
	int mh = GetMusicHandle(id);	
	if (mh == -1){ return -1; }
	_playing_music = mh;
	PlaySoundMem(mh, rf ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, true);
	_playing_music_id = id;
	return 0;
}

void SoundManager::StopMusic(){
	if (_playing_music >= 0){
		StopSoundMem(_playing_music);
		if (_load_mode == LoadMode::InPlay){
			DeleteSoundMem(_playing_music);
			auto it = music_handles.find(_playing_music_id);
			if (it != music_handles.end()){ (*it).second.handle = -1; }
		}
		_playing_music = -1;
		_playing_music_id = "NULL";
	}
}

bool SoundManager::CheckMusicPlaying()const{
	return _playing_music >= 0;
}

bool SoundManager::CheckAssignedMusicPlaying(const std::string& m)const{
	return _playing_music_id != m ? false : true;
}

void SoundManager::SetLoadMode(LoadMode lm){
	_load_mode = lm;
	switch (_load_mode)
	{
	case SoundManager::LoadMode::AllInInit:
		printf("SoundManager:音楽を初期化時にまとめて読み込みます。\n");
		break;
	case SoundManager::LoadMode::InPlay:
		printf("SoundManager:音楽を再生時に逐次読み込みます\n");
		break;
	default:
		break;
	}
	_load_mode = lm;
}

int SoundManager::LoadFile(LoadCategory lc){
	try{
		//リストを取得、読み込み
		string list_pass;
		CSVLoader scvl;
		switch (lc){
		case Sound:
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
			list_pass = SOUND_LIST;
			break;
		case Music:
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
			list_pass = MUSIC_LIST;
			break;
		}
		int res;
		if ((res = FileLoadUtil::LoadCSVFromFLM(&scvl,list_pass,true))){
			printf("リストを読み込めませんでした。(reason:%d,name:%s)\n",res, list_pass.c_str());
			return -1;
		}
		for (vector<string>& s : scvl){
			if (s.size() != 3){
				printf("リストの項目数が不正です。(%d/3)\n",s.size());
				continue; 
			}
			int sh;
			switch (lc){
			case Sound:
				sh = FileLoadUtil::CreateSoundMemFromFLM(s[1], true);
				if (sh < 0){
					printf("指定されたサウンドファイルが読み込めませんでした。(reason:%d ID:%s pass:%s)\n", sh, s[0].c_str(), (/*dir + */s[1]).c_str());
					continue;
				}
				break;
			case Music:
				sh = _load_mode==LoadMode::AllInInit ? FileLoadUtil::CreateSoundMemFromFLM(s[1], true) : -1;
				break;
			default:
				break;
			}
			//ボリューム設定
			int vol;
			try{
				vol = boost::lexical_cast<int>(s[2]);
			}
			catch (boost::bad_lexical_cast&){
				printf("ボリュームが不正です。(V:%s ID:%s pass:%s)\n", s[2].c_str(), s[0].c_str(), (/*dir + */s[1]).c_str());
			}
			if(lc==Sound ||  _load_mode==LoadMode::AllInInit)ChangeVolumeSoundMem(vol, sh);
			switch (lc){
			case Sound:
				sound_handles.insert(unordered_map<string, int>::value_type(s[0], sh));
				break;
			case Music:
				music_handles.insert(unordered_map<string, MusicInfo>::value_type(s[0], MusicInfo({sh,s[1],vol})));
				break;
			default:
				break;
			}
		}
		return 0;
	}
	catch (out_of_range& e){
		printf("初期データが不足しています。(%s)\n", e.what());
		return -1;
	}
}
