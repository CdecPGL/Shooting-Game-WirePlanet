#include "MassageWindowManager.h"
#include "DebugWindow.h"
#include "TextWindow.h"
#include "SelectWindow.h"
#include "InputWindow.h"
#include"SingleLineWindow.h"
#include"InitDatas.h"
#include"boost/lexical_cast.hpp"
#include"ObjectHolderTemplate_WithID.h"
#include"MessageWindowUtilityClass.h"
#include"FileLoadUtility.h"
#include"GraphicManager.h"

using namespace std;

namespace{
	const string MESSAGE_TEXT_FILE("message_text.txt");

	const int DEFAULT_TALK_WINDOW_HEIGHT(96);
	const int DEFAULT_CHILD_MARGINE(4);
	const int NORMAL_STRING_COLOR_R(0);
	const int NORMAL_STRING_COLOR_G(0);
	const int NORMAL_STRING_COLOR_B(0);
	const char* NORMAL_FONT_NAME("メイリオ");
	const int NORMAL_FONT_SIZE(16);
	const int NORMAL_FONT_THICK(2);
	const int NORMAL_FONT_TYPE(DX_FONTTYPE_ANTIALIASING);
	const int NORMAL_NAME_FONT_SIZE(12);
}


MessageWindowManager::MessageWindowManager() :mess_window_holder(make_unique<ObjectHolderTemplate_WithID<int, MessageWindow>>()), _normal_font_handle(-1), _normal_string_color_handle(-1), _normal_name_font_handle(-1)
{
	printf("MessageWindowManagerのインスタンスが作成されました。\n");
}


MessageWindowManager::~MessageWindowManager()
{
	if (_normal_font_handle >= 0)DeleteFontToHandle(_normal_font_handle);
	if (_normal_name_font_handle >= 0)DeleteFontToHandle(_normal_name_font_handle);
	printf("MessageWindowManagerのインスタンスが破棄されました。\n");
}

MessageWindowManager& MessageWindowManager::GetInstance()
{
	static MessageWindowManager mm;
	return mm;
}

int MessageWindowManager::Init(){
	Clear();
	ClearMWValue();
	id_counter = 0;
	int error = LoadMessageFromFile();
	if (error){
		printf("MessageWindowManagerの初期化に失敗しました。\n");
	}
	else{
		printf("MessageWindowManagerが初期化されました。\n");
	}

	//標準文字設定
	_normal_string_color_handle = GetColor(NORMAL_STRING_COLOR_R, NORMAL_STRING_COLOR_G, NORMAL_STRING_COLOR_B);
	if (_normal_font_handle < 0)_normal_font_handle = CreateFontToHandle(NORMAL_FONT_NAME, NORMAL_FONT_SIZE, NORMAL_FONT_THICK, NORMAL_FONT_TYPE);
	if (_normal_name_font_handle < 0)_normal_name_font_handle = CreateFontToHandle(NORMAL_FONT_NAME, NORMAL_NAME_FONT_SIZE, NORMAL_FONT_THICK, NORMAL_FONT_TYPE);
	if (_normal_string_color_handle == -1 || _normal_font_handle == -1 || _normal_name_font_handle==-1){ error = -2; }

	return error;
}

int MessageWindowManager::Reset(){
	Clear();
	global_value.clear();
	return 0;
}

int MessageWindowManager::Process(){
	mess_window_holder->process_unresist_list();
	mess_window_holder->process_resist_list();
	return 0;
}

void MessageWindowManager::Update(){
	for (auto it = mess_window_holder->objects.begin(); it != mess_window_holder->objects.end(); ++it){
		(*it).second->Update();
	}
}

void MessageWindowManager::Draw(){
	for (auto it = mess_window_holder->objects.begin(); it != mess_window_holder->objects.end(); ++it){
		(*it).second->Draw();
	}
}

int MessageWindowManager::CreateRequest(MessageWindowKind mwk,int s_id,Vector2D<int> pos,Vector2D<int> size,MessageWindow::FixedPosition fp,int pid){
	int id = id_counter;
	auto it = messages.find(s_id);
	if (it == messages.end()){ return -2; }
	shared_ptr<MessageWindow> nmw;
	switch (mwk)
	{
	case TEXT_WIN:
		nmw = make_shared<TextWindow>(id,pid);
		break;
	case SLCT_WIN:
		nmw = make_shared<SelectWindow>(id,pid);
		break;
	case INPT_WIN:
		nmw = make_shared<InputWindow>(id,pid);
		break;
	default:
		break;
	}
	nmw->SetFixedPosition(fp);
	nmw->SetStringColorHandle(_normal_string_color_handle);
	nmw->SetFontHandle(_normal_font_handle);
	nmw->SetNameFontHandle(_normal_name_font_handle);
	if (nmw->Init((*it).second, pos, size) != 0){
		nmw.reset();
		return -2;
	}
	if (mess_window_holder->resist_object(nmw, id)){ return -2; }
	++id_counter;
	printf("MessageWindowManager:メッセージウインドウを作成しました。(mess_id=%d)\n", s_id);
	return id;
}

int MessageWindowManager::CreateRequestMinimamSizeWindow(MessageWindowKind mwk, int s_id, int pid){
	Vector2D<int> pos, size;
	//親がマネージャ以外だったら探す
	if (pid >= 0){
		auto it = mess_window_holder->objects.find(pid);
		//指定された親がなかったらエラー
		if (it == mess_window_holder->objects.end()){
			return -2;
		}
		pos = (*it).second->GetPosition() + Vector2D<int>(DEFAULT_CHILD_MARGINE, 0);
	}
	auto mess_it = messages.find(s_id);
	if (mess_it == messages.end()){ return -2; }
	int max_width = 0;
	int line_count=0;
	for (string& l : *((*mess_it).second)){
		if (l.substr(0, 2) == "\\\\"){ continue; }
		++line_count;
		int width = GetDrawStringWidth(l.c_str(), l.size());
		if (width > max_width){ max_width = width; }
	}
	size.Set(max_width + 20, line_count * (NORMAL_FONT_SIZE+4)+20);
	pos.y = game_draw_size.y - size.y;
	return CreateRequest(mwk, s_id, pos, size,MessageWindow::FixedPosition::None, pid);
}

int MessageWindowManager::DeleteRequest(int id, int mode){
	if (mess_window_holder->objects.find(id) == mess_window_holder->objects.end()){ return -1; }
	vector<int> del_id; //消去するID
	del_id.push_back(id); //指定ID
	if (mode == 1 || mode == 3){ //子
		int checking_id = id;
		while (checking_id != -1){
			auto it = mess_window_holder->objects.find(checking_id);
			if (it == mess_window_holder->objects.end()){ break; }
			checking_id = (*it).second->GetChildID();
			del_id.push_back(checking_id);
		}
		del_id.pop_back();
	}
	if (mode == 2 || mode == 3){ //親
		int checking_id = id;
		while (checking_id != -1){
			auto it = mess_window_holder->objects.find(checking_id);
			if (it == mess_window_holder->objects.end()){ break; }
			checking_id = (*it).second->GetParentID();
			del_id.push_back(checking_id);
		}
		del_id.pop_back();
	}

	for (int did : del_id){
		mess_window_holder->objects.find(did)->second->End();
		mess_window_holder->unresist_object(did);
	}
	//削除対象ウインドウの変数領域削除
	//削除対象が会話ウインドウだったら選択されたインデックスを保存しておき、会話ウインドウを非アクティブ化(IDをリセット)する
	if (id == _current_talk_win.window_id){
		int lsi = GetMWValueByInteger("SelectedIndex", _current_talk_win.window_id);
		if (lsi != 0xFFFFFF){
			_current_talk_win.last_selected_index = lsi;
		}
		_current_talk_win.window_id = -1;
	}
	ClearMWValue(id);
	return 0;
}

shared_ptr<MessageWindow> MessageWindowManager::GetMWPointer(int id){
	auto it = mess_window_holder->objects.find(id);
	shared_ptr<MessageWindow> out;
	if (it == mess_window_holder->objects.end()){ return out; }
	return (*it).second;
}

const shared_ptr<SpeakerData> MessageWindowManager::GetSpeakerData(const string& sid)const {
	auto it = _speaker_list.find(sid);
	return it == _speaker_list.end() ? nullptr : (*it).second;
}

int MessageWindowManager::SendMessage(int id,const string& mess,int param){
	auto it = mess_window_holder->objects.find(id);
	if (it == mess_window_holder->objects.end()){
		//リストになかったら新規作成されてまだリストに登録されていないものから探す。
		it = mess_window_holder->res_list.find(id);
		if (it == mess_window_holder->res_list.end()){ return -2; }
	}
	return (*it).second->RecieveMessage(mess,param);
}

int MessageWindowManager::LoadMessageFromFile()
{
	//ゲーム描画サイズ取得
	try{
		game_draw_size.Set(boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawWidth")), boost::lexical_cast<int>(InitDatas::GetInitDatas("Init").at("DrawHeight")));
	}
	catch (InitDatas::init_datas_error& e){
		printf("初期データのセクションが不足しています。(%s)\n", e.what());
		return -2;
	}
	catch (out_of_range& e){
		printf("初期データが不足しています。(%s)\n", e.what());
	}
	catch (boost::bad_lexical_cast& e){
		printf("描画サイズの取得に失敗しました。(%s)\n",e.what());
		return -3;
	}

	DataScript ds;
	if (int error = FileLoadUtil::LoadDataScriptFromFLM(&ds,MESSAGE_TEXT_FILE,true) != 0){ 
		printf("MessageWindowManager::LoadMessageFromFile ファイルからのメッセージ読み込みに失敗しました。\n");
		SetFormatDebugInfo(2, "MessageWindowManager::LoadMessageFromFile", "ファイルからのメッセージ読み込みに失敗しました。", error);
		return -1; 
	}
	//データ読み込み
	_speaker_list.clear();
	messages.clear();
	vector<string> db_list = ds.GetDataName();
	for (string& db : db_list){
		if (db.substr(0, 7) == "message"){ //メッセージだったら
			try{
				int mess_num = boost::lexical_cast<int>(db.substr(7, db.size() - 7));
				shared_ptr<vector<string>> message = make_shared<vector<string>>();
				vector<vector<string>> data;
				ds.GetDataAll(db, data);

				for (vector<vector<string>>::iterator it = data.begin(); it != data.end(); ++it){
					string str;
					for (vector<string>::iterator sit = (*it).begin(); sit != (*it).end(); ++sit){
						str += (*sit);
					}
					message->push_back(str);
				}

				messages.insert(map<int, shared_ptr<vector<string>>>::value_type(mess_num, message));
			}
			catch (boost::bad_lexical_cast&){
				continue;
			}
		}
		else if (db.substr(0, 7) == "speaker"){ //話者定義ブロックだったら
			const DataBlock& s_db = ds.GetRootDataBlock().FindDataBlock(db);
			//話者リスト取得
			vector<string> spkr_list = s_db.GetDataBlockList();
			//各話者の定義読み込み
			for (string& spkr : spkr_list){
				const DataBlock& spkr_db = s_db.FindDataBlock(spkr);
				const vector<vector<string>>& spkr_dat = spkr_db.GetAllData();
				//名前取得(最初に出現した名前を採用する)
				string spkr_name="NULL";
				for (const vector<string>& l : spkr_dat){
					if (l.size() >= 2){
						if (l[0] == "name"){
							spkr_name = l[1];
							break;
						}
					}
				}
				//名前の取得に失敗したらスキップ
				if (spkr_name == "NULL"){ 
					printf("MessageWindowManager::LoadMessageFromFile:話者定義が不完全です。名前が定義されていません。(%s)\n", spkr.c_str());
					continue;
				}
				//話者データ作成
				shared_ptr<SpeakerData> spd = make_shared<SpeakerData>(spkr_name);
				//画像定義読み込み
				try{
					const DataBlock& spkr_g_db = spkr_db.FindDataBlock("graphic");
					const vector<vector<string>>& spkr_g_dat = spkr_g_db.GetAllData();
					string g_pass("NULL");
					Vector2D<int> g_size(0, 0);
					//パスとサイズ取得
					for (const vector<string>& l : spkr_g_dat){
						if (l.size() == 0){ continue; }
						if (l[0] == "pass"){
							if (l.size() < 2){ continue; }
							g_pass = l[1];
						}
						else if (l[0] == "size"){
							if (l.size() < 3){ continue; }
							g_size.x = (int)strtol(l[1].c_str(), nullptr, 10);
							g_size.y = (int)strtol(l[2].c_str(), nullptr, 10);
						}
					}
					if (g_pass == "NULL"){
						printf("MessageWindowManager::LoadMessageFromFile:話者定義が不完全です。画像パスが定義されていません。(%s)\n", spkr.c_str());
						continue;
					}
					if (g_size.x <= 0 || g_size.y <= 0){
						printf("MessageWindowManager::LoadMessageFromFile:話者定義が不完全です。画像サイズが定義されていません。(%s)\n", spkr.c_str());
						continue;
					}
					//画像読み込み
					Vector2D<int> original_g_size;
					//サイズ確認
					int gh = GraphicManager::GetInstance().GetGraphHandle(g_pass);
					//画像読み込みに失敗したら
					if (gh < 0){
						printf("MessageWindowManager::LoadMessageFromFile:話者定義が不完全です。画像の読み込みに失敗しました。(%s,%s)\n", spkr.c_str(), g_pass.c_str());
						continue;
					}
					GetGraphSize(gh, &original_g_size.x, &original_g_size.y);
					//元画像はoriginalとして格納
					spd->graphics.insert(make_pair("original", gh));
					//表情定義読み込み
					try{
						const DataBlock& exp_db = spkr_g_db.FindDataBlock("expression");
						const vector<vector<string>>& exp_dat = exp_db.GetAllData();
						for (const vector<string>& l : exp_dat){
							if (l.size() >= 3){
								Vector2D<int> t_pos((int)strtol(l[1].c_str(), nullptr, 10), (int)strtol(l[2].c_str(), nullptr, 10));
								int ngh = DerivationGraph(t_pos.x*g_size.x, t_pos.y*g_size.y, g_size.x, g_size.y, gh);
								if (ngh < 0){
									printf("MessageWindowManager::LoadMessageFromFile:話者定義が不完全です。表情画像の読み込みに失敗しました。(%s,%d,%d)\n", spkr.c_str(), t_pos.x, t_pos.y);
									continue;
								}
								spd->graphics.insert(make_pair(l[0], ngh));
								//デフォルト画像がセットされていなかったらセットする
								if (spd->default_graphic < 0){
									spd->default_graphic = ngh;
								}
							}
						}
					}
					catch (out_of_range&){
						printf("MessageWindowManager::LoadMessageFromFile:話者定義が不完全です。表情定義が存在しません。(%s)\n", spkr.c_str());
						continue;
					}
				}
				catch (out_of_range&){
					//画像データが未定義だったらそのまま率とに追加
				}
				//話者データをリストに登録
				_speaker_list.emplace(make_pair(spkr, spd));
			}
		}
	}
	return 0;
}

bool MessageWindowManager::CheckExistence(int id)const{
	if (mess_window_holder->is_exist(id)==false){
		if (mess_window_holder->res_list.find(id) == mess_window_holder->res_list.end()){
			return false;
		}
	}
	return true;
}

int MessageWindowManager::GetMWCount() const
{
	return mess_window_holder->size();
}

std::shared_ptr<const std::vector<std::string>> MessageWindowManager::GetMessageByID(int id)const{
	auto it = messages.find(id);
	if (it == messages.end()){
		printf("MessageWindowManager::GetMessageByID 存在しないメッセージ番号が指定されました。(%d)\n", id);
		return nullptr;
	}
	return (*it).second;
}

void MessageWindowManager::Clear()
{
	mess_window_holder->clear();
	id_counter = 0;
}

void MessageWindowManager::ButtonPushed(ButtonKind in)
{
	for (unordered_map<int,shared_ptr<MessageWindow>>::value_type& mw : mess_window_holder->objects){
		if (in & Select){ mw.second->RecieveMessage("SelectKeyPushed"); }
		else if (in & Cancel){ mw.second->RecieveMessage("CancelKeyPushed"); }
		if (in & Up){ mw.second->RecieveMessage("UpKeyPushed"); }
		if (in & Down){ mw.second->RecieveMessage("DownKeyPushed"); }
		if (in & Left){ mw.second->RecieveMessage("LeftKeyPushed"); }
		if (in & Right){ mw.second->RecieveMessage("RightKeyPushed"); }
	}
}

int MessageWindowManager::SetMWValue(const std::string& name, const std::string& value,int id){
	auto it = global_value.find(id);
	if (it == global_value.end()){ //IDグループ新規作成
		map<string, string> gv;
		gv.insert(make_pair(name, value));
		global_value.insert(make_pair(id, gv));
		return 1;
	}
	else{
		auto it2 = (*it).second.find(name);
		if (it2 == (*it).second.end()){
			//変数新規作成
			(*it).second.insert(make_pair(name, value));
			return 1;
		}
		else{
			(*it2).second = value;
			return 0;
		}
	}
}
int MessageWindowManager::SetMWValue(const string& name, int value, int id){
	try{
		return SetMWValue(name, boost::lexical_cast<string>(value), id);
	}
	catch (boost::bad_lexical_cast& e){
		return -1;
	}
}
int MessageWindowManager::SetMWValue(const string& name, double value, int id){
	try{
		return SetMWValue(name, boost::lexical_cast<string>(value), id);
	}
	catch (boost::bad_lexical_cast& e){
		return -1;
	}
}

string MessageWindowManager::GetMWValue(const std::string& name,int id)const{
	auto it = global_value.find(id);
	if (it == global_value.end()){ return "NULL"; }
	auto it2 = (*it).second.find(name);
	if (it2 == (*it).second.end()){ return "NULL"; }
	return (*it2).second;
}
int MessageWindowManager::GetMWValueByInteger(const std::string& name, int id)const{
	auto it = global_value.find(id);
	if (it == global_value.end()){ return 0xFFFFFF; }
	auto it2 = (*it).second.find(name);
	if (it2 == (*it).second.end()){ return 0xFFFFFF; }
	try{
		return boost::lexical_cast<int>((*it2).second);
	}
	catch (boost::bad_lexical_cast&){
		return 0xFFFFFF;
	}
}

void MessageWindowManager::ClearMWValue(int id){
	auto it = global_value.find(id);
	if (it == global_value.end()){ return; }
	(*it).second.clear();
}

void MessageWindowManager::ClearAllMWValues(){
	global_value.clear();
}

//単行ウインドウ関連
int MessageWindowManager::CreateRequestSingleLineWindowByString(const string& text, int draw_frame,SingleLineWinPos s_pos,SingleLineWinKind s_kind){
	int id = id_counter;
	shared_ptr<SingleLineWindow> nmw = make_shared<SingleLineWindow>(id, -1);
	shared_ptr<vector<string>> strdat = make_shared<vector<string>>();
	strdat->push_back(text);

	const int HEIGHT = 26;
	Vector2D<int> pos, size;
	switch (s_pos){
	case SingleLineWinPos::Bottom:
		pos.Set(0, game_draw_size.y - HEIGHT);
		size.Set(game_draw_size.x, HEIGHT);
		break;
	case SingleLineWinPos::Top:
		pos.Set(0, 0);
		size.Set(game_draw_size.x, HEIGHT);
		break;
	case SingleLineWinPos::Center:
		pos.Set(0, (game_draw_size.y - HEIGHT) / 2);
		size.Set(game_draw_size.x, HEIGHT);
		break;
	}
	if (nmw->Init(strdat, pos, size)){
		return -2;
	}
	nmw->SetDrawFrame(draw_frame);
	if (mess_window_holder->resist_object(nmw, id)){ return -2; }
	nmw->SetStringColorHandle(_normal_string_color_handle);
	nmw->SetFontHandle(_normal_font_handle);
	nmw->SetNameFontHandle(_normal_name_font_handle);
	SingleLineWindow::Kind k;
	switch (s_kind)
	{
	case MessageWindowManager::SingleLineWinKind::Normal:
		k = SingleLineWindow::Kind::Normal;
		break;
	case MessageWindowManager::SingleLineWinKind::WhiteStream:
		k = SingleLineWindow::Kind::WhiteStream;
		break;
	case MessageWindowManager::SingleLineWinKind::BlackStream:
		k = SingleLineWindow::Kind::BlackStream;
		break;
	default:
		break;
	}
	nmw->SetKind(k);
	++id_counter;
	return id;
}

//会話ウインドウ関連
int MessageWindowManager::CreateTalkWindow(int m_num,TalkWinPos w_pos){
	Vector2D<int> pos,size;
	switch (w_pos){
	case TalkWinPos::Top:
		pos.Set(0, 0);
		size.Set(game_draw_size.x, DEFAULT_TALK_WINDOW_HEIGHT);
		break;
	case TalkWinPos::Bottom:
		pos.Set(0, game_draw_size.y - DEFAULT_TALK_WINDOW_HEIGHT);
		size.Set(game_draw_size.x, DEFAULT_TALK_WINDOW_HEIGHT);
		break;
	}
	_current_talk_win.last_selected_index = -1;
	_current_talk_win.window_id = CreateRequest(MessageWindowKind::TEXT_WIN, m_num, pos, size,MessageWindow::FixedPosition::Bottom);
	return 0;
}

int MessageWindowManager::DeleteTalkWindow(){
	if(_current_talk_win.window_id < 0){ return -1; }
	DeleteRequest(_current_talk_win.window_id, 2);
	_current_talk_win.window_id = -1;
	return 0;
}

bool MessageWindowManager::IsTalkWindowActive()const{
	return _current_talk_win.window_id == -1 ? false : true;
}

int MessageWindowManager::GetLastSelectedIndex()const{
	//会話ウインドウがアクティブだったら選択されたインデッくぅを更新
	if (CheckExistence(_current_talk_win.window_id)){
		int lsi = GetMWValueByInteger("SelectedIndex", _current_talk_win.window_id);
		if (lsi != 0xFFFFFF){
			_current_talk_win.last_selected_index = lsi;
		}
	}
	return _current_talk_win.last_selected_index;
}
