#pragma once

#include"DxLib.h"
#include<vector>
#include"Vector2D.h"

class LevelChangeEffect
{
public:
	LevelChangeEffect();
	~LevelChangeEffect();
	void Update();
	void Draw();
	void DebugDraw();
	int Init();
	
	void StartEffect();

	bool Playing()const;

	//�X�e�[�W�ԍ����Z�b�g����(1~:�ʏ� 0:�`���[�g���A�� -1:�t�@�C�i���X�e�[�W -2:�Q�[���I�[�o�[ -3:�e�X�g�X�e�[�W -4:�N���A -5:�ŏI�g -6:�{�X -7:�ʏ�N���A -8:Ready�c�c?)
	void SetStageNumber(int);
	void SetFontData(const std::string&,int,int); //�t�H���g�f�[�^����(�t�H���g���A�����A�t�H���g�^�C�v)
	void SetPosition(const Vector2D<int>&);
	void SetSize(const Vector2D<int>&);
private:
	Vector2D<int> position;
	Vector2D<int> size;
	int stage_num;
	int fh;
	int f_size;
	std::string f_name;
	int f_thick;
	int f_type;
	int gh;
	int cnt;
	Vector2D<int> g_size;
	int separation_y; //�c������
	bool playing; //�Đ�����

	int SetUpEffect(); //�G�t�F�N�g����
	static std::string GetEffectString(int); //�G�t�F�N�g������擾
	static std::string GetOrdin(int); //�����擾
};

