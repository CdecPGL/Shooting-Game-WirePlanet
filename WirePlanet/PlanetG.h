#pragma once
#include "Planet.h"
#include"boost/logic/tribool.hpp"
//#include"Animation.h"

namespace AnimLib{
	class Animation;
}

class PlanetG :
	public Planet
{
public:
	//コンストラクタ(元画像名,質量、摩擦、半径、分割数)
	PlanetG(const std::string&,double,double,double,double);
	~PlanetG();
	int Init()override;
	void Update()override;
	/*void Reset()override;*/
private:
	//本体描画関係
	std::string _origin_graph_name; //元画像名
	int ghandle_origine; //planet元画像ハンドル
	int _texture_ghandle; //テクスチャ画像ハンドル
	VERTEX3D* _vtxs; //頂点データ
	unsigned short* _p_indxs; //polygonを構成する頂点データ
	Vector2D<int> _g_size;  //画像サイズ
	Vector2D<int> _t_size; //テクスチャサイズ
	int _v_separation; //縦分割数
	int _polygon_num; //ポリゴン数
	int _vertex_num; //頂点数
	//コア関連
	std::unique_ptr<AnimLib::Animation> _core_anim;
	//コアプロテクタ関連
	std::unique_ptr<AnimLib::Animation> _core_prtctr_anim;

	void DrawBody()const override;

	static bool is2Pow(int); //2の塁上か調べる
	static int getMin2Pow(int); //指定された数字より大きくて最小の2の累乗を取得する
};

