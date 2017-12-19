#pragma once

#include <vector>
#include <map>
#include <list>
#include <string>
#include "Vector2D.h"
#include"DxLib.h"
#include "SoundManager.h"
#include<memory>

struct SpeakerData;

//�萔
//�R�}���h������(����)
namespace{
	//const std::string CREATE_TEXT_WIN("TextWindow"); //�e�L�X�g�E�C���h�E�쐬(���b�Z�[�W(�e�L�X�g)�ԍ�[,����=0(0:�q�Ƃ��č�� 1:�Ɨ����č�� 2�Ɨ����č�莩��������))
	const std::string CREATE_SLCT_WIN("SelectWindow"); //�Z���N�g�E�C���h�E�쐬(���b�Z�[�W(�e�L�X�g)�ԍ�[,����=0(0:�q�Ƃ��č�� 1:�Ɨ����č�� 2�Ɨ����č�莩��������))
	//const std::string CREATE_INPT_WIN("InputWindow"); //���̓E�C���h�E�쐬(���b�Z�[�W(�e�L�X�g)�ԍ�[,����=0(0:�q�Ƃ��č�� 1:�Ɨ����č�� 2�Ɨ����č�莩��������))
	const std::string READ_TEXT("ReadText"); //�ǉ��Ńe�L�X�g��ǂݍ���(�e�L�X�g�ԍ�)
	const std::string CLOSE_WIN("Close"); //�E�C���h�E�����(0:�����̂݁A1:�q�Ǝ����A2:�����Ɛe 3:�e�Ǝ����Ǝq)
	const std::string CHANGE_SPEAKER("ChangeSpeaker"); //�b�ҕύX(�b��ID)
	const std::string CHANGE_EXPRESSION("ChangeExpression"); //�\��ύX(�\��ID)
	const std::string WAIT_KEY_PUSH("WaitKeyPush"); //�L�[���͑ҋ@
	const std::string WAIT_TIME("WaitTime"); //��莞�ԑҋ@(�t���[��)
	const std::string CHANGE_DRAW_SPEED("ChangeDrawSpeed"); //�ꕶ���̕\�����x�ύX([�ꕶ���̕\���t���[����]=Default)
	const std::string CLEAR("Clear"); //�\����������
	//��������
	const std::string JUMP("Jump"); //�w��̃��x���ɃW�����v�B���x�����Ȃ������疳��(���x����)
	const std::string IF_EQUAL_JUMP("IfEqualJump"); //2�̈����̓��e��������������w��̃��x���܂ŃW�����v����B���x�����Ȃ������疳��(��r�Ώ�1,��r�Ώ�2,�W�����v�惉�x��)
	const std::string IFN_EQUAL_JUMP("IfnEqualJump"); //2�̈����̓��e���قȂ�����w��̃��x���܂ŃW�����v����B���x�����Ȃ������疳��(��r�Ώ�1,��r�Ώ�2,�W�����v�惉�x��)
	//�ϐ��֌W
	const std::string SET_MW_VALUE("SetVariable"); //���b�Z�[�W�E�C���h�E�O���[�o���ϐ�(�^��string)�Z�b�g(�ϐ���&�Z�b�g������e[&�Z�b�g��(G:�O���[�o���̈�orL:���[�J���̈�)=G])
	const std::string ADD_MW_VALUE("AddVariable"); //���b�Z�[�W�E�C���h�E�O���[�o���ϐ�(�^��string)�Z�b�g(�ϐ���&���Z�l)
}

//�R�}���h������ɂ���
//�R�}���h���ʎq:\\�o�b�N�X���b�V��(�~�L�����p)�~2
//�R�}���h:�萔�̍��Q��
//�R�}���h����:�R�}���h���s���̃p�����[�^�B�����̎���&(�A���h)�ŋ�؂�
//���@:[�R�}���h���ʎq][�R�}���h](=)[�R�}���h����]

//���x���ɂ���
//���x�����ʎq:@@

//�ϐ��ɂ���(�������B�e�L�X�g���̕ϐ��u����������)
//�ϐ����ʎq:<<,>>�ň͂�
//�����񒆂ɕϐ����������炻�̓��e�ɒu�������B�Y������ϐ����Ȃ������疳�������
/*�ȉ��̕ϐ��̓V�X�e���Ŏg�p����܂�
SelectedIndex:�I���E�C���h�E�őI�����ꂽ���ڂ̓`�B�Ŏg�p
*/

//���b�Z�[�W(����)
//***KeyPushed ***�L�[�������ꂽ(***=Up,Dpwn,Left,Right,Select,Cancel)(�����Ȃ�)
//ChildWindowDestructed �q�E�C���h�E���j�����ꂽ(�j�����ꂽ�E�C���h�E��ID)
//ParentWindowDestructed �e�E�C���h�E���j�����ꂽ(�j�����ꂽ�E�C���h�E��ID)
//struct MWMessage{
//	MWMessage(const std::string&, int...);
//	std::string message;
//	std::vector<int> param;
//};

class MessageWindow
{
public:
	MessageWindow(int, int = -1);
	virtual ~MessageWindow();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	//������(�����f�[�^,�ʒu,�T�C�Y)
	virtual int Init(std::shared_ptr<const std::vector<std::string>>, Vector2D<int>, Vector2D<int>);
	//�I������
	virtual int End(); //�I������

	//�擾�֐�
	int GetMyID()const; //������ID�擾
	int GetParentID()const; //�e��ID�擾(-1�Ŗ���)
	int GetChildID()const; //�q��ID�擾(-1�Ŗ���)
	Vector2D<int> GetPosition()const; //�ʒu�擾
	Vector2D<int> GetSize()const; //�T�C�Y�擾

	//�ݒ�֐�
	void SetStringColorHandle(int);
	void SetFontHandle(int);
	void SetNameFontHandle(int);

	virtual int RecieveMessage(const std::string&, int = 0); //���b�Z�[�W�̎󂯎��֐�(���b�Z�[�W�A�p�����[�^//,����//)(-1:�����ȃ��b�Z�[�W)

	enum class FixedPosition{ None = 0, Left = 1, Right = 2, Top = 4, Bottom = 8 };
	void SetFixedPosition(FixedPosition);

protected:
	Vector2D<int> position; //�E�C���h�E�̍�����W
	Vector2D<int> size; //�E�C���h�E�̃T�C�Y
	FixedPosition _fixed_pos; //�Œ�ʒu
	int word_size = 16;
	std::shared_ptr <const std::vector<std::string>> strings; //������f�[�^
	bool enable; //����\��
	SoundManager& sdm;
	int _string_col_handle; //�����F�n���h��
	int _font_handle; //�t�H���g�n���h��
	int _name_font_handle; //�b�Җ��p�t�H���g�n���h��
	std::shared_ptr<SpeakerData> _current_speaker;
	int _current_face_graph;
	int _word_draw_frame; //�ꕶ���̕\������
	bool _waiting_key_push; //�L�[���͒�����
	int _wait_frame; //�ҋ@����t���[��(0�Ŕ�ҋ@)
	std::list<std::string> _draw_strings; //�\�����镶����
	int _max_line; //�\���\�s��(�����̑傫���ƃE�C���h�E�̍������玩���Œ���)
	int _line; //���݂̍s(0����)
	int _raw; //���݂̗�(0����)
	int _word_draw_cntr; //�����\���J�E���^�[m,
	int _inc_cntr; //�����J�E���^

	void UpdateCounter();

	int DeleteRequestMWM(int = 0); //�}�l�[�W���[�ɍ폜�˗�(0(�f�t�H���g):���������폜 1:�q���S�č폜 2:�e���q�����ׂč폜)
	void DrawWindow(); //�E�C���h�E�̕`��
	void DrawStrings(); //������̕`��
	void DrawFaceGraphic(const Vector2D<int>&, const Vector2D<int>&); //��摜�`��(���㑊�΍��W�A�T�C�Y)

	void PushFrontDrawStrings(const std::string&); //�\���������擪�ɒǉ�
	void PushBackDrawStrings(const std::string&); //�\����������Ō���ɒǉ�

	//�L�[�������ꂽ���̏���
	virtual void UpKeyPushed(); //��L�[�������ꂽ
	virtual void DownKeyPushed(); //���L�[�������ꂽ
	virtual void LeftKeyPushed(); //���L�[�������ꂽ
	virtual void RightKeyPushed(); //�E�L�[�������ꂽ
	virtual void SelectKeyPushed(); //�I���L�[�������ꂽ
	virtual void CancelKeyPushed(); //�L�����Z���L�[�������ꂽ

	//�R�}���h�֘A
	bool CheckCommand(const std::string&)const; //�����񂪃R�}���h�����񂩊m�F
	int ProcessCommand(const std::string&); //���ߕ�����̏���(0:�ʏ�R�}���h���s 1:���f�R�}���h���s -1:�R�}���h������łȂ� -2:�R�}���h���s -3:�����ȃR�}���h)
	std::string GetNextString(); //���̕�����擾(���߂͎��s���Ď��ɐi��)

	//���x���֘A
	std::unordered_map<std::string, int> _label; //���x�����X�g(���x�����A���ݍs)
	bool IsLabel(const std::string&)const; //���x�����m�F
	void AnalyzeLabel(); //���x�����
	int FindLabel(const std::string&)const; //���x�������B�Ȃ�������-1(�����̕ϐ��̈��T���āA�Ȃ�������O���[�o���̈��T��)

	//�ϐ��֘A
	std::string PermuteVariable(const std::string&); //�^����ꂽ������̕ϐ������̓��e�ɒu�������������Ԃ��B

	virtual void ResetStrings(std::shared_ptr<const std::vector<std::string>>); //�h���N���X�̉��z�֐��ł͕K���Ō�ɂ��̃N���X��ResetStrings���Ă�

	static bool SJISMultiCheck(unsigned char); //S_JIS�ɒu���ă}���`�o�C�g�����̃o�C�g���`�F�b�N
private:
	int ID; //���b�Z�[�W�E�C���h�EID
	int pID; //�e��ID //-1�Ń}�l�[�W���[���e
	int cID; //�q��ID //-1�Ŏq�Ȃ�
	//	std::vector<MWMessage> message_pool; //�󂯎�������b�Z�[�W���X�g

	std::map<std::string, void(MessageWindow::*)()> keypushmess_map; //�L�[���͌n���b�Z�[�W�ɑΉ����郁���o�֐�
	int ProcessKeyPushedMessage(const std::string&); //�L�[���͌n���b�Z�[�W����(0:�L�[���͏������s -1:���b�Z�[�W�ɓ��Ă͂܂鏈�������݂��Ȃ�)���q�N���X���̂��̊֐��̐擪�ł��̃N���X�̂��̊֐����Ăяo��
};
