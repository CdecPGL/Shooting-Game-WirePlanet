#pragma once
#include "MessageWindow.h"
class SingleLineWindow :
	public MessageWindow
{
public:
	SingleLineWindow(int, int);
	~SingleLineWindow();

	void Update()override;
	void Draw()override;

	int Init(std::shared_ptr<const std::vector<std::string>>, Vector2D<int>, Vector2D<int>)override;
	int End()override;

	void SetDrawFrame(int);

	enum class Kind{ Normal, WhiteStream, BlackStream };
	void SetKind(Kind);
private:
	std::string text;
	int inc_cnt;
	int draw_frame;
	int _black_stream_gh;
	Kind _kind;
};
