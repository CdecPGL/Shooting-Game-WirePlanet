#pragma once

#include<string>
#include<unordered_map>
#include"DxLib.h"
#include"Vector2D.h"

/*�G�t�F�N�g�Ή��ꗗ
-1:�G�t�F�N�g����
0:�A���[�g�G�t�F�N�g(Alert)
1:�S��ʉ摜�\��(FullScreenGraph)
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
