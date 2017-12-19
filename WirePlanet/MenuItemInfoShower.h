#pragma once

#include"Vector2D.h"
#include"DxLib.h"

#include<unordered_map>

class MenuItemInfoShower
{
public:
	MenuItemInfoShower();
	~MenuItemInfoShower();

	int Init();
	void Update();
	void Draw();

	void ChangeSelectedItem(int);

	void SetPosition(const Vector2D<int>&);
	void SetSize(const Vector2D<int>&);
	void SetAnimationSpeed(double);

	void AddGraphHandle(int, int); //�摜�n���h���ǉ�(�C���f�b�N�X�A�摜�n���h��)

private:
	Vector2D<int> position;
	Vector2D<int> size;
	double circle_rate; //�~�̑傫���{��
	int anim_cnt; //�A�j���[�V�����J�E���^
	double anim_speed; //�A�j���[�V�����̑���

	std::unordered_map<int, int> ghandle_list; //�摜�n���h�����X�g
	int current_index; //���݂̃C���f�b�N�X
	int next_index; //�ύX��̃C���f�b�N�X

	Vector2D<int> _draw_area_size;
};

