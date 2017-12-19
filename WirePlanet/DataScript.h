/**
* 自作データスクリプト操作クラス。
* 2014/1/26 Version1.0
* 2014/2/1 Version 2.0 スクリプトのファイルへの保存を実装
* 2014/2/4 Version 2.0.1 ファイル末尾の}が二重になってしまう不具合を暫定的に回避
* 2014/3/31 Version 3.0 構文解析にboost::regexを使用。既知の不具合解消。
* 2014/4/1 Version 3.0.1 ヘッダファイルのusing namespace std;を削除。可能なところにconst,参照を設定。
* 2014/5/2 Version 3.1 コメント機能実装(//でコメント行)
* 2014/5/26 Version 3.2 文字列からの読み込みに対応。
* 2014/6/22 Version 3.3 ムーブセマンティクスを使用し高速化?
* 2014/7/18 Version 4.0 データブロックの多重構造に対応(書き込み系の機能は未実装)
* 2014/7/22 Version 4.1 行はじめの水平タブ文字は除去するよう変更
* 2014/8/30 Version 4.2 バイナリデータからの読み込みに対応。SJISの2バイトコードを考慮した処理に変更。カッコの数が合わない時にstd::domain_errorを投げるよう変更。
* 2014/12/17 Version 5.0 DataBlock大幅変更。代入演算子追加。子ブロックの名前重複を許可。データの編集に対応。プロジェクト設定でOriginalScriptLibrary.libを設定しなくてもよいよう変更(pragma comment)。プリプロセス処理機能追加
* 2014/12/21 Version 5.1 DataScript標準データ構造導入
* 2015/1/8 boostlibraryを1.57.0に変更。luaをスタティックリンクに変更

* 未設定では拡張子は追加されない。
* 文法
* Version 1,2,3
* データ名{データ}
* ,データの区切り
* ;データ改行
* //コメント行(行の途中からは不可能。必ず行初めから)
* 位置指定は行(縦)列(横)の順なので(x,y指定と逆)注意
* データの列の要素数はすべて合わせる(エラーにならないが推奨)
* Version 4
* 複数ブロックに対応
* データブロック:データブロック名{データまたはデータブロック}
* データ:行で構成。行は;で句切る
* 行:データで構成。データは,で句切る。(行はじめの水平タブ文字は除外されます)
* //コメント行(行の途中からは不可能。必ず行初めから)
*------V4実装に伴うV1-3用関数の互換性
*V1-3までのファイルを読込み可能。
*GetDataAll,GetDataLine,GetDataAssigned,GetDataName,GetSize,CheckDataExistenceは使用可能
*ChangeDataAll,ChangeDataLine,ChangeDataAssigned,DeleteData,ChangeSize,CreateDataは使用不可能(-1を返す)
* Version 5
* V4に加えて同名ブロックに対応
* プリプロセス処理機能追加。SetUseMacroでフラグを立てると使用可能。上から実行されます。アルファベットのみ対応
* <<プリプロセス処理(PPP)>>
* PPPの定義は#で始まり;で終わる。引数同士は,で句切る。ファイルの最初に連続して記述する必要がある。それ以外は通常の文字列として処理される。
* <<PPP命令一覧>>
実装見送り(* PPP無効化命令(これ以降のプリProcess処理をスキップする):INVALIDATE_PPP
* PPP有効化命令(INVALIDATE_PPPで無効化されていた場合。これ以降処理を実行する):VALIDATE_PPP)
* 置換命令:REPLACE 置換対象文字列 置換後文字列(記述順に置換される。置換後の文字列も置換対象になるので注意)


*/

#ifndef DEF_DATASCRIPT_H
#define DEF_DATASCRIPT_H

#include"DataBlock.h"
#include<string>
//#include<map>
#include<unordered_map>

//DSVer1_4を指定しても内部でDSVer1_5に置き換えられます。
enum class DataScriptVersion{ DSVer1_4,DSVer1_5};

class DataScript
{
public:
	/**
	* @brief コンストラクタ
	*/
	DataScript();
	/**
	* @brief コンストラクタ
	* @param 拡張子(.は不要)
	*/
	DataScript(const std::string&);
	/**
	* @brief デストラクタ
	*/
	~DataScript();
	/**
	* @brief スクリプトからデータ読み込み
	* @param filename スクリプトファイル名 (add_flag 追加読み込みフラグ。trueで現在のデータを消さずに追加読み込み。デフォルトはfalse。)
	* @return 成功したら0。失敗したら-1。
	*/
	int LoadDataFromScript(const std::string&,bool = false);
	/**
	* @brief 文字列からデータ読み込み(改行,水平タブを無視しない不具合あり)
	* @param 文字列 (add_flag 追加読み込みフラグ。trueで現在のデータを消さずに追加読み込み。デフォルトはfalse。)
	* @return 成功したら0。失敗したら-1。
	*/
	int LoadDataFromString(const std::string&, bool = false);
	/**
	* @brief バイナリデータからデータ読み込み
	* @param top_ptr バイナリデータの先頭アドレス size バイナリデータサイズ (add_flag 追加読み込みフラグ。trueで現在のデータを消さずに追加読み込み。デフォルトはfalse。)
	* @return 成功したら0。失敗したら-1。
	*/
	int LoadDataFromBynary(const void*,unsigned int, bool = false);
	/**
	* @brief 指定名のスクリプトにデータ保存
	* @param filename スクリプトファイル名 (add_flag 追加読み込みフラグ。trueで書き込み先のデータを消さずに追加書き込み。デフォルトはfalse。)
	* @return 成功したら0。失敗したら-1。
	*/
	int SaveDataToScript(const std::string&, bool = false)const;
	/**
	* @brief 読み込んだスクリプトにデータ保存
	* @param filename スクリプトファイル名 (add_flag 追加読み込みフラグ。trueで現在のデータを消さずに追加読み込み。デフォルトはfalse。)
	* @return 成功したら0。失敗したら-1。
	*/
	int SaveDataToLoadedScript()const;
	/**
	* @brief 拡張子設定
	* @param file extention 拡張子名(.は不要)
	*/
	void SetExtension(const std::string&);
	/**
	* @brief 指定名のデータ全取得(V1-3)
	* @param dataname 取得したいデータ名 data:データを格納するstd::vector
	* @return 0:成功 -1指定されたデータが存在しない
	*/
	int GetDataAll(const std::string&, std::vector<std::vector<std::string>>&)const;
	/**
	* @brief 指定名のデータ行取得(V1-3)
	* @param dataname 取得したいデータ名 line 行番号(0~) data:データを格納するstd::vector
	* @return 0:成功 -1:指定されたデータが存在しない -2:行が不正
	*/
	int GetDataLine(const std::string&, int, std::vector<std::string>&)const;
	/**
	* @brief 指定名のデータ位置指定取得(V1-3)
	* @param dataname 取得したいデータ名 line,raw:指定行列 data:データを格納するstring
	* @return 0:成功 -1:指定されたデータが存在しない -2:行列が不正
	*/
	int GetDataAssigned(const std::string&, int, int, std::string&)const;
	/**
	* @brief 指定名のデータブロック変更(V1-3)
	* @param dataname 変更したいデータ名 data:データ
	* @return 0:成功 -1指定されたデータが存在しない
	*/
	int ChangeDataAll(const std::string&, const std::vector<std::vector<std::string>>&);
	/**
	* @brief 指定名のデータ行変更(V1-3)
	* @param dataname 取得したいデータ名 line 行番号(0~) data:データ
	* @return 0:成功 -1:指定されたデータが存在しない -2:行が不正
	*/
	int ChangeDataLine(const std::string&, int, const std::vector<std::string>&);
	/**
	* @brief 指定名のデータ位置指定変更(V1-3)
	* @param dataname 取得したいデータ名 line,raw:指定行列 data:データ
	* @return 0:成功 -1:指定されたデータが存在しない -2:行列が不正
	*/
	int ChangeDataAssigned(const std::string&, int, int, const std::string&);
	/**
	* @brief 指定名のデータ消去(V1-3)
	* @param 削除したいデータ名
	* @return 0:成功 -1:失敗
	*/
	int DeleteData(const std::string&);
	/**
	* @brief データ名リストを取得(V1-3)
	* @return データ名リスト
	*/
	std::vector<std::string> GetDataName()const;
	/**
	* @brief 指定名のデータのサイズ取得(行×列)(V1-3)
	* @param dataname:データ名 line,raw:サイズ格納先
	* @return 0:成功 -1:指定されたデータが存在しない
	*/
	int GetSize(const std::string&,int&,int&)const;
	/**
	* @brief 指定名のデータのサイズ変更(行×列)(V1-3)
	* @param dataname:データ名 line,raw:サイズ (blank:空白を埋める文字列を指定)
	* @return 0:成功 -1:指定されたデータが存在しない
	*/
	int ChangeSize(const std::string&, int, int,std::string = "");
	/**
	* @brief 指定名のデータ作成(V1-3)
	* @param dataname:データ名 line,raw:サイズ (blank:空白を埋める文字列を指定)
	* @return 0:成功 -1:すでに存在する
	*/
	int CreateData(const std::string&, int, int,std::string = "");
	/**
	* @brief 確認したいデータ指定名(V1-3)
	* @param dataname:データ名
	* @return true:存在する false:存在しない
	*/
	bool CheckDataExistence(const std::string&)const;

	/**
	* @brief ルートデータブロックを取得(const版)
	* @return ルートデータブロックの参照
	*/
	std::shared_ptr<const DataBlock> GetRoot()const;
	/**
	* @brief ルートデータブロックを取得(非const版)
	* @return ルートデータブロックの参照
	*/
	std::shared_ptr<DataBlock> GetRoot();

	/**
	* @brief 読み込んだデータ削除
	*/
	void Clear();
	/**
	* @brief 読み込んだデータの表示(デバッグ用)
	*/
	void DrawData()const;
	/**
	* @brief プリプロセス機能を使用するか設定
	* @param true:使用する false:使用しない
	*/
	void SetPreProcessFlag(bool);
	/**
	* @brief 文字列の取得
	* @return 文字列
	*/
	std::string ToString()const;
	/**
	* @brief エラー文字列の取得
	* @return エラー文字列
	*/
	std::vector<std::string> GetErrorString()const;
	/**
	* @brief 処理・構文バージョンの設定
	* @param バージョン
	*/
	void SetVersion(DataScriptVersion);
	/**
	* @brief 現在設定されている処理・構文バージョンの取得
	* @return バージョン
	*/
	DataScriptVersion GetVersion()const;
	/**
	* @brief 動作テスト
	* @param ファイル名,読み込み回数
	* @return -1:エラー発生 0:正常終了
	*/
	int Test(const std::string&,int = 1);

	/**
	* @brief ルートデータブロックを取得(const版)(互換用)
	* @return ルートデータブロックの参照
	*/
	const DataBlock& GetRootDataBlock()const;
	/**
	* @brief ルートデータブロックを取得(非const版)(互換用)
	* @return ルートデータブロックの参照
	*/
	DataBlock& GetRootDataBlock();
private:
	std::string extension; //拡張子
//	std::map<std::string, std::vector<std::vector<std::string>>> data; //データ
	std::string script_name; //読み込んだスクリプト名
	std::vector<std::string> error_strings; //エラー文字列
	void SetErrorString(const std::string&); //エラー文字列セット
	DataScriptVersion version; //使用バージョン
	std::shared_ptr<DataBlock> root_data_block; //ルートデータブロック
	bool _preproc_valid_flag; //プリProcess機能有効化フラグ

	//ロード処理関数
	int process_load(std::string);
	//ロード前処理関数
	int pre_process_before_load(std::string&); //マクロなどのプリProcess処理(プリProcess命令は除去される)
	//データロード関数(引数はデータ文字列)
	std::unordered_map < DataScriptVersion, int(DataScript::*)(std::string&)> loaddata;
	//データを文字列化する関数
	std::unordered_map<DataScriptVersion, std::string(DataScript::*)()const> datatostring;
	//バージョン5の関数
	int LoadDataV1_5(std::string&);
	static std::shared_ptr<DataBlock> LoadDataBlock(const std::string&);
	std::string DataToStringV1_5()const;
	static std::string DataBlockToString(const DataBlock&,int = 0);

	//SJISのマルチバイト文字コードか確認する
	static bool SJISMultiCheck(unsigned char);
};

#endif