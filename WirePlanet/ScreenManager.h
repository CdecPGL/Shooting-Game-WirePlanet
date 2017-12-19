#pragma once

#include"DxLib.h"
#include<unordered_map>
#include<vector>
#include<string>
#include"Vector2D.h"
#include<array>

//���C���[�ɂ��āB���C���[�̐������������قǉ��ɕ`�悳���B

class ScreenManager
{
public:
	ScreenManager(const ScreenManager&);
	~ScreenManager();
	static ScreenManager& GetInstance();
	int Init();
	int End();
	int ChangeScreenBufferNum(int); //�X�N���[���o�b�t�@�����ύX(1�ȏ�)
	int PushScreen();
	void DrawScreen(); //�X�N���[���`��
	void SetCommonPixShader(int,int); //���ʃs�N�Z���V�F�[�_�[�Z�b�g(PS�n���h���A�K���^�C�~���O)
	void ResetCommonPixShader(); //���ʃs�N�Z���V�F�[�_�[���Z�b�g
	enum class ScreenEffect{ Broom };
	int ApplyEffectToTopScreen(ScreenEffect,int); //���ݍőO�ʂɗL��X�N���[���ɃG�t�F�N�g���|����B(�G�t�F�N�g�̎�ށA����)
	struct PixShaderConstF{
		FLOAT4 vals[32];
		unsigned int size=0;
	};
	PixShaderConstF cmn_pix_shader_cnst_vals; //���ʃs�N�Z���V�F�[�_�ɓn���萔
private:
	ScreenManager();
	std::vector<int> _scr_buf;
	int _current_buf_num; //���ݕ`��Ώۂ̃o�b�t�@�i���o�[
	Vector2D<int> _draw_area_size;
	//�V�F�[�_�[�֘A
	std::pair<int, int> _cmn_ps; //���ʃs�N�Z���V�F�[�_�[�f�[�^
	VERTEX2DSHADER vertex[4];
	void ApplyPixcelShader(int,int,int); //�s�N�Z���V�F�[�_��K�p(���A��A�V�F�[�_)
	void SetCmnPixShaderCnstVals(); //���ʃs�N�Z���V�F�[�_�ɓn���萔���Z�b�g
};

