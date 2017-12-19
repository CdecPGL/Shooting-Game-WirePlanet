#pragma once

#include<string>
#include<unordered_map>
#include"DxLib.h"
#include"Vector2D.h"

/*エフェクト対応一覧
-1:エフェクト無し
0:アラートエフェクト(Alert)
1:全画面画像表示(FullScreenGraph)
*/

class GameScreenEffect
{
public:
	GameScreenEffect();
	~GameScreenEffect();
	int Init();
	void Update();
	void Draw();

	int StartEffect(const std::string&, const std::string& = "NULL");
	void StopEffect();
	bool IsEffectPlaying()const;
	std::string GetCurrentEffectName()const;
private:
	struct CurrentEffectInfo{
		std::string name;
		int num;
		std::string param;
	};
	CurrentEffectInfo _current_effect_info;
	std::unordered_map<std::string, int> _id_num_map;
	int inc_cnt;
	Vector2D<int> _draw_area_size;
};
