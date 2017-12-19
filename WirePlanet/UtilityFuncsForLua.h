#pragma once

#include<string>

namespace UtilityFuncsForLua
{
	void PrintToConsole(const char*);
	void PrintToScreen(const char*);
	void PrintToConsoleAndScreen(const char*);

	//‰¹ŠÖ˜A
	int PlayMusic(const std::string&);
	int PlayNoLoopMusic(const std::string&);
	int StopMusic();
	bool IsMusicPlaying();
	bool IsAssignedMusicPlaying(const std::string&);
	int PlaySoundEffect(const std::string&);
	int CreateSoundPlayHandle(const std::string&);
	int DeleteSoundPlayHandle(int);
	int PlaySoundBySPHandle(int,bool);
	int StopSoundBySPHandle(int);
	bool CheckSoundPlayingBySPHandle(int);
};

