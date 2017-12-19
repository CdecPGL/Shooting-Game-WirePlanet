#pragma once
#include"DxLib.h"
class VSYNCManager
{
public:
	VSYNCManager(const VSYNCManager&) = delete;
	~VSYNCManager();
	const VSYNCManager& operator = (const VSYNCManager&) = delete;
	static VSYNCManager& GetInstance();
	int Init(); //������(-1:���s 0:����(���������L��) 1:����(������������))
	int ScreenFlip();
private:
	VSYNCManager();
	int _pre_cycle_time;
	int _ref_cnt;
	enum class Mode{ VSYNC, TIMER };
	Mode _mode;
};

