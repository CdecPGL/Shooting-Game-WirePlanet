#pragma once
#include "MessageWindow.h"

const int MAX_INPUT_LENGTH(20); //最大入力文字数(半角換算)

//半角20文字(全角10文字)まで
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
	int kih; //キー入力ハンドル
};
