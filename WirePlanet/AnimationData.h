#pragma once
#include "DxLib.h"
#include <unordered_map>
#include <string.h>
#include <string>
#include<memory>

class DataBlock;

using namespace std;

/*アニメーションファイルについて
形式は二つ。拡張子はganimが望ましい。Version2以上、Version1は読み込み前に設定する。デフォルトではV2以上
1.AnimFile_v1のLuaを用いた方式(Version1)※AnimLibV5では動作未検証---対応破棄---
初めに”size=**”でアニメサイズを指定。縦横は等しくないといけない。
次に"ANIMATION_DATA_UNIT_NUM=**"でアニメーションデータユニットの数を指定。
あとはアニメーションデータユニットを定義していく。
(n=0~**-1)Σ("
adun_tag="タグ名";
adun_graphic_filename="画像ファイルのパス";
adun_interval=アニメーションにおける画像切り替えの間隔;
adun_repeat_flag=繰り返すか;
")

2.AnimLib_v2のDatScriptを用いた方式(version2)。DataScriptの文法に従う。(Version2)
特殊ブロック(一つでもないとエラー)
version{アニメーションライブラリのバージョン(2.0);}
size{幅,高さ;}
graph_pass{
//画像ファイルのパス。複数指定可能。上から0,1,2…と番号が振られる。
aaa.png;
bbb.png;
…
}
アニメーション定義ブロック
タグ名{
interval,アニメ切り替え間隔(フレーム)(省略で1);
repeat,繰り返すかどうか(0or1)(省略で0);
//ここから各フレームの画像上での位置(1フレームから順番に)(x,yブロック座標(指定したサイズの)、画像番号[省略で0])
FrameDefS;
0,0,0;
1,0,0;
2,0,0;
…
FrameDefE;
}

3.AnimLib_v2のDatScriptV5を用いた方式(version3)。DataScriptの文法に従う。(Version5)
特殊ブロック(一つでもないとエラー)
version{アニメーションライブラリのバージョン(3.1);}
size{デフォルト幅,高さ;}
graph_pass{
//画像ファイルのパス。複数指定可能。上から0,1,2…と番号が振られる。任意で整数の番号を付けられる。つける場合はすべてにつける必要がある。省略すると上から(0,1,2……と振られる)
aaa.png[,画像番号];
bbb.png[,画像番号];
…
}
アニメーション定義ブロック
animation{
	タグ名{
		interval,アニメ切り替え間隔(フレーム)(省略で1);
		repeat,繰り返すかどうか(0or1)(省略で0);
		size,幅,高さ;(省略でデフォルト値)
		//フレーム情報。各フレームの画像上での位置(1フレームから順番に)(x,y座標(指定したサイズの)、画像番号[省略で0])
		frame{
			[AutomaticGeneration,[画像番号];] //これを指定すると、画像番号で指定した画像幅から自動でフレームを指定する。この時これ以降のパラメータは無視されるので注意。
			[coordinates,pixcel or block = block;] //位置の定義法。(pixeclでピクセル単位、blockでブロック単位。デフォルトではblock)
			0,0,0;
			1,0,0;
			2,0,0;
			…
		}
	}
	タグ名{
		……
	}
	……
}
4.DatScript標準データ構造v1を用いた方式(version4)。DataScript_v5の文法に従う。
データIDはganim。
定義はv3.1に従う。

*/
namespace AnimLib{

	//アニメーションデータユニット
	struct AnimationDataUnit{
		//コンストラクタ(タグ,元画像ハンドル,間隔,繰り返し,サイズ)
		AnimationDataUnit() = default;
		AnimationDataUnit(const string, int, int, bool, int);
		~AnimationDataUnit(); //デストラクタ
		string tag; //タグc
		vector<int> ghandle; //画像ハンドル(0は元画像)c
		int num; //アニメーション枚数c
		int size_x,size_y; //画像サイズ
		int interval; //画像変更間隔(フレーム単位)(0でアニメーションしない)c
		bool repeat_flag; //trueで繰り返すc
//		std::unordered_map<std::string,std::vector<std::vector<std::string>>> additionnal_data;
		//フレーム情報
		struct FrameInfo{
			int graph_num = 0;
			int pos_x = 0;
			int pos_y = 0;
		};
		std::vector<FrameInfo> frame_info;
	};

	//アニメーションデータ
	struct AnimationData{
		AnimationData(); //からのAnimationDataを作成
		//デストラクタ
		~AnimationData();
		//読み込んだファイル名の確認(trueで一致)
		bool CheckFileName(string)const;

		string animfilename; //読み込んだファイル名
		std::vector<std::unique_ptr<AnimationDataUnit>> animationdataunit; //アニメーションデータc
		int num; //登録されたアニメーションの数c
		std::vector<int> _ghandles; //画像ハンドル

		void SetGhandlesToAnimDataUnit(); //アニメデータユニットに画像ハンドルをセットする
	};
};
