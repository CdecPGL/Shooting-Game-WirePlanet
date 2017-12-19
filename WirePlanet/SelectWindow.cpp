#include "SelectWindow.h"
#include"MassageWindowManager.h"

using namespace std;
SelectWindow::SelectWindow(int id, int pid) :MessageWindow(id, pid), cursor(0), status(0)
{
}

SelectWindow::~SelectWindow()
{
}

int SelectWindow::Init(std::shared_ptr<const std::vector<std::string>> strdat, Vector2D<int> pos, Vector2D<int> s)
{
	MessageWindow::Init(strdat, pos, s);
	//�I�������o
	int idx(0);
	int max_width(0);
	_max_line = 0;
	for (const string& str : *strings){
		if (!CheckCommand(str)){
			++_max_line;
			select_line.push_back(idx);
			int w = GetDrawStringWidthToHandle((*strings)[idx].c_str(), (*strings)[idx].size(), _font_handle);
			if (w > max_width){ max_width = w; }
			PushBackDrawStrings((*strings)[idx]);
		}
		++idx;
	}
	Vector2D<int> p_size = size;
	size.Set(max_width + 24, select_line.size()*word_size + 10);
	if (((int)_fixed_pos&(int)FixedPosition::Bottom)){
		position.y -= (size.y - p_size.y);
	}
	if (((int)_fixed_pos&(int)FixedPosition::Right)){
		position.x -= (size.x - p_size.x);
	}
	//�L�[���͑҂��I��
	_waiting_key_push = true;
	return 0;
}

void SelectWindow::Update()
{
	//	ResistDrawList(0);
	if (status == 1){
		if (_line == -1){
			//�e��������e�̕ϐ��̈�ɃZ�b�g
			DeleteRequestMWM();
			return;
		} //���߂��Ȃ�������j��
		if (!enable){ return; } //����s�\��������X�L�b�v
		bool pause(false); //�ꎞ��~��
		while (CheckCommand((*strings)[_line]))
		{
			if (ProcessCommand((*strings)[_line])){ //�R�}���h���s���s
				PushBackDrawStrings("error �R�}���h���s���s : " + (*strings)[_line]); //�G���[�\��
				select_line.insert(select_line.begin() + cursor + 1, (int)strings->size() - 1);
			}
			else{ //�R�}���h���s������ꎞ��~
				pause = true;
				++_line;
				if (_line >= (int)(*strings).size()){
					_line = -1;
				}
				break;
			}
			if (_line >= (int)(*strings).size()){
				_line = -1;
			}
			break;
		}
		if (!pause){
			//			DeleteRequestMWM();
			_line = -1;
		}
	}
}

void SelectWindow::Draw()
{
	//�E�C���h�E
	DrawWindow();
	//�I����
	int line(0);
	for (string& l : _draw_strings){
		DrawFormatStringToHandle(position.x + 21, position.y + 5 + line*word_size, _string_col_handle, _font_handle, "%s", l.c_str());
		++line;
	}
	//�J�[�\��
	DrawStringToHandle(position.x + 5, position.y + 5 + cursor*word_size, "��", _string_col_handle, _font_handle);
}

int SelectWindow::MoveCursor(int m)
{
	if (!enable){ return 0; }
	cursor += m;
	if (cursor < 0){ cursor = (int)select_line.size() - 1; return -1; }
	if (cursor >= (int)select_line.size()){ cursor = 0; return  -1; }
	return 0;
}

void SelectWindow::UpKeyPushed()
{
	sdm.PlaySoundByID("Select");
	MoveCursor(-1);
}

void SelectWindow::DownKeyPushed()
{
	sdm.PlaySoundByID("Select");
	MoveCursor(1);
}

void SelectWindow::SelectKeyPushed()
{
	if (!enable){ return; }
	sdm.PlaySoundByID("Define");
	if ((int)select_line.size() == 0){ status = 1; return; }
	_line = select_line[cursor] + 1;
	if (_line >= (int)strings->size()){ _line = -1; }
	if (GetParentID() >= 0){
		MessageWindowManager::GetInstance().SetMWValue("SelectedIndex", cursor, GetParentID());
	}
	//���Ȃ�������O���[�o���̈�ɃZ�b�g
	else
	{
		MessageWindowManager::GetInstance().SetMWValue("SelectedIndex", cursor);
	}
	status = 1;
}

int SelectWindow::RecieveMessage(const string& mess, int param)
{
	if (MessageWindow::RecieveMessage(mess, param) == 0){ return 0; } //�e�N���X�̃��b�Z�[�W����
	return -1;
}

void SelectWindow::ResetStrings(shared_ptr<const vector<string>>){
	//������̃��Z�b�g�ɂ͖��Ή��Ȃ̂ŉ������Ȃ��B
	printf("SelectWindow::ResetStrings �I���E�C���h�E�ł͕�����f�[�^�̃��Z�b�g�ɑΉ����Ă��܂���B\n");
}