#include "UtilityFuncsForLua.h"
#include"DxLib.h"
#include"SoundManager.h"


void UtilityFuncsForLua::PrintToConsole(const char* str){
	printf(str);
}

void UtilityFuncsForLua::PrintToScreen(const char* str){
	printfDx(str);
}

void UtilityFuncsForLua::PrintToConsoleAndScreen(const char* str){
	PrintToConsole(str);
	PrintToScreen(str);
}

int UtilityFuncsForLua::PlayMusic(const std::string& name){
	int ret = SoundManager::GetInstance().PlayMusicByID(name);
	if (ret < 0){
		printf("UtilityFuncsForLua::PlayMusic ‰¹Šy‚ÌÄ¶‚ÉŽ¸”s‚µ‚Ü‚µ‚½B(%s,%d)\n", name.c_str(), ret);
	}
	return ret;
}
int UtilityFuncsForLua::PlayNoLoopMusic(const std::string& name){
	int ret = SoundManager::GetInstance().PlayMusicByID(name,false);
	if (ret < 0){
		printf("UtilityFuncsForLua::PlayNoLoopMusic ‰¹Šy‚ÌÄ¶‚ÉŽ¸”s‚µ‚Ü‚µ‚½B(%s,%d)\n", name.c_str(), ret);
	}
	return ret;
}
bool UtilityFuncsForLua::IsAssignedMusicPlaying(const std::string& m){
	return SoundManager::GetInstance().CheckAssignedMusicPlaying(m);
}
int UtilityFuncsForLua::StopMusic(){
	SoundManager::GetInstance().StopMusic();
	return 0;
}
bool UtilityFuncsForLua::IsMusicPlaying(){
	return SoundManager::GetInstance().CheckMusicPlaying();
}
int UtilityFuncsForLua::PlaySoundEffect(const std::string& name){
	int ret = SoundManager::GetInstance().PlaySoundByID(name);
	if (ret < 0){
		printf("UtilityFuncsForLua::PlaySoundEffect Œø‰Ê‚ÌÄ¶‚ÉŽ¸”s‚µ‚Ü‚µ‚½B(%s,%d)\n", name.c_str(), ret);
	}
	return ret;
}
int UtilityFuncsForLua::CreateSoundPlayHandle(const std::string& name){
	int ret = SoundManager::GetInstance().CreateSoundPlayHandle(name);
	if (ret < 0){
		printf("UtilityFuncsForLua::CreateSoundPlayHandle Œø‰Ê‰¹Ä¶ƒnƒ“ƒhƒ‹‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B(%s,%d)\n", name.c_str(), ret);
	}
	return ret;
}
int UtilityFuncsForLua::DeleteSoundPlayHandle(int sph){
	return SoundManager::GetInstance().DeleteSoundPlayHandle(sph);
}
int UtilityFuncsForLua::PlaySoundBySPHandle(int sph, bool rf){
	return SoundManager::GetInstance().PlaySoundByHandle(sph, rf);
}
int UtilityFuncsForLua::StopSoundBySPHandle(int sph){
	return SoundManager::GetInstance().StopSoundPlay(sph);
}
bool UtilityFuncsForLua::CheckSoundPlayingBySPHandle(int sph){
	return SoundManager::GetInstance().CheckSoundPlaying(sph);
}
