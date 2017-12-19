#pragma once

#include<string>
#include<list>
#include"DxLib.h"
#include "Vector2D.h"
#include"Counter.h"
#include<unordered_map>

/*
DebugInfo
�G���[���ȂǁB��x�o�^�������萔�܂��ێ������B
DebugData
�f�o�b�O�p�ɉ�ʂɕ\������f�[�^Draw()�Ŕj�������̂ŁA���t���[���o�^����B
*/

//�V���O���g��

using namespace std;

class DebugWindow
{
public:
	~DebugWindow();
	static DebugWindow& GetInstance();

	void Update(); //�X�V
	void Draw();
	void SetDebugInfo(int, string, string, double); //���o�^(�d�v�x�A�ʒu�A���e�A�G���[�ԍ�)
	void SetDebugData(Vector2D<int>, string, int); //�f�[�^�o�^(�\���ʒu�A�\�����e�A�F(GetColor()))
	void AddExecutionTime(const std::string&, int); //���s���Ԓǉ�
	void ChangeMaxBuffer(int);
	void ChangeMaxLine(int);
	void ChangeDrawTime(int);
	void ChangeFontSize(int);
private:
	DebugWindow();

	int max_buffer; //�ő�ۑ���
	int max_line; //�ő�\����
	int draw_time; //�\������(�t���[��)
	int size; //�����T�C�Y
	struct DebugInfo{
		DebugInfo(int, string, string, double);
		int level; //�d�v�x
		string location; //�ꏊ
		string info; //���
		double num; //�ԍ�
	};
	list<DebugInfo> di; //�f�o�b�O���
	int di_fh; //�f�o�b�O���p�t�H���g�n���h��

	int counter; //�ύX����̌o�ߎ���

	int GetColorByLevel(int); //�d�v�x�̐F���擾

	struct DebugData{
		DebugData(Vector2D<int>, string, int);
		Vector2D<int> position; //�\���ʒu
		string data; //�f�[�^������
		int color; //�F(GetColor())
	};
	list<DebugData> dd; //�f�o�b�O�p�\��������
	int dd_ak_fh; //�f�o�b�O�f�[�^�p�E�e��f�[�^�\���p�t�H���g�n���h��

	std::unordered_map<std::string, int> _execution_times; //���s����
	void DrawPerformanceInfo(); //�p�t�H�[�}���X���\��
	void DrawManagerInfo();

	enum class DrawInfo{ None, MngrInnfo, PfmcInfo };
	DrawInfo _draw_info;
	void ChangeDrawInfo();
	bool debug_data_visible; //�f�o�b�O�f�[�^�\���t���O
	Counter& cntr;
};

//�����t���o�^
#   define SetFormatDebugInfo( lev,loc,inf,nm, ... ) \
{ \
	TCHAR c[256]; \
	_stprintf_s(c, inf, __VA_ARGS__); \
	DebugWindow::GetInstance().SetDebugInfo(lev, loc, c, nm); \
}

// �����t�f�[�^�o�^
#   define SetFormatDebugData( x,y,col,dat, ... ) \
{ \
	TCHAR c[256]; \
	_stprintf_s(c, dat, __VA_ARGS__); \
	DebugWindow::GetInstance().SetDebugData(Vector2D<int>(x,y), c, col); \
}