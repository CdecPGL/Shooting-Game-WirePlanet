#pragma once
//2013/10/30 Version 1.0 アクションファイルの読み込み関数を実装しアクションライブラリが完成
//2014/5/1 Version1.1 GetCurrentActionByTag(),CheckActionPlaying()関数追加。
//2014/6/16 Version1.2 StartActionByTagに繰り返し指定を追加
//2014/6/22 Version1.3 一部関数の引数をコンスト参照に変更
//2014/9/5 Version1.4 メモリ上のデータからのアクションデータ作成に対応。外部からの画像ハンドル参照読み込みに対応
#include "DxLib.h"
#include <vector>
#include <string.h>
#include <string>
#include<memory>
#include<unordered_map>

//判定の属性
const int Col_Atrb_Trrn(1); //地形(地形と衝突)
const int Col_Atrb_Atck(2); //攻撃
const int Col_Atrb_Dmge(4); //ダメージを受ける
const int Col_Atrb_Item(8); //アイテム(アイテム属性を持つとアイテム属性を持つ相手と衝突できます)

//当たり判定データ
struct HitAreaData{
	HitAreaData(int, int, int, int, int, int, int* = nullptr); //コンストラクタ(属性,相対座標x,y,半径,始まり,終わりフレーム,追加パラメタ)c
	int attribute; //種類
	int x, y; //相対的位置(-で上+で下)c
	int radius; //半径c
	int start_frame, end_frame; //当たり判定の始まり、終わりフレーム(1~num)c(0,0でアクション中ずっと有効)
	int additional_param[2]; //追加パラメータ(WirePlanetでは0:攻撃力 1:与える無敵時間)
};

//アクション用データ単位
struct ActionDataUnit{
	//コンストラクタ(タグ,元画像ハンドル,間隔,繰り返し,サイズ)
	ActionDataUnit(){};
	ActionDataUnit(const std::string, int, int, bool, int);
	~ActionDataUnit(); //デストラクタ
	const std::string tag; //タグc
	std::vector<int> ghandle; //画像ハンドル(0は元画像)c
	int num; //アニメーション枚数c
	int interval; //画像変更間隔(フレーム単位)(0でアニメーションしない)c
	bool repeat_flag; //trueで繰り返すc
	std::vector<HitAreaData*> hitarea; //当たり判定データc
};

//アクションデータ
struct ActionData{
	ActionData(); //コンストラクタ()
	explicit ActionData(const char*); //コンストラクタ(ファイル名)
	~ActionData();
	int size; //キャラのサイズ(ビット単位)c
	std::vector<ActionDataUnit*> data; //アクションデータ単位c
	//アクションファイル読み込み関数(ファイル名,書き込むアクションデータのポインタ)
	int ReadActionFileFromFile(const std::string&);
	//メモリからアクションファイル読み込み関数(データの先頭アドレス、データサイズ)
	int ReadActionFileFromMem(const void*,unsigned int);
	int num; //登録されたアクションの数c
	//画像ハンドルの参照
	using RefGHandlesType = std::unordered_map<std::string,int>;
	static RefGHandlesType _ref_ghandles;
	static bool _ghandle_ref_flag; //画像反動参照フラグ
private:
	ActionData(const ActionData&) = delete;
	const ActionData& operator = (const ActionData&) = delete;
	//分割済み文字列からアクションファイル読み込み関数(データ文字列郡)
	int ReadActionFileFromSeparatedString(std::vector<std::string>&&);
};

class ActionDataAdmin{
public:
	~ActionDataAdmin();
	ActionDataAdmin(const ActionDataAdmin&) = delete;
	static ActionDataAdmin& GetInstance();
	int Init();
	ActionData& GetActionData(const std::string&);
	void SetRefGHandles(ActionData::RefGHandlesType&&);
	void SetRefGHandles(const ActionData::RefGHandlesType&);
	void SetRefGhandleFlag(bool);
	void Clear();
private:
	ActionDataAdmin();
	std::unordered_map<std::string, std::unique_ptr<ActionData>> action_datas;
};

//アクションクラス
class Action
{
public:
	Action(const ActionData&); //コンストラクタ(アクションデータの参照)

	~Action(void); //デストラクタ
	//	Action(const Action& action); //コピーコンストラクタ
	void Update(); //アニメーションの更新(更新しても現在の各データを取得しないと反映されない)
	int GetCurrentGraph() const; //現在の画像を取得
	int GetCurrentFrame() const; //現在のフレームを取得(0でアニメーション終了済)
	bool CheckCurrentActionTag(const std::string&) const; //現在のアクションのタグが引数と一致するか確認(trueで一致)
	int StartActionByTag(const std::string&); //タグを用いて任意のアクションを選択しスタート
	int StartActionByTag(const std::string&, bool); //タグを用いて任意のアクションを選択しスタート
	int ProgressAnim(); //アクションを一コマ進める
	const std::string GetCurrentActionTag()const; //現在のアクションタグを取得。終了していたら"None"を返す
	bool CheckActionPlaying()const; //アクション再生中か調べる
	//当たり判定
	//	int GetCurrentHitAreaNum() const; //現在有効な当たり判定の数を得る
	int SetCurrentHitAreaToVector(std::vector<HitAreaData*>*) const; //HitAreaDataポインタのvectorに現在有効な当たり判定データを入れる
	int GetGraphSize()const; //画像サイズ所得
private:
	int counter; //アニメーション用カウンタ
	int frame; //アニメーションのフレーム
	int current_anim; //現在のアニメーション
	const ActionData& actiondata;
	//	int current_hitarea_num; //現在の当たり判定の数
	bool repeat;
};