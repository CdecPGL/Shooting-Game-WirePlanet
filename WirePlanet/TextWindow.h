#pragma once
#include "MessageWindow.h"

class TextWindow :
	public MessageWindow
{
public:
	TextWindow(int, int = -1);
	~TextWindow();

	virtual void Update()override;

	virtual void Draw()override;

	virtual int Init(std::shared_ptr<const std::vector<std::string>>, Vector2D<int>, Vector2D<int>)override;

	virtual int RecieveMessage(const std::string&, int = -1)override;

private:
	virtual void SelectKeyPushed()override;
	int _popopo_sph;
};
