#pragma once
#include "MessageWindow.h"

const int MAX_INPUT_LENGTH(20); //�ő���͕�����(���p���Z)

//���p20����(�S�p10����)�܂�
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
	int kih; //�L�[���̓n���h��
};
