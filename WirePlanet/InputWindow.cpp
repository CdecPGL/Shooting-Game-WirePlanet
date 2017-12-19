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
	if (CheckKeyInput(kih) != 0){ //�L�[���͂�����������
		//		sdm.Play("MenuDecideSE");
		char inpt[MAX_INPUT_LENGTH];
		GetKeyInputString(inpt, kih);
		string command = "\\\\SetGlobalValue=InputText&";
		command += inpt;
		ProcessCommand(command); //���ʂ��O���[�o���ϐ��ɕۑ�
		DeleteRequestMWM();
	}
	//	ResistDrawList(0);
}

void InputWindow::Draw()
{
	DrawWindow(); //�E�C���h�E
	//������
	int idx(0);
	for (string str : *strings){
		DrawFormatStringToHandle(position.x + 5, position.y + 5 + idx*word_size, _string_col_handle, _font_handle, "%s", str.c_str());
		++idx;
	}
	DrawBox(position.x + 5, position.y + 5 + idx*word_size, position.x + 5 + word_size / 2 * MAX_INPUT_LENGTH, position.y + 5 + (idx + 1)*word_size, _string_col_handle, false);
	//���͒��̕�����
	DrawKeyInputString(position.x + 5, position.y + 5 + idx*word_size, kih);
}

int InputWindow::Init(std::shared_ptr<const std::vector<std::string>> strdat, Vector2D<int> pos, Vector2D<int> s)
{
	MessageWindow::Init(strdat, pos, s);
	//�L�[���̓n���h��
	kih = MakeKeyInput(MAX_INPUT_LENGTH, false, false, false);
	if (SetActiveKeyInput(kih) != 0){ return -1; }
	//�L�[���͑҂��I�t
	_waiting_key_push = false;
	return 0;
}

int InputWindow::End(){
	MessageWindow::End();
	DeleteKeyInput(kih);
	return 0;
}