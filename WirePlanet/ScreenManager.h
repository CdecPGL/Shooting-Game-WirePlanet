#pragma once

#include"DxLib.h"
#include<unordered_map>
#include<vector>
#include<string>
#include"Vector2D.h"
#include<array>

//レイヤーについて。レイヤーの数字が小さいほど奥に描画される。

class ScreenManager
{
public:
	ScreenManager(const ScreenManager&);
	~ScreenManager();
	static ScreenManager& GetInstance();
	int Init();
	int End();
	int ChangeScreenBufferNum(int); //スクリーンバッファ枚数変更(1以上)
	int PushScreen();
	void DrawScreen(); //スクリーン描画
	void SetCommonPixShader(int,int); //共通ピクセルシェーダーセット(PSハンドル、適応タイミング)
	void ResetCommonPixShader(); //共通ピクセルシェーダーリセット
	enum class ScreenEffect{ Broom };
	int ApplyEffectToTopScreen(ScreenEffect,int); //現在最前面に有るスクリーンにエフェクトを掛ける。(エフェクトの種類、強さ)
	struct PixShaderConstF{
		FLOAT4 vals[32];
		unsigned int size=0;
	};
	PixShaderConstF cmn_pix_shader_cnst_vals; //共通ピクセルシェーダに渡す定数
private:
	ScreenManager();
	std::vector<int> _scr_buf;
	int _current_buf_num; //現在描画対象のバッファナンバー
	Vector2D<int> _draw_area_size;
	//シェーダー関連
	std::pair<int, int> _cmn_ps; //共通ピクセルシェーダーデータ
	VERTEX2DSHADER vertex[4];
	void ApplyPixcelShader(int,int,int); //ピクセルシェーダを適用(元、先、シェーダ)
	void SetCmnPixShaderCnstVals(); //共通ピクセルシェーダに渡す定数をセット
};

