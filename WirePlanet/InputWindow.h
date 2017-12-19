#pragma once
#include "MessageWindow.h"

const int MAX_INPUT_LENGTH(20); //Å‘å“ü—Í•¶š”(”¼ŠpŠ·Z)

//”¼Šp20•¶š(‘SŠp10•¶š)‚Ü‚Å
class InputWindow :
	public MessageWindow
{
public:
	InputWindow(int, int = -1);
	~InputWindow();
	int Init(std::shared_ptr<const std::vector<std::string>>, Vector2D<int>, Vector2D<int>)override;
	int End()override;

	virtual void Update()override;

	virtual void Draw()override;
private:
	int kih; //ƒL[“ü—Íƒnƒ“ƒhƒ‹
};
