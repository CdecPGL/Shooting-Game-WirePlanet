#pragma once
#include"DxLib.h"
class VSYNCManager
{
public:
	VSYNCManager(const VSYNCManager&) = delete;
	~VSYNCManager();
	const VSYNCManager& operator = (const VSYNCManager&) = delete;
	static VSYNCManager& GetInstance();
	int Init(); //初期化(-1:失敗 0:成功(垂直同期有効) 1:成功(垂直同期無効))
	int ScreenFlip();
private:
	VSYNCManager();
	int _pre_cycle_time;
	int _ref_cnt;
	enum class Mode{ VSYNC, TIMER };
	Mode _mode;
};

