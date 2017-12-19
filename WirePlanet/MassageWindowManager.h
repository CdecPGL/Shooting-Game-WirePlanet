#pragma once

#include "MessageWindow.h"
#include "Vector2D.h"
#include "DataScript.h"
#include<memory>
#include <sstream>

#undef SendMessage

template<typename T, class C>
class ObjectHolderTemplate_WithID;


struct SpeakerData;

//�E�C���h�E���
enum MessageWindowKind{TEXT_WIN,SLCT_WIN,INPT_WIN};

//�f�[�^�u���b�N����"message0�`"

//�V���O���g��

class MessageWindowManager
{
public:
	~MessageWindowManager();
	static MessageWindowManager& GetInstance();
	int Init();
	int Reset();
	int Process();

	void Update();
	void Draw();

	int CreateRequest(MessageWindowKind, int, Vector2D<int>, Vector2D<int>,MessageWindow::FixedPosition, int = -1); //���b�Z�[�W�ԍ���I�����ă��b�Z�[�W�E�C���h�E���쐬(���,���b�Z�[�W�ԍ�,�ʒu,�T�C�Y[,�eID:�f�t�H���g��-1(�}�l�[�W���[���e)])(-1�Ŏ��s)
	int CreateRequestAndSetString(MessageWindowKind, const std::string&, Vector2D<int>, Vector2D<int>, int = -1); //�e�L�X�g���w�肵�ă��b�Z�[�W�E�C���h�E���쐬(���,�e�L�X�g������,�ʒu,�T�C�Y[,�eID:�f�t�H���g��-1(�}�l�[�W���[���e)])(-1�Ŏ��s)
	int CreateRequestMinimamSizeWindow(MessageWindowKind, int, int = -1); ////���b�Z�[�W�ԍ���I�����čŏ��T�C�Y�̃��b�Z�[�W�E�C���h�E���쐬����B(���,���b�Z�[�W�ԍ�[,�eID:�f�t�H���g��-1(�}�l�[�W���[���e)])(-1�Ŏ��s)
	int DeleteRequest(int,int=0); //ID�Ń��b�Z�[�W�E�C���h�E���폜�B���@�\��(ID,�������@0:�ӂ� 1:�q������ 2:�e������ 3:�e���q������)
	int SendMessage(int, const std::string&, int); //�w��ID�̃E�C���h�E�Ƀ��b�Z�[�W�𑗐M(ID,���b�Z�[�W,�p�����[�^)(�߂�l -1:�����ȃ��b�Z�[�W -2:ID������)
	std::shared_ptr<MessageWindow> GetMWPointer(int); //ID�Ŏw�肵���E�C���h�E�̃|�C���^���擾

	const std::shared_ptr<SpeakerData> GetSpeakerData(const std::string&)const; //�b�҃f�[�^�擾

	bool CheckExistence(int)const; //�w��ID�ɃE�C���h�E�����݂��邩(�쐬���N�G�X�g������T��)

	int GetMWCount()const; //�o�^����Ă���E�C���h�E�̐����擾

	std::shared_ptr<const std::vector<std::string>> GetMessageByID(int)const;  //�w��ԍ��̃��b�Z�[�W�擾

	enum ButtonKind{Select=1,Cancel=2,Up=4,Down=8,Left=16,Right=32};
	void ButtonPushed(ButtonKind); //�{�^���������ꂽ(�����ꂽ�{�^��)

	//�ϐ��n�֐�
	int SetMWValue(const std::string&, const std::string&,int = -1); //���b�Z�[�W�E�C���h�E�ϐ��Z�b�g(�ϐ����A���e[�A���[�J��ID���w��B�Ȃ��ŃO���[�o���̈�(-1)])(return 0:�㏑�� 1:�V�K�쐬)
	int SetMWValue(const std::string&, int, int = -1); //���b�Z�[�W�E�C���h�E�ϐ��Z�b�g(�ϐ����A���e[�A���[�J��ID���w��Ȃ��ŃO���[�o���̈�(-1)])(return 0:�㏑�� 1:�V�K�쐬)
	int SetMWValue(const std::string&, double, int = -1); //���b�Z�[�W�E�C���h�E�ϐ��Z�b�g(�ϐ����A���e[�A���[�J��ID���w��Ȃ��ŃO���[�o���̈�(-1)])(return 0:�㏑�� 1:�V�K�쐬)
	std::string GetMWValue(const std::string&,int = -1)const; //���b�Z�[�W�E�C���h�E�ϐ��擾(�ϐ���[�A���[�J��ID���w��B�w�肵�Ȃ��ꍇ�̓O���[�o���̈�(-1)])(������"NULL"���Ԃ����)
	int GetMWValueByInteger(const std::string&, int = -1)const; //���b�Z�[�W�E�C���h�E�ϐ��擾(�ϐ���[�A���[�J��ID���w��B�w�肵�Ȃ��ꍇ�̓O���[�o���̈�(-1)])(������0xFFFFFF���Ԃ����)
	void ClearMWValue(int = -1); //�w��̃��b�Z�[�W�E�C���h�E�ϐ��S����[���w��ŃO���[�o���̈�(-1)]
	void ClearAllMWValues();

	void Clear(); //�E�C���h�E�S����(���N�G�X�g��)

	//�P�s�E�C���h�E�֘A
	enum class SingleLineWinPos{Bottom=0,Top=1,Center=2};
	enum class SingleLineWinKind{Normal=0,WhiteStream=1,BlackStream=2};
	int CreateRequestSingleLineWindowByString(const std::string&, int,SingleLineWinPos,SingleLineWinKind); //�e�L�X�g���w�肵�ĒP�s�E�C���h�E���쐬(�e�L�X�g�A�\�����ԁA�ʒu�A���)

	//��b�E�C���h�E�֘A(DeleteRequest���ł��֘A�������s���Ă���̂Œ���)
	enum class TalkWinPos{Bottom=0,Top=1};
	int CreateTalkWindow(int,TalkWinPos); //��b�E�C���h�E�쐬(�e�L�X�g�ԍ�,�ʒu)
	int DeleteTalkWindow(); //��b�E�C���h�E�폜
	bool IsTalkWindowActive()const; //��b�E�C���h�E���A�N�e�B�u���擾
	int GetLastSelectedIndex()const; //�Ō�ɑI�����ꂽ�I�����̃C���f�b�N�X(�܂��I������Ă��Ȃ��ꍇ��-1��Ԃ�)
private:
	MessageWindowManager();
	int LoadMessageFromFile(); //�t�@�C�����烁�b�Z�[�W�ǂݍ���
	int id_counter;
	std::unique_ptr<ObjectHolderTemplate_WithID<int,MessageWindow>> mess_window_holder;
	std::map<int, std::shared_ptr<std::vector<std::string>>> messages;
	std::unordered_map<int,std::map<std::string, std::string>> global_value; //���b�Z�[�W�E�C���h�E�O���[�o���ϐ�
	Vector2D<int> game_draw_size; //�Q�[���̕`��T�C�Y

	int _normal_string_color_handle; //�W���̕����F�n���h��
	int _normal_font_handle; //�W���̃t�H���g�n���h��
	int _normal_name_font_handle; //�W���̖��O�\���p�t�H���g�n���h��

	std::unordered_map<std::string, std::shared_ptr<SpeakerData>> _speaker_list; //�b�҃��X�g

	//��b�E�C���h�E�֘A
	struct TalkWIndowInfo{
		int window_id = -1;
		mutable int last_selected_index = -1;
	};
	TalkWIndowInfo _current_talk_win;
};

