#include "InputWindow.h"

using namespace std;

InputWindow::InputWindow(int id, int pid) :MessageWindow(id, pid), kih(-1)
{
}

InputWindow::~InputWindow()
{
}

void InputWindow::Update()
{
	if (CheckKeyInput(kih) != 0){ //キー入力が完了したら
		//		sdm.Play("MenuDecideSE");
		char inpt[MAX_INPUT_LENGTH];
		GetKeyInputString(inpt, kih);
		string command = "\\\\SetGlobalValue=InputText&";
		command += inpt;
		ProcessCommand(command); //結果をグローバル変数に保存
		DeleteRequestMWM();
	}
	//	ResistDrawList(0);
}

void InputWindow::Draw()
{
	DrawWindow(); //ウインドウ
	//文字列
	int idx(0);
	for (string str : *strings){
		DrawFormatStringToHandle(position.x + 5, position.y + 5 + idx*word_size, _string_col_handle, _font_handle, "%s", str.c_str());
		++idx;
	}
	DrawBox(position.x + 5, position.y + 5 + idx*word_size, position.x + 5 + word_size / 2 * MAX_INPUT_LENGTH, position.y + 5 + (idx + 1)*word_size, _string_col_handle, false);
	//入力中の文字列
	DrawKeyInputString(position.x + 5, position.y + 5 + idx*word_size, kih);
}

int InputWindow::Init(std::shared_ptr<const std::vector<std::string>> strdat, Vector2D<int> pos, Vector2D<int> s)
{
	MessageWindow::Init(strdat, pos, s);
	//キー入力ハンドル
	kih = MakeKeyInput(MAX_INPUT_LENGTH, false, false, false);
	if (SetActiveKeyInput(kih) != 0){ return -1; }
	//キー入力待ちオフ
	_waiting_key_push = false;
	return 0;
}

int InputWindow::End(){
	MessageWindow::End();
	DeleteKeyInput(kih);
	return 0;
}