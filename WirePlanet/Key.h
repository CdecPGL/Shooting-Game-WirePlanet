//version 1.0 2013/12/15
//Version 2.0 2014/02/21 ���܂Œʂ�̃V���O���g���^�ɉ����A�m���V���O���g���^��ǉ�
//Version 2.1 2014/02/22 �V���O���g���^�̃R�s�[�R���X�g���N�^�֎~
//Version 2.2 2014/02/24 GetPushKey()�ɘA�˃J�E���^�����Z�b�g���邩�̑I���t���O��ǉ�
//Version 3.0 2014/03/17 �}�E�X���͊֘A�̊֐���ǉ�
//Version 4.0 2014/05/07 �p�b�h�֘A�̊֐��ǉ�(DX_INPUT_KEY_PAD1(1P)�̂�)
//Version 4.0a 2016/4/30 �������������̍Č�

#ifndef DEF_KEY_H
#define DEF_KEY_H

#include "DxLib.h"
#include<unordered_map>
#include <functional>

#define CUSTOM_INPUT_A 1 //�J�X�^������A
#define CUSTOM_INPUT_B 2 //�J�X�^������B
#define CUSTOM_INPUT_X 3 //�J�X�^������X
#define CUSTOM_INPUT_Y 4 //�J�X�^������Y
#define CUSTOM_INPUT_UP 5 //�J�X�^�����͏�
#define CUSTOM_INPUT_DOWN 6 //�J�X�^�����͉�
#define CUSTOM_INPUT_LEFT 7 //�J�X�^�����͍�
#define CUSTOM_INPUT_RIGHT 8 //�J�X�^�����͉E
#define CUSTOM_INPUT_L 9 //�J�X�^������L
#define CUSTOM_INPUT_R 10 //�J�X�^������R
#define CUSTOM_INPUT_ZL 11 //�J�X�^������ZL
#define CUSTOM_INPUT_ZR 12 //�J�X�^������ZR
#define CUSTOM_INPUT_START 13 //�J�X�^������START

//�L�[���̓N���X(��V���O���g��)

class nsKey {
private:
	char key[256]; //�L�[�̓��͏����i�[
	char pkey[256]; //�O��̍X�V���̃L�[���͏��
	bool burstflag[256]; //�A�˂��L����(true�ŗL��)
	mutable int counter[256]; //�A�˗p�J�E���^�[
	int interval[256]; //�A�ˊԊu

	int mouse_input; //�}�E�X�̓��͏����i�[
	int pmouse_input; //�O��̍X�V���̃}�E�X�̓��͏��

	int UpdateKey(); //�}�E�X�ƃL�[���͂̍X�V(0:���� -1:���s)

	int pad_input; //�p�b�h�̓���
	int ppad_input; //�O��̃p�b�h�̓���

	int UpdateJoyPad(); //�p�b�h�̓��͍X�V

	int dir_input; //��������
	int pdir_input; //�O��̕�������
	int UpdateDirectionInput(); //�������͂̍X�V


	bool GetJoypadState(int); //�p�b�h�̃{�^�����͏��擾
	bool GetJoypadPush(int); //�p�b�h�̃{�^���������ꂽ��

	bool GetDirState(int);
	bool GetDirPush(int);

	//�J�X�^���C���v�b�g�Ή��}�b�v<CUSTOM_INPUT_*,<State,Pushed>>
	std::unordered_map<int, std::pair<std::function<bool()>, std::function<bool()>>> custom_input_map; 

public:
	nsKey(); //�R���X�g���N�^
	virtual ~nsKey(); //�f�X�g���N�^
	int Update(); //��ԍX�V
	char GetKeyStatus(int) const; //����̃L�[���͏��̎擾(�L�[��������Ă����1��Ԃ�)
	char GetKeyPush(int, bool = true) const; //�L�[�������ꂽ�u�Ԃ���1��Ԃ�[�L�[�ԍ��A�A�˃J�E���^�[�����Z�b�g���邩(�f�t�H���g�ł�true)](�A�˂��L������������Ԋu���Ƃ�1��Ԃ��Acounter�����Z�b�g)
	bool GetBurstFlag(int) const; //����L�[�̘A�˂��L����
	void SetBurstFlag(int, bool); //����L�[�̘A�˂̗L���������Z�b�g
	void SetInterval(int, int); //����L�[�̘A�ˊԊu�̃Z�b�g

	bool GetMouseButtonState(int); //����{�^���̓��͏��̎擾
	bool GetMouseButtonPush(int); //����{�^���������ꂽ������true��Ԃ��B(������DxLiblary��MOUSE_INPUT_LEFT�Ȃ�)
	void GetMousePosition(int&, int&); //�}�E�X�̍��W���擾

	//�J�X�^���ݒ����
	//�J�X�^�����͏�
	bool GetCustomInputState(int);//
	//�J�X�^�����͉���
	bool GetCustomInputPush(int);//
	//�f�t�H���g�̃L�[���蓖�Ă�ݒ�
	void SetDefaultCustomMap();
};

//�V���O���g����
class Key : public nsKey{
private:
	Key();
	Key(const Key&);
public:
	~Key();
	static Key& GetInstance(); //�C���X�^���X�̎擾
};


#endif DEF_KEY_H