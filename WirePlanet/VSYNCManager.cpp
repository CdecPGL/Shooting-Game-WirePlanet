#include "VSYNCManager.h"

namespace{
	const unsigned int REFRESH_RATE(60);
}

VSYNCManager::VSYNCManager() : _mode(Mode::VSYNC), _ref_cnt(0), _pre_cycle_time(0)
{
}


VSYNCManager::~VSYNCManager()
{
}

VSYNCManager& VSYNCManager::GetInstance(){
	static VSYNCManager vm;
	return vm;
}

int VSYNCManager::Init(){
	//リフレッシュレートの計測
	int RefreshRate;
	HDC hdc;
	hdc = GetDC(GetMainWindowHandle());	// デバイスコンテキストの取得
	RefreshRate = GetDeviceCaps(hdc, VREFRESH);	// リフレッシュレートの取得
	ReleaseDC(GetMainWindowHandle(), hdc);	// デバイスコンテキストの解放
	printf("リフレッシュレート:%dHz\n", RefreshRate);
	//リフレッシュレートによって動作を変える
	if (RefreshRate != REFRESH_RATE){ //指定と違ったら垂直同期を切り、タイマーで更新感覚を管理する
		SetWaitVSyncFlag(false);
		_mode = Mode::TIMER;
		return 1;
	}
	else{
		_mode = Mode::VSYNC;
		return 0;
	}
}

int VSYNCManager::ScreenFlip(){
	if (_mode == Mode::TIMER){
		++_ref_cnt;
		int _cur_time = GetNowCount();
		int _next_ref_time = _pre_cycle_time + (int)(1000.0 / REFRESH_RATE * _ref_cnt);
		if (_cur_time < _next_ref_time){
			WaitTimer(_next_ref_time - _cur_time);
		}
		if (_ref_cnt == REFRESH_RATE){
			_pre_cycle_time = GetNowCount();
			_ref_cnt = 0;
		}
	}
	return DxLib::ScreenFlip();
}
