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
	//�������̎��s(�Ώۂ�Game�N���X�|�C���^,������ID[,��������ɌĂяo�����C�x���g,�C�x���g�p�����[�^])
	int operator()(Game*, const std::string&, const std::string& = std::string("NULL"), int = 0);
private:
	GameInitializer();
	std::unique_ptr<LuaStateHolder> _l_state;
	int initLuaStateHolder();
};
