//version 1.0 2013/12/15
//Version 2.0 2014/02/21 今まで通りのシングルトン型に加え、ノンシングルトン型を追加
//Version 2.1 2014/02/22 シングルトン型のコピーコンストラクタ禁止
//Version 2.2 2014/02/24 GetPushKey()に連射カウンタをリセットするかの選択フラグを追加
//Version 3.0 2014/03/17 マウス入力関連の関数を追加
//Version 4.0 2014/05/07 パッド関連の関数追加(DX_INPUT_KEY_PAD1(1P)のみ)
//Version 4.0a 2016/4/30 消失した実装の再現

#ifndef DEF_KEY_H
#define DEF_KEY_H

#include "DxLib.h"
#include<unordered_map>
#include <functional>

#define CUSTOM_INPUT_A 1 //カスタム入力A
#define CUSTOM_INPUT_B 2 //カスタム入力B
#define CUSTOM_INPUT_X 3 //カスタム入力X
#define CUSTOM_INPUT_Y 4 //カスタム入力Y
#define CUSTOM_INPUT_UP 5 //カスタム入力上
#define CUSTOM_INPUT_DOWN 6 //カスタム入力下
#define CUSTOM_INPUT_LEFT 7 //カスタム入力左
#define CUSTOM_INPUT_RIGHT 8 //カスタム入力右
#define CUSTOM_INPUT_L 9 //カスタム入力L
#define CUSTOM_INPUT_R 10 //カスタム入力R
#define CUSTOM_INPUT_ZL 11 //カスタム入力ZL
#define CUSTOM_INPUT_ZR 12 //カスタム入力ZR
#define CUSTOM_INPUT_START 13 //カスタム入力START

//キー入力クラス(非シングルトン)

class nsKey {
private:
	char key[256]; //キーの入力情報を格納
	char pkey[256]; //前回の更新時のキー入力情報
	bool burstflag[256]; //連射が有効か(trueで有効)
	mutable int counter[256]; //連射用カウンター
	int interval[256]; //連射間隔

	int mouse_input; //マウスの入力情報を格納
	int pmouse_input; //前回の更新時のマウスの入力情報

	int UpdateKey(); //マウスとキー入力の更新(0:成功 -1:失敗)

	int pad_input; //パッドの入力
	int ppad_input; //前回のパッドの入力

	int UpdateJoyPad(); //パッドの入力更新

	int dir_input; //方向入力
	int pdir_input; //前回の方向入力
	int UpdateDirectionInput(); //方向入力の更新


	bool GetJoypadState(int); //パッドのボタン入力情報取得
	bool GetJoypadPush(int); //パッドのボタンが押されたか

	bool GetDirState(int);
	bool GetDirPush(int);

	//カスタムインプット対応マップ<CUSTOM_INPUT_*,<State,Pushed>>
	std::unordered_map<int, std::pair<std::function<bool()>, std::function<bool()>>> custom_input_map; 

public:
	nsKey(); //コンストラクタ
	virtual ~nsKey(); //デストラクタ
	int Update(); //状態更新
	char GetKeyStatus(int) const; //特定のキー入力情報の取得(キーが押されている間1を返す)
	char GetKeyPush(int, bool = true) const; //キーが押された瞬間だけ1を返す[キー番号、連射カウンターをリセットするか(デフォルトではtrue)](連射が有効だったら一定間隔ごとに1を返し、counterをリセット)
	bool GetBurstFlag(int) const; //特定キーの連射が有効か
	void SetBurstFlag(int, bool); //特定キーの連射の有効無効をセット
	void SetInterval(int, int); //特定キーの連射間隔のセット

	bool GetMouseButtonState(int); //特定ボタンの入力情報の取得
	bool GetMouseButtonPush(int); //特定ボタンが押された時だけtrueを返す。(引数はDxLiblaryのMOUSE_INPUT_LEFTなど)
	void GetMousePosition(int&, int&); //マウスの座標を取得

	//カスタム設定入力
	//カスタム入力状況
	bool GetCustomInputState(int);//
	//カスタム入力押下
	bool GetCustomInputPush(int);//
	//デフォルトのキー割り当てを設定
	void SetDefaultCustomMap();
};

//シングルトン版
class Key : public nsKey{
private:
	Key();
	Key(const Key&);
public:
	~Key();
	static Key& GetInstance(); //インスタンスの取得
};


#endif DEF_KEY_H