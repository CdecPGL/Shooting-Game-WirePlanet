#pragma once

#include<string>
#include<memory>
#include"DxLib.h"

class CSVLoader;
class DataScript;
struct lua_State;
struct ActionData;

namespace FileLoadUtil{
	//ロードしたデータをどうするか(Remain:残す,Unload:破棄,Set:設定に従う)
	enum class LoadedData{ Remain, Unload, Set };
	static LoadedData loaded_data = LoadedData::Set; //ロードしたデータをどうするか(デフォルトでは設定に従う)
	void SetLoadedDataTreat(LoadedData);
	//ファイルロードマネージャからグラフィックハンドル作成。使用法はLoadGraphと同じ。(ファイル名、読み込み後にデータを破棄するかのフラグ)
	int CreateGraphFromFLM(const std::string&, bool = true);
	//ファイルロードマネージャから分割グラフィックハンドル作成。使用法はLoadDivGraphと同じ。(ファイル名、総数、横枚数、縦枚数、分割された画像の幅、高さ、ハンドルを保存するバッファ、読み込み後にデータを破棄するかのフラグ)(動作未確認)
	int CreateDivGraphFromFLM(const std::string&, int, int, int, int, int, int*, bool = true);
	//ファイルロードマネージャからサウンドハンドル作成。使用法はLoadSoundMemと同じ。(ファイル名、読み込み後にデータを破棄するかのフラグ)
	int CreateSoundMemFromFLM(const std::string&, bool = true);
	//ファイルロードマネージャからピクセルシェーダーハンドル作成。使用法はLoadPixelShaderと同じ。(ファイル名、読み込み後にデータを破棄するかのフラグ)
	int CreatePixelShaderFromFLM(const std::string&, bool = true);
	//ファイルロードマネージャからCSVファイルを読み込む(読み込み先のCSVLoaderクラスポインタ、ファイル名、読み込み後にデータを破棄するかのフラグ)
	int LoadCSVFromFLM(CSVLoader*, const std::string&, bool = true);
	//ファイルロードマネージャからデータスクリプトを読み込む(読み込み先のDataScriptクラスポインタ、ファイル名、読み込み後にデータを破棄するかのフラグ)
	int LoadDataScriptFromFLM(DataScript*, const std::string&, bool = true);
	//ファイルロードマネージャからLuaファイルを読み込む(読み込み先のlua_Stateポインタ、ファイル名、読み込み後にデータを破棄するかのフラグ)
	int LoadLuaScriptFromFLM(lua_State*, const std::string&, bool = true);
	//ファイルロードマネージャからActionDataを読み込む(読み込み先のlua_Stateポインタ、ファイル名、読み込み後にデータを破棄するかのフラグ)
	int LoadActionDataFromFLM(ActionData*, const std::string&, bool = true);
}
