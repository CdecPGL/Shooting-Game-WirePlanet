#pragma once

#include "DxLib.h"
#include<memory>
#include<string>
#include<unordered_map>
#include"Vector2D.h"

class CharacterBase;
class PlayerMeter;

class InformationArea
{
public:
	InformationArea();
	~InformationArea();
	int Init();
	void Update();
	void Draw();

	void SetPoint(unsigned long long int);

	void SetLeftAreaWidth(int);
	void SetRightAreaWidth(int);
	int ResistPlayer(const std::string&,std::shared_ptr<CharacterBase>,const std::string& ="NULL"); //プレイヤーの登録(プレイヤーID、プレイヤーポインタ[、アイコン画像])
	int UnesistPlayer(const std::string&); //プレイヤーの登録解除(プレイヤーID)

	int ChangePlayer(const std::string&); //プレイヤー交代
private:
	Vector2D<int> _draw_area_size;

	int left_area_width; //左エリアの幅
	int right_area_width; //右エリアの幅

	struct PlayerInfo{
		PlayerInfo();
		PlayerInfo(std::shared_ptr<CharacterBase>);
		std::unique_ptr<PlayerMeter> meter;
		std::shared_ptr<CharacterBase> player;
		void Update();
	};
	std::unordered_map<std::string, std::unique_ptr<PlayerInfo>> _players; //プレイヤーリスト
	std::pair<std::string,PlayerInfo*> usg_player; //使用中のプレーヤー

	std::list<PlayerInfo*> _player_proc_list; //プレイヤー処理リスト(先頭が現在使っているプレイヤー)



	void set_player_meter_position();

	unsigned long long int _point;
	unsigned int _hight_score;

	int _default_fh;
};

