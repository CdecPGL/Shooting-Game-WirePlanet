#pragma once
/**アニメーションクラス
*Version1.2 2013/12/27
*Version2.0 2014/1/22 Animationクラスの公開関数自体はほぼ変更なし。AnimationDataAdminを全て見直し。非シングルトンに変更。できるだけ生ポインタを使わないよう変更。AnimationライブラリをAnimLib名前空間に配置。AnimationDataAdminへのアクセス関数を配置Animationクラスに追加。Animationにデフォルトコンストラクタを追加
*Version2.1 2014/1/27 DxLibrary3Das2Dを使った描画関数追加
*Version2.2 2014/2/12 FreeXYExtDraw()を追加
*Version2.3 2014/5/26 アニメーションデータを後からセットする関数SetAnimationData()追加。再読み込み修正。AnimationDataAdmin修正。Animation::CheckAnimationPlaying(),GetAnomatopnTagList()追加。
*Version3.0 2014/5/29 アニメーションファイル形式V2を追加。詳細はAnimationData.h
*Version4.0 2014/7/22 アニメーションファイル形式V3を追加。詳細はAnimationData.h
*Version5.0 2014/9/04 バイナリファイルからのアニメーションファイル読み込みに対応。すでに読み込まれた画像ハンドルを参照してのアニメーションファイル作成を可能にした。アニメーションデータ定義でピクセル単位の指定が可能に。画像の保持、読み込み方法変更。
	AnimationdataAdmin::GetAnimationDataPointerの引数に自動読み込みフラグ追加。AnimDataV2以上で縦横のサイズが異るアニメに対応。
*Version5.1 2014/9/06 AnimationDataManagerの拡張子自動追加機能を廃止
*Version5.2 2014/12/18 DataScriptからの読み込みに対応。DSからの読み込み時はバージョンを自分で指定する必要がある。AnimationDataAdminをシングルトン化。それに伴いAnimationクラス内のadaを削除。ライブラリリンクのpragmaを追加
*Version6.0 2014/12/21 DataScript標準データ構造に対応したバージョンV4を追加。V2以上で先頭にV1との識別を入れる必要がなくなった。V1よ利用する場合は読み込み前にADAでバージョンを設定する。
*Version6.1 2014/12/23 V3,4でフレームデータの自動定義に対応
*Version6.2 2014/12/28 マネージャーに登録せずにアニメデータを読み込み可能に
*Version6.3 2014/12/28 boostlibraryを1.57.0に変更。v1を切り捨て
*Version6.3.1 2015/1/9 AnimationクラスコンストラクタでAnimationDataがセットされないバグを修正
*Version6.4 2015/1/10 フレームの開始番号を1から0に変更
*Version6.5 2015/2/27 任意の画像を指定可能に
*Version6.6 2015/3/5 DxLibrary3a2関連を削除
*Version6.7 2015/3/20 Animation::Drawの引数に拡大率,反転フラグを追加。Vector2D<double>版追加。GetAnimationDataPointerの自動読み込みフラグの初期値をfalseに変更、フラグオフ時のバグ修正
*/

#include "AnimationDataAdmin.h"
#include <vector>
#include <string.h>
#include <string>
#include "Vector2D.h"

#define ANIMATION_DRAW_CENTER_LEFT 1
#define ANIMATION_DRAW_CENTER_RIGHT 2
#define ANIMATION_DRAW_CENTER_UP 4
#define ANIMATION_DRAW_CENTER_DOWN 8

/**アニメーションライブラリ名前空間
*/
namespace AnimLib{
	/**アニメーションクラス
	*/
	class Animation
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		Animation();
		/**
		* @brief コンストラクタ
		* @param アニメーションデータ名
		*/
		Animation(const std::string&);
		/**
		* @brief コンストラクタ
		* @param アニメーションデータ
		*/
		Animation(std::shared_ptr<const AnimationData>);
		/**
		* @brief デストラクタ
		*/
		virtual ~Animation();
		/**
		* @brief アニメーションデータセット
		* @param アニメーションデータ名
		* @return -1で失敗　０で成功
		*/
		int AttachAnimationData(const std::string&);
		/**
		* @brief アニメーションデータセット
		* @param アニメーションデータ
		* @return -1で失敗　０で成功
		*/
		int AttachAnimationData(std::shared_ptr<const AnimationData>);
		/**
		* @brief アニメーションの更新(更新しても現在の各データを取得しないと反映されない)
		*/
		virtual void Update();
		/**
		* @brief 現在の画像を取得
		* @return 画像ハンドル
		*/
		int GetCurrentGraph() const;
		/**
		* @brief 現在の画像のサイズを取得(サイズを格納する変数のポインタ)
		*/
		void GetCurrentGraphSize(int*,int*);
		/**
		* @brief 現在のフレームを取得
		* @return 現在のフレーム(0-)。-1でアニメーション未再生
		*/
		int GetCurrentFrame() const;
		/**
		* @brief 現在のアニメーションのタグが引数と一致するか確認
		* @param 比較するタグ
		* @return 一致でtrue
		*/
		bool CheckCurrentAnimationTag(const std::string&) const;
		/**
		* @brief 現在のアニメーションのタグを取得する
		* @return タグ(無かったらNone)
		*/
		string GetCurrentAnimationTag() const;
		/**
		* @brief タグを用いて任意のアニメーションを選択しスタート
		* @param タグ
		* @return 0で成功、-1で失敗
		*/
		virtual int StartAnimationByTag(const std::string&);
		/**
		* @brief タグを用いて任意のアニメーションを選択しスタート(Loop指定版)
		* @param タグ
		* @return 0で成功、-1で失敗
		*/
		virtual int StartAnimationByTag(const std::string&,bool);
		/**
		* @brief アニメーションの停止
		*/
		void StopAnimation();
		/**
		* @brief 状態をリセット
		*/
		void Reset();
		/**
		* @brief アニメーションを一コマ進める
		* @return 0で成功、-1で失敗
		*/
		virtual int ProgressAnimation();
		/**
		* @brief 描画
		* @param 座標x,y,角度,拡大率,反転フラグ,(中心座標指定:ANIMATION_DRAW_CENTER_*_*を使う。デフォルトでは中心)
		*/
		void Draw(double, double, double,double,bool, char = 0);
		/**
		* @brief 描画
		* @param 座標,角度,拡大率,反転フラグ,(中心座標指定:ANIMATION_DRAW_CENTER_*_*を使う。デフォルトでは中心)
		*/
		void Draw(const Vector2D<double>& pos, double angle, double ext,bool rf, char c = 0){ Draw(pos.x, pos.y, angle, ext,rf, c); }
		/**
		* @brief サイズを指定して描画
		* @param 座標x,y,角度,サイズ,(中心座標指定:ANIMATION_DRAW_CENTER_*_*を使う。デフォルトでは中心)
		*/
		void FreeSizeDraw(double, double, double, int, char = 0);
		/**
		* @brief 縦横拡大率を指定して描画
		* @param 座標x,y,角度,拡大率x,y,(中心座標指定:ANIMATION_DRAW_CENTER_*_*を使う。デフォルトでは中心)
		*/
		void FreeXYExtDraw(double, double, double, double,double, char = 0);
		/**
		* @brief アニメーションデータのをアニメーションファイル名からセット(互換用)
		* @param アニメーションファイル名
		* @return 0成功 -1失敗
		*/
		int SetAnimationData(const std::string&);
		/**
		* @brief アニメーション再生中か確認
		* @return false再生していない true再生中
		*/
		bool CheckAnimationPlaying()const;
		/**
		* @brief アニメーションタグリストを取得
		* @param リストを格納するvector
		*/
		void GetAnimationTagList(std::vector<std::string>*)const;
		/**
		* @brief アニメーションタグリストを取得
		* @return タグリストを格納したvector
		*/
		std::vector<std::string> GetAnimationTagList()const;
	private:
		shared_ptr<const AnimationData> animationdata; //アニメーションデータのポインタ
		int counter; //アニメーション用カウンタ
		int frame; //アニメーションのフレーム
		int current_anim; //現在のアニメーション
		bool _roop_flag;
		bool _playing;
	};
};
