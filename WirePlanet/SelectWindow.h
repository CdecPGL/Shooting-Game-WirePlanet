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
	int cursor; //�J�[�\���̈ʒu
	std::vector<int> select_line; //�I�����̍s�ԍ����X�g
	int status; //�X�e�[�^�X 0:�I�� 1:���ߎ��s��

	int MoveCursor(int); //�J�[�\���̈ړ�(�ړ����B����������)(-1�Ŕ͈͊O)

	virtual void UpKeyPushed();

	virtual void DownKeyPushed();

	virtual void SelectKeyPushed();

	virtual void ResetStrings(std::shared_ptr<const std::vector<std::string>>)override;
};
