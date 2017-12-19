#pragma once

#include"DxLib.h"
#include<string>
#include<vector>
#include"Vector2D.h"

class MenuItems
{
public:
	MenuItems();
	~MenuItems();
	void Update();
	void Draw();

	void AddMenuItem(int,const std::string&); //���ڒǉ�(�C���f�b�N�X�A���ږ�)
	void ClearMenuItem(); //���ڍ폜


	int ProgressCursor(); //�J�[�\����i�߂�(�ύX��̃C���f�b�N�X�B)
	int BackCursor(); //�J�[�\����߂�(�ύX��̃C���f�b�N�X)

	void SetPosition(const Vector2D<int>&);
	void SetSize(const Vector2D<int>&);
	void SetLineHeight(int);
	void SetMargine(int);
	int SetFont(const std::string&); //�t�H���g�Z�b�g(�t�H���g��)
	const Vector2D<int>& GetPosition()const;

	int GetCurrentCursorPosition()const; //���݂̃J�[�\���ʒu���擾
private:
	Vector2D<int> position;
	Vector2D < int > size;
	int line_height; //�s�̍���
	int margine; //�s�Ԃ̌���
	int cursor_pos; //�J�[�\���̈ʒu
	double cursor_pos_d; //�\���Ɏg���J�[�\���̈ʒu
	int draw_line; //�\������s��
	struct Item{
		std::string name;
	};
	std::vector<Item> items;
	int fh; //�t�H���g�n���h��
};

