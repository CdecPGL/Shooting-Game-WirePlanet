#pragma once
#include "MessageWindow.h"
//#include <map>

class SelectWindow :
	public MessageWindow
{
public:
	SelectWindow(int, int = -1);
	~SelectWindow();

	virtual void Update()override;

	virtual void Draw()override;

	virtual int RecieveMessage(const std::string&, int = 0)override;

	virtual int Init(std::shared_ptr<const std::vector<std::string>>, Vector2D<int>, Vector2D<int>)override;
private:
	int cursor; //カーソルの位置
	std::vector<int> select_line; //選択肢の行番号リスト
	int status; //ステータス 0:選択中 1:命令実行中

	int MoveCursor(int); //カーソルの移動(移動数。下方向が正)(-1で範囲外)

	virtual void UpKeyPushed();

	virtual void DownKeyPushed();

	virtual void SelectKeyPushed();

	virtual void ResetStrings(std::shared_ptr<const std::vector<std::string>>)override;
};
