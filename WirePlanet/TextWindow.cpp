#include "TextWindow.h"
#include"MessageWindowUtilityClass.h"
#include"SoundManager.h"

//メッセージ
//ProgressMessage:メッセージを進める
//SetSelectedIndex:(選択ウインドウで)選択されたインデックスセット

using namespace std;

TextWindow::TextWindow(int id, int pid) :MessageWindow(id, pid), _popopo_sph(-1)
{
}


TextWindow::~TextWindow()
{
	SoundManager::GetInstance().DeleteSoundPlayHandle(_popopo_sph);
}

void TextWindow::Update()
{
	if (!_waiting_key_push && _wait_frame<=0){
		if (_raw < 0 && _line>=0){
			//最終行を超えてたらウインドウを閉じる
			if (_line >= (int)strings->size()){ DeleteRequestMWM(); }
			//命令かラベルだったら次の行に進める
			else if (ProcessCommand((*strings)[_line]) >= 0 || IsLabel((*strings)[_line])){
				++_line;
				_raw = -1;
			}
			else{
				_raw = 0;
			}
		}
		else if (_word_draw_cntr >= _word_draw_frame){
			_word_draw_cntr = 0;
			//行が読み込まれてないか現在行の幅を現在列が超えていたら次の行へ
			if (_line < 0 || (int)(*strings)[_line].size() <= _raw){
				++_line;
				_raw = -1;
			}
			else{
				if (_raw == 0){ PushBackDrawStrings(string()); }
				_draw_strings.back().push_back((*strings)[_line][_raw]);
				//マルチバイト文字だったらもう一バイト追加して列もインクリメントする。
				if (SJISMultiCheck((*strings)[_line][_raw])){
					++_raw;
					_draw_strings.back().push_back((*strings)[_line][_raw]);
				}
				SoundManager::GetInstance().PlaySoundByHandle(_popopo_sph,false,15000);
				++_raw;
			}
		}
	}
	UpdateCounter();
}

void TextWindow::Draw()
{
	//ウインドウ表示
	DrawWindow();
	//文字表示
	int i(0);
	for (string str : _draw_strings){
		DrawFormatStringToHandle(position.x + 5+72+16, position.y + 5 + i*word_size, _string_col_handle,_font_handle, "%s", str.c_str());
		++i;
	}
	//話者表示
	if (_current_speaker){
		DrawFormatStringToHandle(position.x + 5, position.y +78, _string_col_handle, _name_font_handle, "%s", _current_speaker->name.c_str());
	}
	//顔画像表示
	DrawFaceGraphic(Vector2D<int>(5, 5), Vector2D<int>(72, 72));
	//キー入力待機表示
	if (_waiting_key_push){
		int dy = _inc_cntr % 60 < 30 ? 0 : -1;
		DrawFormatStringToHandle(position.x + size.x - word_size - 5, position.y + size.y - word_size - 5 + dy, _string_col_handle, _font_handle, "▼");
	}
}

int TextWindow::Init(std::shared_ptr<const std::vector<std::string>> strdat, Vector2D<int> pos, Vector2D<int> s)
{
	MessageWindow::Init(strdat, pos, s);
	//キー入力待ちオフ
	_waiting_key_push = false;

	_popopo_sph = SoundManager::GetInstance().CreateSoundPlayHandle("TalkSound");
	return 0;
}

int TextWindow::RecieveMessage(const std::string& mess, int param)
{
	if (MessageWindow::RecieveMessage(mess,param) == 0){ return 0; } //親クラスのメッセージ処理
	if (mess == "ProgressLine")
	{
//		ProgressLine(param);
		return 0;
	}
	else{
		return -1;
	}
}

void TextWindow::SelectKeyPushed()
{
//	sdm.Play("MenuSelectSE");
//	ProgressLine(1);
	_waiting_key_push = false;
}
