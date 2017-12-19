#pragma once

#include <vector>
#include <map>
#include <list>
#include <string>
#include "Vector2D.h"
#include"DxLib.h"
#include "SoundManager.h"
#include<memory>

struct SpeakerData;

//定数
//コマンド文字列(引数)
namespace{
	//const std::string CREATE_TEXT_WIN("TextWindow"); //テキストウインドウ作成(メッセージ(テキスト)番号[,動作=0(0:子として作る 1:独立して作る 2独立して作り自分を消す))
	const std::string CREATE_SLCT_WIN("SelectWindow"); //セレクトウインドウ作成(メッセージ(テキスト)番号[,動作=0(0:子として作る 1:独立して作る 2独立して作り自分を消す))
	//const std::string CREATE_INPT_WIN("InputWindow"); //入力ウインドウ作成(メッセージ(テキスト)番号[,動作=0(0:子として作る 1:独立して作る 2独立して作り自分を消す))
	const std::string READ_TEXT("ReadText"); //追加でテキストを読み込み(テキスト番号)
	const std::string CLOSE_WIN("Close"); //ウインドウを閉じる(0:自分のみ、1:子と自分、2:自分と親 3:親と自分と子)
	const std::string CHANGE_SPEAKER("ChangeSpeaker"); //話者変更(話者ID)
	const std::string CHANGE_EXPRESSION("ChangeExpression"); //表情変更(表情ID)
	const std::string WAIT_KEY_PUSH("WaitKeyPush"); //キー入力待機
	const std::string WAIT_TIME("WaitTime"); //一定時間待機(フレーム)
	const std::string CHANGE_DRAW_SPEED("ChangeDrawSpeed"); //一文字の表示速度変更([一文字の表示フレーム数]=Default)
	const std::string CLEAR("Clear"); //表示文字消去
	//条件分岐
	const std::string JUMP("Jump"); //指定のラベルにジャンプ。ラベルがなかったら無視(ラベル名)
	const std::string IF_EQUAL_JUMP("IfEqualJump"); //2つの引数の内容が等しかったら指定のラベルまでジャンプする。ラベルがなかったら無視(比較対象1,比較対象2,ジャンプ先ラベル)
	const std::string IFN_EQUAL_JUMP("IfnEqualJump"); //2つの引数の内容が異なったら指定のラベルまでジャンプする。ラベルがなかったら無視(比較対象1,比較対象2,ジャンプ先ラベル)
	//変数関係
	const std::string SET_MW_VALUE("SetVariable"); //メッセージウインドウグローバル変数(型はstring)セット(変数名&セットする内容[&セット先(G:グローバル領域orL:ローカル領域)=G])
	const std::string ADD_MW_VALUE("AddVariable"); //メッセージウインドウグローバル変数(型はstring)セット(変数名&加算値)
}

//コマンド文字列について
//コマンド識別子:\\バックスラッシュ(円記号半角)×2
//コマンド:定数の項参照
//コマンド引数:コマンド実行時のパラメータ。複数の時は&(アンド)で句切る
//文法:[コマンド識別子][コマンド](=)[コマンド引数]

//ラベルについて
//ラベル識別子:@@

//変数について(未完成。テキスト中の変数置換が未実装)
//変数識別子:<<,>>で囲む
//文字列中に変数があったらその内容に置換される。該当する変数がなかったら無視される
/*以下の変数はシステムで使用されます
SelectedIndex:選択ウインドウで選択された項目の伝達で使用
*/

//メッセージ(引数)
//***KeyPushed ***キーが押された(***=Up,Dpwn,Left,Right,Select,Cancel)(引数なし)
//ChildWindowDestructed 子ウインドウが破棄された(破棄されたウインドウのID)
//ParentWindowDestructed 親ウインドウが破棄された(破棄されたウインドウのID)
//struct MWMessage{
//	MWMessage(const std::string&, int...);
//	std::string message;
//	std::vector<int> param;
//};

class MessageWindow
{
public:
	MessageWindow(int, int = -1);
	virtual ~MessageWindow();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	//初期か(文字データ,位置,サイズ)
	virtual int Init(std::shared_ptr<const std::vector<std::string>>, Vector2D<int>, Vector2D<int>);
	//終了処理
	virtual int End(); //終了処理

	//取得関数
	int GetMyID()const; //自分のID取得
	int GetParentID()const; //親のID取得(-1で無し)
	int GetChildID()const; //子のID取得(-1で無し)
	Vector2D<int> GetPosition()const; //位置取得
	Vector2D<int> GetSize()const; //サイズ取得

	//設定関数
	void SetStringColorHandle(int);
	void SetFontHandle(int);
	void SetNameFontHandle(int);

	virtual int RecieveMessage(const std::string&, int = 0); //メッセージの受け取り関数(メッセージ、パラメータ//,結果//)(-1:無効なメッセージ)

	enum class FixedPosition{ None = 0, Left = 1, Right = 2, Top = 4, Bottom = 8 };
	void SetFixedPosition(FixedPosition);

protected:
	Vector2D<int> position; //ウインドウの左上座標
	Vector2D<int> size; //ウインドウのサイズ
	FixedPosition _fixed_pos; //固定位置
	int word_size = 16;
	std::shared_ptr <const std::vector<std::string>> strings; //文字列データ
	bool enable; //操作可能か
	SoundManager& sdm;
	int _string_col_handle; //文字色ハンドル
	int _font_handle; //フォントハンドル
	int _name_font_handle; //話者名用フォントハンドル
	std::shared_ptr<SpeakerData> _current_speaker;
	int _current_face_graph;
	int _word_draw_frame; //一文字の表示時間
	bool _waiting_key_push; //キー入力町中か
	int _wait_frame; //待機するフレーム(0で非待機)
	std::list<std::string> _draw_strings; //表示する文字列
	int _max_line; //表示可能行数(文字の大きさとウインドウの高さから自動で調節)
	int _line; //現在の行(0から)
	int _raw; //現在の列(0から)
	int _word_draw_cntr; //文字表示カウンターm,
	int _inc_cntr; //増加カウンタ

	void UpdateCounter();

	int DeleteRequestMWM(int = 0); //マネージャーに削除依頼(0(デフォルト):自分だけ削除 1:子も全て削除 2:親も子もすべて削除)
	void DrawWindow(); //ウインドウの描画
	void DrawStrings(); //文字列の描画
	void DrawFaceGraphic(const Vector2D<int>&, const Vector2D<int>&); //顔画像描画(左上相対座標、サイズ)

	void PushFrontDrawStrings(const std::string&); //表示文字列を先頭に追加
	void PushBackDrawStrings(const std::string&); //表示文字列を最後尾に追加

	//キーが押された時の処理
	virtual void UpKeyPushed(); //上キーが押された
	virtual void DownKeyPushed(); //下キーが押された
	virtual void LeftKeyPushed(); //左キーが押された
	virtual void RightKeyPushed(); //右キーが押された
	virtual void SelectKeyPushed(); //選択キーが押された
	virtual void CancelKeyPushed(); //キャンセルキーが押された

	//コマンド関連
	bool CheckCommand(const std::string&)const; //文字列がコマンド文字列か確認
	int ProcessCommand(const std::string&); //命令文字列の処理(0:通常コマンド実行 1:中断コマンド実行 -1:コマンド文字列でない -2:コマンド失敗 -3:無効なコマンド)
	std::string GetNextString(); //次の文字列取得(命令は実行して次に進む)

	//ラベル関連
	std::unordered_map<std::string, int> _label; //ラベルリスト(ラベル名、存在行)
	bool IsLabel(const std::string&)const; //ラベル化確認
	void AnalyzeLabel(); //ラベル解析
	int FindLabel(const std::string&)const; //ラベル検索。なかったら-1(自分の変数領域を探して、なかったらグローバル領域を探す)

	//変数関連
	std::string PermuteVariable(const std::string&); //与えられた文字列の変数をその内容に置換した文字列を返す。

	virtual void ResetStrings(std::shared_ptr<const std::vector<std::string>>); //派生クラスの仮想関数では必ず最後にこのクラスのResetStringsを呼ぶ

	static bool SJISMultiCheck(unsigned char); //S_JISに置いてマルチバイト文字のバイトかチェック
private:
	int ID; //メッセージウインドウID
	int pID; //親のID //-1でマネージャーが親
	int cID; //子のID //-1で子なし
	//	std::vector<MWMessage> message_pool; //受け取ったメッセージリスト

	std::map<std::string, void(MessageWindow::*)()> keypushmess_map; //キー入力系メッセージに対応するメンバ関数
	int ProcessKeyPushedMessage(const std::string&); //キー入力系メッセージ処理(0:キー入力処理実行 -1:メッセージに当てはまる処理が存在しない)※子クラス内のこの関数の先頭でこのクラスのこの関数を呼び出す
};
