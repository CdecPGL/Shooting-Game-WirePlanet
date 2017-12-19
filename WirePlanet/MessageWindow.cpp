#include "MessageWindow.h"
#include "MassageWindowManager.h"
#include"MessageWindowUtilityClass.h"
using namespace std;

const int WINDOW_EDGE_RADIUS(8);
const int DEFAULT_WORD_DRAW_FRAME(1);

//���b�Z�[�W

MessageWindow::MessageWindow(int id, int pid) : ID(id), enable(true), pID(pid), cID(-1), sdm(SoundManager::GetInstance()), _string_col_handle(-1), _font_handle(-1), _name_font_handle(-1)
, _word_draw_frame(DEFAULT_WORD_DRAW_FRAME), _waiting_key_push(false), _wait_frame(0), _line(-1), _raw(-1), _word_draw_cntr(_word_draw_frame), _inc_cntr(0), _current_face_graph(-1), _fixed_pos(FixedPosition::None)
{
	_max_line = size.y / word_size - 1;
}

MessageWindow::~MessageWindow()
{
}

int MessageWindow::Init(std::shared_ptr<const std::vector<std::string>> strs, Vector2D<int> p, Vector2D<int> s)
{
	//�ʒu�A�T�C�Y�A������f�[�^�Z�b�g
	strings = strs;
	position = p;
	size = s;
	//�L�[���̓��b�Z�[�W�Ή������o�֐��}�b�v�o�^
	keypushmess_map.insert(pair<string, void(MessageWindow::*)()>("UpKeyPushed", &MessageWindow::UpKeyPushed));
	keypushmess_map.insert(pair<string, void(MessageWindow::*)()>("DownKeyPushed", &MessageWindow::DownKeyPushed));
	keypushmess_map.insert(pair<string, void(MessageWindow::*)()>("LeftKeyPushed", &MessageWindow::LeftKeyPushed));
	keypushmess_map.insert(pair<string, void(MessageWindow::*)()>("RightKeyPushed", &MessageWindow::RightKeyPushed));
	keypushmess_map.insert(pair<string, void(MessageWindow::*)()>("SelectKeyPushed", &MessageWindow::SelectKeyPushed));
	keypushmess_map.insert(pair<string, void(MessageWindow::*)()>("CancelKeyPushed", &MessageWindow::CancelKeyPushed));
	//�\���\�s���v�Z
	_max_line = size.y / word_size - 1;
	//���x�����
	AnalyzeLabel();
	return 0;
}

int MessageWindow::End(){
	//�e�ɔj������邱�Ƃ𑗐M
	MessageWindowManager::GetInstance().SendMessage(pID, "ChildWindowDestructed", ID);
	//�q�ɔj������邱�Ƃ𑗐M
	MessageWindowManager::GetInstance().SendMessage(cID, "ParentWindwDestructed", ID);
	return 0;
}

int MessageWindow::DeleteRequestMWM(int mode){
	return MessageWindowManager::GetInstance().DeleteRequest(ID, mode);
}

int MessageWindow::GetParentID()const{ return pID; }
int MessageWindow::GetChildID()const{ return cID; }
Vector2D<int> MessageWindow::GetPosition()const{ return position; }
Vector2D<int> MessageWindow::GetSize()const{ return size; }

void MessageWindow::SetStringColorHandle(int ch){
	_string_col_handle = ch;
}
void MessageWindow::SetFontHandle(int fh){
	_font_handle = fh;
}
void MessageWindow::SetNameFontHandle(int fh){
	_name_font_handle = fh;
}

int MessageWindow::RecieveMessage(const std::string& mess, int param){
	if (ProcessKeyPushedMessage(mess) == 0){ return 0; } //�L�[���͌n���b�Z�[�W����
	if (mess == "ChildWindowDestructed"){ //�q�E�C���h�E���j�����ꂽ
		if (cID == param){
			enable = true; //�E�C���h�E�����L���ɂ���
			cID = -1;
		}
		return 0;
	}
	else if (mess == "ParentWindowDestructed"){
		if (pID == param){
			pID = -1;
		}
		return 0;
	}
	else{
		return -1;
	}
}

int MessageWindow::GetMyID()const{ return ID; }

void MessageWindow::DrawWindow(){
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	/*DrawBox(position.x, position.y, (position + size).x, (position + size).y, GetColor(255, 255, 255), true);
	DrawBox(position.x + 3, position.y + 3, (position + size).x - 3, (position + size).y - 3, GetColor(0, 0, 50), true);*/
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	/*DrawCircle(position.x + WINDOW_EDGE_RADIUS, position.y + WINDOW_EDGE_RADIUS, WINDOW_EDGE_RADIUS, GetColor(0, 0, 0), false, 1);
	DrawCircle((position+size).x - 1 - WINDOW_EDGE_RADIUS, position.y + WINDOW_EDGE_RADIUS, WINDOW_EDGE_RADIUS, GetColor(0, 0, 0), false, 1);
	DrawCircle((position+size).x - 1 - WINDOW_EDGE_RADIUS, (position+size).y - 1 - WINDOW_EDGE_RADIUS, WINDOW_EDGE_RADIUS, GetColor(0, 0, 0), false, 1);
	DrawCircle(position.x + WINDOW_EDGE_RADIUS, (position+size).y - 1 - WINDOW_EDGE_RADIUS, WINDOW_EDGE_RADIUS, GetColor(0, 0, 0), false, 1);
	DrawLine(position.x + WINDOW_EDGE_RADIUS, position.y, (position + size).x - 1 - WINDOW_EDGE_RADIUS, position.y, GetColor(0, 0, 0), false);*/
	DrawBox(position.x, position.y, (position + size).x, (position + size).y, GetColor(0, 0, 0), false);
	DrawBox(position.x + 1, position.y + 1, (position + size).x - 1, (position + size).y - 1, GetColor(255, 255, 255), true);
}

void MessageWindow::DrawFaceGraphic(const Vector2D<int>& relative_pos, const Vector2D<int>& size){
	//�b�҂��Z�b�g����Ă���
	if (_current_speaker){
		Vector2D<int> draw_pos = position + relative_pos;
		//�摜���L����������
		if (_current_face_graph >= 0){
			DrawExtendGraph(draw_pos.x, draw_pos.y, (draw_pos + size).x, (draw_pos + size).y, _current_face_graph, true);
		}
		//������������
		else{
			DrawBox(draw_pos.x, draw_pos.y, (draw_pos + size).x, (draw_pos + size).y, _string_col_handle, false);
			DrawStringToHandle(draw_pos.x, draw_pos.y, "No Image", _string_col_handle, _font_handle);
		}
	}
}

void MessageWindow::UpKeyPushed(){}
void MessageWindow::DownKeyPushed(){}
void MessageWindow::LeftKeyPushed(){}
void MessageWindow::RightKeyPushed(){}
void MessageWindow::SelectKeyPushed(){}
void MessageWindow::CancelKeyPushed(){}

void MessageWindow::UpdateCounter(){
	if (_wait_frame){ --_wait_frame; }
	++_word_draw_cntr;
	++_inc_cntr;
}

int MessageWindow::ProcessKeyPushedMessage(const std::string& mess)
{
	//�L�[���͑҂��o�Ȃ�������X�L�b�v
	if (!_waiting_key_push){ return -2; }
	//�L�[���̓��b�Z�[�W�o�Ȃ�������X�L�b�v
	map<string, void(MessageWindow::*)()>::iterator it = keypushmess_map.find(mess);
	if (it == keypushmess_map.end()){ return -1; }
	(this->*(it->second))();
	return 0;
}

bool MessageWindow::CheckCommand(const std::string& str) const
{
	//�R�}���h���ʎq�m�F
	if (str.substr(0, 2) == "\\\\"){ return true; }
	return false;
}

string MessageWindow::GetNextString(){
	if (strings->size() == 0){ return "NULL"; }
	auto it = strings->end() - 1;
	while (ProcessCommand(*it) != -1 && it != strings->end()){
		++it;
	}
	return it == strings->end() ? "NULL" : (*it);
}

void MessageWindow::PushFrontDrawStrings(const string& str){
	_draw_strings.push_front(str);
	if ((int)_draw_strings.size() > _max_line){ _draw_strings.pop_back(); }
}

void MessageWindow::PushBackDrawStrings(const std::string& str){
	_draw_strings.push_back(str);
	if ((int)_draw_strings.size() > _max_line){ _draw_strings.pop_front(); }
}

int MessageWindow::ProcessCommand(const std::string& cmnd_str)
{
	//�R�}���h���ʎq�m�F
	if (!CheckCommand(cmnd_str)){ return -1; }
	//�R�}���h�ƃR�}���h�����𕪗�
	int space_pos = (int)cmnd_str.find_first_of("="); //�R�}���h�ƈ�������؂�C�R�[���̈ʒu
	string command;
	string prmts;
	if (space_pos != string::npos){
		command = cmnd_str.substr(2, space_pos - 2);
		prmts = cmnd_str.substr(space_pos + 1, cmnd_str.size() - space_pos - 1).c_str(); //�p�����[�^�Q
	}
	else{
		command = cmnd_str.substr(2, cmnd_str.size() - 2);
	}
	//�p�����[�^�𕪗�
	vector<string> prmt; //�p�����[�^
	int idx;
	while ((idx = prmts.find_first_of("&")) != string::npos){
		prmt.push_back(prmts.substr(0, idx));
		prmts.erase(prmts.begin(), prmts.begin() + idx + 1);
	}
	if (space_pos != string::npos){ prmt.push_back(prmts); }
	bool pause = false;
	//�R�}���h���s
	if (command == READ_TEXT){
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		_draw_strings.clear();
		auto it = MessageWindowManager::GetInstance().GetMessageByID((int)strtol(prmt[0].c_str(), NULL, 10));
		if (it == nullptr){ return -3; }
		ResetStrings(it);
	}
	//if (command == CREATE_TEXT_WIN){
	//	if (cID != -1){ return -2; } //�q�����݂�����쐬���Ȃ�
	//	if (prmt.size() < 1){ return -2; } //����������Ȃ�
	//	int crt_mode = 0;
	//	if (prmt.size() >= 2){ crt_mode = (int)strtol(prmt[1].c_str(), NULL, 10); }
	//	int crt_win_pid=ID;
	//	switch (crt_mode)
	//	{
	//	case 0:
	//		crt_win_pid = ID;
	//		break;
	//	case 2:
	//		MessageWindowManager::GetInstance().DeleteRequest(ID, 1);
	//	case 1:
	//		crt_win_pid = -1;
	//		break;
	//	default:
	//		break;
	//	}
	//	int nid = MessageWindowManager::GetInstance().CreateRequestDefaultSizeWindow(TEXT_WIN, (int)strtol(prmt[0].c_str(), NULL, 10), crt_win_pid);
	//	if (nid < 0){ return -2; } //�E�C���h�E�̍쐬�Ɏ��s
	//	if (crt_mode == 0){
	//		cID = nid;
	//		enable = false; //�q�E�C���h�E���j�������܂ő���s�\�ɂ���
	//	}
	//}
	else if (command == CREATE_SLCT_WIN){
		if (cID != -1){ return -2; } //�q�����݂�����쐬���Ȃ�
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		int crt_mode = 0;
		if (prmt.size() >= 2){ crt_mode = (int)strtol(prmt[1].c_str(), NULL, 10); }
		int crt_win_pid = ID;
		switch (crt_mode)
		{
		case 0:
			crt_win_pid = ID;
			break;
		case 2:
			MessageWindowManager::GetInstance().DeleteRequest(ID, 1);
		case 1:
			crt_win_pid = -1;
			break;
		default:
			break;
		}
		int nid = MessageWindowManager::GetInstance().CreateRequest(SLCT_WIN, (int)strtol(prmt[0].c_str(), NULL, 10), position, Vector2D<int>(), FixedPosition::Bottom, crt_win_pid);
		if (nid < 0){ return -2; } //�E�C���h�E�̍쐬�Ɏ��s
		if (crt_mode == 0){
			cID = nid;
			enable = false; //�q�E�C���h�E���j�������܂ő���s�\�ɂ���
		}
	}
	//else if (command == CREATE_INPT_WIN){
	//	if (cID != -1){ return -2; } //�q�����݂�����쐬���Ȃ�
	//	if (prmt.size() < 1){ return -2; } //����������Ȃ�
	//	int crt_mode = 0;
	//	if (prmt.size() >= 2){ crt_mode = (int)strtol(prmt[1].c_str(), NULL, 10); }
	//	int crt_win_pid = ID;
	//	switch (crt_mode)
	//	{
	//	case 0:
	//		crt_win_pid = ID;
	//		break;
	//	case 2:
	//		MessageWindowManager::GetInstance().DeleteRequest(ID, 1);
	//	case 1:
	//		crt_win_pid = -1;
	//		break;
	//	default:
	//		break;
	//	}
	//	int nid = MessageWindowManager::GetInstance().CreateRequestDefaultSizeWindow(INPT_WIN, (int)strtol(prmt[0].c_str(), NULL, 10),crt_win_pid);
	//	if (nid < 0){ return -2; } //�E�C���h�E�̍쐬�Ɏ��s
	//	if (crt_mode == 0){
	//		cID = nid;
	//		enable = false; //�q�E�C���h�E���j�������܂ő���s�\�ɂ���
	//	}
	//}
	else if (command == SET_MW_VALUE){
		if (prmt.size() < 2){ return -2; } //����������Ȃ�
		string area_str("G");
		if (prmt.size() >= 3){ area_str = prmt[2]; }
		int area = -1;
		if (prmt[2] == "L"){ area = ID; }
		else if (prmt[2] == "G"){ area = -1; }
		MessageWindowManager::GetInstance().SetMWValue(prmt[0], prmt[1], area);
	}
	else if (command == CLOSE_WIN){
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		if (DeleteRequestMWM((int)strtol(prmt[0].c_str(), NULL, 10))){ return -2; }
	}
	else if (command == CHANGE_SPEAKER){
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		_current_speaker = MessageWindowManager::GetInstance().GetSpeakerData(prmt[0]);
		_draw_strings.clear();
		if (_current_speaker == nullptr){
			printf("�w�肳�ꂽ�b�҂����݂��܂���:%s\n", prmt[0].c_str());
		}
		else{
			//�f�t�H���g�摜���Z�b�g����Ă����猻�݂̉摜������ɂ���
			if (_current_speaker->default_graphic >= 0){
				_current_face_graph = _current_speaker->default_graphic;
			}
			else{
				_current_face_graph = -1;
			}
		}
	}
	else if (command == CHANGE_EXPRESSION){
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		if (_current_speaker){
			auto it = _current_speaker->graphics.find(prmt[0]);
			if (it == _current_speaker->graphics.end()){
				printf("�w�肳�ꂽ�\����݂��܂���:%s\n", prmt[0].c_str());
			}
			else{
				_current_face_graph = (*it).second;
			}
		}
		else{
			printf("�b�҂��Z�b�g����Ă��܂���:%s\n", prmt[0].c_str());
		}
	}
	else if (command == WAIT_KEY_PUSH){
		_waiting_key_push = true;
		pause = true;
	}
	else if (command == WAIT_TIME){
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		_wait_frame = (int)strtol(prmt[0].c_str(), NULL, 10);
		pause = true;
	}
	else if (command == CHANGE_DRAW_SPEED){
		//		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		if (prmt.size() >= 1){
			_word_draw_frame = (int)strtol(prmt[0].c_str(), NULL, 10);
		}
		else{
			_word_draw_frame = DEFAULT_WORD_DRAW_FRAME;
		}
	}
	else if (command == CLEAR){
		_draw_strings.clear();
	}
	else if (command == JUMP){
		if (prmt.size() < 1){ return -2; } //����������Ȃ�
		int idx = FindLabel(prmt[0]);
		if (idx < 0){ return -1; }
		_line = idx;
	}
	else if (command == IF_EQUAL_JUMP){
		if (prmt.size() < 3){ return -2; } //����������Ȃ�
		int idx = FindLabel(prmt[2]);
		if (idx < 0){ return -1; }
		prmt[0] = move(PermuteVariable(prmt[0]));
		prmt[1] = move(PermuteVariable(prmt[1]));
		if (prmt[0] == prmt[1]){
			_line = idx;
		}
	}
	else if (command == IFN_EQUAL_JUMP){
		if (prmt.size() < 3){ return -2; } //����������Ȃ�
		int idx = FindLabel(prmt[2]);
		if (idx < 0){ return -1; }
		prmt[0] = move(PermuteVariable(prmt[0]));
		prmt[1] = move(PermuteVariable(prmt[1]));
		if (prmt[0] != prmt[1]){
			_line = idx;
		}
	}
	else{
		return -3; //�R�}���h������
	}
	return pause ? 1 : 0;
}

//���x���֘A
bool MessageWindow::IsLabel(const string& str)const{
	if (str.size() < 3){ return false; }
	return str.substr(0, 2) == "@@" ? true : false;
}
void MessageWindow::AnalyzeLabel(){
	_label.clear();
	int i = 0;
	for (const auto& l : *strings){
		if (IsLabel(l)){
			_label.insert(make_pair(l.substr(2, l.size() - 2), i));
		}
		++i;
	}
}
int MessageWindow::FindLabel(const string& ln)const{
	auto it = _label.find(ln);
	return it == _label.end() ? -1 : (*it).second;
}

//�ϐ��֘A
string MessageWindow::PermuteVariable(const string& str){
	string out(str);
	int s_pos = 0;
	int e_pos = -2;
	while (true){
		s_pos = str.find_first_of("<<", e_pos + 2);
		if (s_pos == string::npos){ break; }
		e_pos = str.find_first_of(">>", s_pos + 2);
		if (e_pos == string::npos){ break; }
		string val_name = str.substr(s_pos + 2, e_pos - s_pos - 2);
		string val;
		val = MessageWindowManager::GetInstance().GetMWValue(val_name, GetMyID());
		if (val == "NULL"){
			val = MessageWindowManager::GetInstance().GetMWValue(val_name);
			if (val == "NULL"){ continue; }
		}
		out.replace(s_pos, e_pos - s_pos + 2, val);
	}
	return move(out);
}

void MessageWindow::ResetStrings(shared_ptr<const vector<string>> nstrings){
	strings = nstrings;
	_line = -1;
	_raw = 0;
	_word_draw_cntr = 0;
	_inc_cntr = 0;
	AnalyzeLabel();
}

void MessageWindow::SetFixedPosition(FixedPosition fp){ _fixed_pos = fp; }

bool MessageWindow::SJISMultiCheck(unsigned char c){
	if (((c >= 0x81) && (c <= 0x9f)) || ((c >= 0xe0) && (c <= 0xfc))){ return true; }
	else { return false; }
}