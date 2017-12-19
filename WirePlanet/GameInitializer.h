#pragma once

#include<string>
#include<memory>

class Game;
class LuaStateHolder;

class GameInitializer
{
public:
	GameInitializer(const GameInitializer&) = delete;
	~GameInitializer();
	static GameInitializer& GetInstance();
	int Init();
	int Reload();
	//初期化の実行(対象のGameクラスポインタ,初期化ID[,初期化後に呼び出されるイベント,イベントパラメータ])
	int operator()(Game*, const std::string&, const std::string& = std::string("NULL"), int = 0);
private:
	GameInitializer();
	std::unique_ptr<LuaStateHolder> _l_state;
	int initLuaStateHolder();
};
