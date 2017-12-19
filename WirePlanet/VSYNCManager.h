#pragma once
#include"DxLib.h"
class VSYNCManager
{
public:
	VSYNCManager(const VSYNCManager&) = delete;
	~VSYNCManager();
	const VSYNCManager& operator = (const VSYNCManager&) = delete;
	static VSYNCManager& GetInstance();
	int Init(); //‰Šú‰»(-1:¸”s 0:¬Œ÷(‚’¼“¯Šú—LŒø) 1:¬Œ÷(‚’¼“¯Šú–³Œø))
	int ScreenFlip();
private:
	VSYNCManager();
	int _pre_cycle_time;
	int _ref_cnt;
	enum class Mode{ VSYNC, TIMER };
	Mode _mode;
};

