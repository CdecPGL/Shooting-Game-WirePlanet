#include "DebugWindow.h"
#include"Key.h"
#include "CharacterManager.h"
#include"SceneManager.h"
#include<typeinfo>
#include"EnemyGenerator.h"
#include"EnemyMoverManager.h"
#include"EffectManager.h"
#include"SceneManager.h"
#include"EventManager.h"
#include"FileLoadManager.h"
#include"InitFuncs.h"

DebugWindow::DebugInfo::DebugInfo(int l, string lo, string i, double n) :level(l), location(lo), info(i), num(n){

}

DebugWindow::DebugData::DebugData(Vector2D<int> pos, string dat, int col) : position(pos), data(dat), color(col){

}

DebugWindow::DebugWindow() : max_buffer(100), max_line(30), draw_time(300), size(16), di_fh(CreateFontToHandle(NULL, size, 1, DX_FONTTYPE_EDGE, -1, 1)), counter(draw_time),_draw_info(DrawInfo::None), cntr(Counter::GetInstance()), debug_data_visible(false), dd_ak_fh(CreateFontToHandle(NULL, -1, -1, DX_FONTTYPE_EDGE))
{
}


DebugWindow::~DebugWindow()
{
	DeleteFontToHandle(di_fh);
	DeleteFontToHandle(dd_ak_fh);
}

DebugWindow& DebugWindow::GetInstance(){
	static DebugWindow dw;
	return dw;
}

void DebugWindow::Update(){
	Key& key = Key::GetInstance();
	if (key.GetCustomInputState(CUSTOM_INPUT_L) && key.GetCustomInputState(CUSTOM_INPUT_R)){
		if (key.GetCustomInputPush(CUSTOM_INPUT_A)){ debug_data_visible = !debug_data_visible; }
		if (key.GetCustomInputPush(CUSTOM_INPUT_B)){ ChangeDrawInfo(); }
	}
	if (key.GetKeyStatus(KEY_INPUT_F1)){
		DrawStringToHandle(0,0,"F1:デバッグリファレンス表示 F2 or L+A:デバッグデータ表示切り替え",GetColor(255,255,255),dd_ak_fh);
		DrawStringToHandle(0, 16, "F6 or L+B:ゲーム情報表示切り替え F7:不要データの削除", GetColor(255, 255, 255), dd_ak_fh);
		DrawStringToHandle(0, 32, "F8:FLMの更新 F9:スクリプトファイル再読み込み F10:敵生成データの再読み込み", GetColor(255, 255, 255), dd_ak_fh);
		DrawStringToHandle(0, 32, "F11 定義データをまとめて再読み込み", GetColor(255, 255, 255),dd_ak_fh);
	}
	if (key.GetKeyPush(KEY_INPUT_F2)){ debug_data_visible = !debug_data_visible; }
	if (key.GetKeyPush(KEY_INPUT_F6)){ ChangeDrawInfo(); }
	if (key.GetKeyPush(KEY_INPUT_F7)){ 
		InitFuncs::CleanManager(); 
		printf("マネージャの不要なデータを削除しました。\n");
	}
	if (key.GetKeyPush(KEY_INPUT_F8)){
//		SceneManager::GetInstance().CreateAndChangeScene("Title");
		if (cdec::FileLoadManager::GetInstance().ReloadAllFIle()==0){
			printf("FLMの更新に成功しました。\n");
		}
	}
	if (key.GetKeyPush(KEY_INPUT_F9)){
		SceneManager::GetInstance().CreateAndChangeScene("Title");
		if (InitFuncs::ReloadScriptFile() == 0){
			printf("スクリプトファイルの再読み込みに成功しました。\n");
		}
	}
	if (key.GetKeyPush(KEY_INPUT_F10)){
		SceneManager::GetInstance().CreateAndChangeScene("Title");
		if (EnemyGenerator::GetInstance().Reload() == 0){
			printf("敵生成データの再読み込みに成功しました。\n");
		}
	}
	if (key.GetKeyPush(KEY_INPUT_F11)){
		SceneManager::GetInstance().CreateAndChangeScene("Title");
		if (cdec::FileLoadManager::GetInstance().ReloadAllFIle() == 0 && InitFuncs::ReloadScriptFile() == 0 && EnemyGenerator::GetInstance().Reload() == 0){
			printf("定義データの再読み込みに成功しました。\n");
		}
	}
}

void DebugWindow::Draw(){
	//DebugData
	if (debug_data_visible){
		for (list<DebugData>::iterator it = dd.begin(); it != dd.end(); ++it){
			DrawStringToHandle((*it).position.x, (*it).position.y, (*it).data.c_str(), (*it).color,dd_ak_fh);
		}
		dd.clear();
	}

	//DebugInfo
	if (Key::GetInstance().GetKeyStatus(KEY_INPUT_F1) == 1){
		counter = 0;
	}
	//if (counter >= draw_time){ return; }
	int line = di.size();
	if (line > max_line){ line = max_line; }
	list<DebugInfo>::iterator it = di.begin();
	int alpha = 255;
	if (counter >= draw_time){
		alpha = 255 * (60 - (counter - draw_time + 1)) / 60;
	}
	if (alpha >= 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		for (int i = 0; i < line; ++i){
			int color = GetColorByLevel((*it).level);
			DrawFormatStringToHandle(0, i*size, color, di_fh, "%s:%s(%lf)", (*it).location.c_str(), (*it).info.c_str(), (*it).num);
			++it;
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//各種変数表示
	switch (_draw_info){
	case DrawInfo::MngrInnfo:
		DrawManagerInfo();
		break;
	case DrawInfo::PfmcInfo:
		DrawPerformanceInfo();
		break;
	default:
		break;
	}
	//現在のシーン表示
	auto cur_sne_ptr = SceneManager::GetInstance().GetCurrentScene().get();
	if (cur_sne_ptr != nullptr){ SetFormatDebugData(0, 464, GetColor(255, 255, 255), "%s", typeid(*cur_sne_ptr).name()); }
	//カウンタ内容表示
	SetFormatDebugData(508, 0, GetColor(255, 255, 255), "frame:%d", cntr.GetCount());
	SetFormatDebugData(508, 16, GetColor(255, 255, 255), "FPS:%d", cntr.GetFPS());
	SetFormatDebugData(508, 32, GetColor(255, 255, 255), ">%lf%%", cntr.GetProcessRateHiprecision());
	++counter;
}

void DebugWindow::SetDebugInfo(int l,string lo,string i,double n){
	DebugInfo ndi(l, lo, i, n);
	di.push_front(ndi);
	if ((int)di.size() > max_buffer){
		di.pop_back();
	}
	counter = 0;
}

void DebugWindow::SetDebugData(Vector2D<int> pos, string dat, int col){
	if (!debug_data_visible){ return; }
	DebugData ndd(pos,dat,col);
	dd.push_back(ndd);
}

void DebugWindow::AddExecutionTime(const string& n, int t){
	if (_draw_info != DrawInfo::PfmcInfo){ return; }
	_execution_times.insert(make_pair(n,t));
}

void DebugWindow::ChangeMaxBuffer(int num){
	max_buffer = num;
	if ((int)di.size() > max_buffer){
		while ((int)di.size() <= max_buffer){
			di.pop_back();
		}
	}
}

void DebugWindow::ChangeMaxLine(int num){
	max_line = num;
}

void DebugWindow::ChangeDrawTime(int num){
	draw_time = num;
}

void DebugWindow::ChangeFontSize(int num){
	size = num;
	DeleteFontToHandle(di_fh);
	di_fh = CreateFontToHandle(NULL, size, 1, DX_FONTTYPE_EDGE,-1,1);
}

//重要度職を取得
int DebugWindow::GetColorByLevel(int l){
	switch (l)
	{
	case 1:
		return GetColor(255, 255, 255);
	case 2:
		return GetColor(255, 255, 0);
	case 3:
		return GetColor(255, 0, 0);
	default:
		return GetColor(255, 255, 255);
		break;
	}
}

void DebugWindow::ChangeDrawInfo(){
	switch (_draw_info)
	{
	case DrawInfo::None:
		_draw_info = DrawInfo::MngrInnfo;
		break;
	case DrawInfo::MngrInnfo:
		_draw_info = DrawInfo::PfmcInfo;
		break;
	case DrawInfo::PfmcInfo:
		_draw_info = DrawInfo::None;
		break;
	default:
		_draw_info = DrawInfo::None;
		break;
	}
}

void DebugWindow::DrawPerformanceInfo(){
	DrawStringToHandle(0, 0, "===PerformanceInfomation===", GetColor(255, 255, 255), dd_ak_fh);
	int cntr = 1;
	for (auto& it : _execution_times){
		DrawFormatStringToHandle(0, 16 * cntr, GetColor(255, 255, 255), dd_ak_fh, "%s:%dms", it.first.c_str(), it.second);
		++cntr;
	}
	_execution_times.clear();
}

void DebugWindow::DrawManagerInfo(){
	int i = 0;
	DrawStringToHandle(0, 16 * i++, "===ManagerInfomation===", GetColor(255, 255, 255), dd_ak_fh);
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---SceneManager---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "SceneCount:%d", SceneManager::GetInstance().GetObjectCount());
	CharacterManager& cm = CharacterManager::GetInstance();
	int chara_count = cm.GetObjectCount();
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---CharacterManager---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "CharacterCount:%d,ColDetCnt:%d", chara_count, cm.GetCollisionDetectedCount());
	EnemyGenerator& eg = EnemyGenerator::GetInstance();
	int eg_cgnum = eg.GetCurrectGenNumber();
	int eg_cntr = eg.GetCounterValue();
	int eg_gq = eg.GetCurrentGenQueueCount();
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---EnemyMoverManager---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "cnt:%d,using_memory:%dKB", EnemyMoverManager::GetInstance().GetCount(), EnemyMoverManager::GetInstance().GetUsingMemory());
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---EnemyGenerator---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "cgnum:%d cntr:%d cgen_q:%d", eg_cgnum, eg_cntr, eg_gq);
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---EffectManager---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "EffectCount:%d", EffectManager::GetInstance().GetObjectCount());
	EventManager& evm = EventManager::GetInstance();
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---EventManager---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "EventCount:%d TriggerCount:%d using_memory:%dKB", evm.GetEventCount(), evm.GetTriggerCount(), evm.GetUsingMemory());
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---FileLoadManager---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "UsingMemory:%dKB", cdec::FileLoadManager::GetInstance().GetUsingMemory() / 1024);
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "---DxLib---");
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "DxGetAllocSize:%lfMB", (double)DxGetAllocSize()/1024/1024);
	DrawFormatStringToHandle(0, 16 * i++, GetColor(255, 255, 255), dd_ak_fh, "GetGraphNum:%d", GetGraphNum());
}
