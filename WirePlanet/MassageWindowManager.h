#pragma once

#include "MessageWindow.h"
#include "Vector2D.h"
#include "DataScript.h"
#include<memory>
#include <sstream>

#undef SendMessage

template<typename T, class C>
class ObjectHolderTemplate_WithID;


struct SpeakerData;

//ウインドウ種類
enum MessageWindowKind{TEXT_WIN,SLCT_WIN,INPT_WIN};

//データブロック名は"message0～"

//シングルトン

class MessageWindowManager
{
public:
	~MessageWindowManager();
	static MessageWindowManager& GetInstance();
	int Init();
	int Reset();
	int Process();

	void Update();
	void Draw();

	int CreateRequest(MessageWindowKind, int, Vector2D<int>, Vector2D<int>,MessageWindow::FixedPosition, int = -1); //メッセージ番号を選択してメッセージウインドウを作成(種類,メッセージ番号,位置,サイズ[,親ID:デフォルトで-1(マネージャーが親)])(-1で失敗)
	int CreateRequestAndSetString(MessageWindowKind, const std::string&, Vector2D<int>, Vector2D<int>, int = -1); //テキストを指定してメッセージウインドウを作成(種類,テキスト文字列,位置,サイズ[,親ID:デフォルトで-1(マネージャーが親)])(-1で失敗)
	int CreateRequestMinimamSizeWindow(MessageWindowKind, int, int = -1); ////メッセージ番号を選択して最小サイズのメッセージウインドウを作成する。(種類,メッセージ番号[,親ID:デフォルトで-1(マネージャーが親)])(-1で失敗)
	int DeleteRequest(int,int=0); //IDでメッセージウインドウを削除。高機能版(ID,消去方法0:ふつう 1:子も消去 2:親も消去 3:親も子も消去)
	int SendMessage(int, const std::string&, int); //指定IDのウインドウにメッセージを送信(ID,メッセージ,パラメータ)(戻り値 -1:無効なメッセージ -2:IDが無効)
	std::shared_ptr<MessageWindow> GetMWPointer(int); //IDで指定したウインドウのポインタを取得

	const std::shared_ptr<SpeakerData> GetSpeakerData(const std::string&)const; //話者データ取得

	bool CheckExistence(int)const; //指定IDにウインドウが存在するか(作成リクエストからも探す)

	int GetMWCount()const; //登録されているウインドウの数を取得

	std::shared_ptr<const std::vector<std::string>> GetMessageByID(int)const;  //指定番号のメッセージ取得

	enum ButtonKind{Select=1,Cancel=2,Up=4,Down=8,Left=16,Right=32};
	void ButtonPushed(ButtonKind); //ボタンが押された(押されたボタン)

	//変数系関数
	int SetMWValue(const std::string&, const std::string&,int = -1); //メッセージウインドウ変数セット(変数名、内容[、ローカルIDを指定。なしでグローバル領域(-1)])(return 0:上書き 1:新規作成)
	int SetMWValue(const std::string&, int, int = -1); //メッセージウインドウ変数セット(変数名、内容[、ローカルIDを指定なしでグローバル領域(-1)])(return 0:上書き 1:新規作成)
	int SetMWValue(const std::string&, double, int = -1); //メッセージウインドウ変数セット(変数名、内容[、ローカルIDを指定なしでグローバル領域(-1)])(return 0:上書き 1:新規作成)
	std::string GetMWValue(const std::string&,int = -1)const; //メッセージウインドウ変数取得(変数名[、ローカルIDを指定。指定しない場合はグローバル領域(-1)])(無しで"NULL"が返される)
	int GetMWValueByInteger(const std::string&, int = -1)const; //メッセージウインドウ変数取得(変数名[、ローカルIDを指定。指定しない場合はグローバル領域(-1)])(無しで0xFFFFFFが返される)
	void ClearMWValue(int = -1); //指定のメッセージウインドウ変数全消去[未指定でグローバル領域(-1)]
	void ClearAllMWValues();

	void Clear(); //ウインドウ全消去(リクエストも)

	//単行ウインドウ関連
	enum class SingleLineWinPos{Bottom=0,Top=1,Center=2};
	enum class SingleLineWinKind{Normal=0,WhiteStream=1,BlackStream=2};
	int CreateRequestSingleLineWindowByString(const std::string&, int,SingleLineWinPos,SingleLineWinKind); //テキストを指定して単行ウインドウを作成(テキスト、表示時間、位置、種類)

	//会話ウインドウ関連(DeleteRequest内でも関連処理を行っているので注意)
	enum class TalkWinPos{Bottom=0,Top=1};
	int CreateTalkWindow(int,TalkWinPos); //会話ウインドウ作成(テキスト番号,位置)
	int DeleteTalkWindow(); //会話ウインドウ削除
	bool IsTalkWindowActive()const; //会話ウインドウがアクティブ化取得
	int GetLastSelectedIndex()const; //最後に選択された選択肢のインデックス(まだ選択されていない場合は-1を返す)
private:
	MessageWindowManager();
	int LoadMessageFromFile(); //ファイルからメッセージ読み込み
	int id_counter;
	std::unique_ptr<ObjectHolderTemplate_WithID<int,MessageWindow>> mess_window_holder;
	std::map<int, std::shared_ptr<std::vector<std::string>>> messages;
	std::unordered_map<int,std::map<std::string, std::string>> global_value; //メッセージウインドウグローバル変数
	Vector2D<int> game_draw_size; //ゲームの描画サイズ

	int _normal_string_color_handle; //標準の文字色ハンドル
	int _normal_font_handle; //標準のフォントハンドル
	int _normal_name_font_handle; //標準の名前表示用フォントハンドル

	std::unordered_map<std::string, std::shared_ptr<SpeakerData>> _speaker_list; //話者リスト

	//会話ウインドウ関連
	struct TalkWIndowInfo{
		int window_id = -1;
		mutable int last_selected_index = -1;
	};
	TalkWIndowInfo _current_talk_win;
};

