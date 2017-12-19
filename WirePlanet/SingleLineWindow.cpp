#include "SingleLineWindow.h"
#include"GraphicManager.h"

const int DEFAULT_HEIGHT(26);
const int DEFAULT_DRAW_FRAME(120);

SingleLineWindow::SingleLineWindow(int id, int pid) :MessageWindow(id, pid), inc_cnt(0), draw_frame(DEFAULT_DRAW_FRAME), _black_stream_gh(-1), _kind(Kind::Normal)
{
}

SingleLineWindow::~SingleLineWindow()
{
}

void SingleLineWindow::Update(){
	if (inc_cnt >= draw_frame){ DeleteRequestMWM(); }
	++inc_cnt;
}

void SingleLineWindow::Draw(){
	int string_pos_x = 0;
	switch (_kind)
	{
	case SingleLineWindow::Kind::Normal:
		DrawWindow();
		DrawFormatStringToHandle(position.x + 5, position.y + 5, _string_col_handle, _font_handle, "%s", text.c_str());
		break;
	case SingleLineWindow::Kind::WhiteStream:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
		DrawRotaGraph3(position.x + size.x / 2, position.y + size.y / 2, 32, 32, (double)size.y / 64.0 * 2, (double)size.x / 64.0, DX_PI / 2, _black_stream_gh, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		string_pos_x = position.x + (size.x - GetDrawStringWidthToHandle(text.c_str(), text.size(), _font_handle)) / 2;
		DrawStringToHandle(string_pos_x, position.y + 5, text.c_str(), GetColor(0, 0, 0), _font_handle);
		break;
	case SingleLineWindow::Kind::BlackStream:
		SetDrawBlendMode(DX_BLENDMODE_SUB, 150);
		DrawRotaGraph3(position.x + size.x / 2, position.y + size.y / 2, 32, 32, (double)size.y / 64.0 * 2, (double)size.x / 64.0, DX_PI / 2, _black_stream_gh, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		string_pos_x = position.x + (size.x - GetDrawStringWidthToHandle(text.c_str(), text.size(), _font_handle)) / 2;
		DrawStringToHandle(string_pos_x, position.y + 5, text.c_str(), GetColor(255, 255, 255), _font_handle);
		break;
	default:
		break;
	}
}
int SingleLineWindow::Init(std::shared_ptr<const std::vector<std::string>> strdat, Vector2D<int> pos, Vector2D<int> s){
	s.y = DEFAULT_HEIGHT;
	MessageWindow::Init(strdat, pos, s);
	//コマンド以外で一番初めの行をテキストとする
	int i = 0;
	while ((int)strdat->size() > i && CheckCommand((*strdat)[i]) == true){
		++i;
	}
	if ((int)strdat->size() > i){ text = (*strdat)[i]; }
	/*_black_stream_gh = MakeScreen(64, 64, true);
	GraphFilter(GraphicManager::GetInstance().GetGraphHandle("Laser64"),_black_stream_gh, DX_GRAPH_FILTER_INVERT);*/
	_black_stream_gh = GraphicManager::GetInstance().GetGraphHandle("Laser64");
	return 0;
}

int SingleLineWindow::End(){
	//	DeleteGraph(_black_stream_gh);
	MessageWindow::End();
	return 0;
}

void SingleLineWindow::SetDrawFrame(int df){ if (df > 1){ draw_frame = df; } }
void SingleLineWindow::SetKind(Kind k){ _kind = k; }