#pragma once
//2013/10/30 Version 1.0 �A�N�V�����t�@�C���̓ǂݍ��݊֐����������A�N�V�������C�u����������
//2014/5/1 Version1.1 GetCurrentActionByTag(),CheckActionPlaying()�֐��ǉ��B
//2014/6/16 Version1.2 StartActionByTag�ɌJ��Ԃ��w���ǉ�
//2014/6/22 Version1.3 �ꕔ�֐��̈������R���X�g�Q�ƂɕύX
//2014/9/5 Version1.4 ��������̃f�[�^����̃A�N�V�����f�[�^�쐬�ɑΉ��B�O������̉摜�n���h���Q�Ɠǂݍ��݂ɑΉ�
#include "DxLib.h"
#include <vector>
#include <string.h>
#include <string>
#include<memory>
#include<unordered_map>

//����̑���
const int Col_Atrb_Trrn(1); //�n�`(�n�`�ƏՓ�)
const int Col_Atrb_Atck(2); //�U��
const int Col_Atrb_Dmge(4); //�_���[�W���󂯂�
const int Col_Atrb_Item(8); //�A�C�e��(�A�C�e�����������ƃA�C�e��������������ƏՓ˂ł��܂�)

//�����蔻��f�[�^
struct HitAreaData{
	HitAreaData(int, int, int, int, int, int, int* = nullptr); //�R���X�g���N�^(����,���΍��Wx,y,���a,�n�܂�,�I���t���[��,�ǉ��p�����^)c
	int attribute; //���
	int x, y; //���ΓI�ʒu(-�ŏ�+�ŉ�)c
	int radius; //���ac
	int start_frame, end_frame; //�����蔻��̎n�܂�A�I���t���[��(1~num)c(0,0�ŃA�N�V�����������ƗL��)
	int additional_param[2]; //�ǉ��p�����[�^(WirePlanet�ł�0:�U���� 1:�^���閳�G����)
};

//�A�N�V�����p�f�[�^�P��
struct ActionDataUnit{
	//�R���X�g���N�^(�^�O,���摜�n���h��,�Ԋu,�J��Ԃ�,�T�C�Y)
	ActionDataUnit(){};
	ActionDataUnit(const std::string, int, int, bool, int);
	~ActionDataUnit(); //�f�X�g���N�^
	const std::string tag; //�^�Oc
	std::vector<int> ghandle; //�摜�n���h��(0�͌��摜)c
	int num; //�A�j���[�V��������c
	int interval; //�摜�ύX�Ԋu(�t���[���P��)(0�ŃA�j���[�V�������Ȃ�)c
	bool repeat_flag; //true�ŌJ��Ԃ�c
	std::vector<HitAreaData*> hitarea; //�����蔻��f�[�^c
};

//�A�N�V�����f�[�^
struct ActionData{
	ActionData(); //�R���X�g���N�^()
	explicit ActionData(const char*); //�R���X�g���N�^(�t�@�C����)
	~ActionData();
	int size; //�L�����̃T�C�Y(�r�b�g�P��)c
	std::vector<ActionDataUnit*> data; //�A�N�V�����f�[�^�P��c
	//�A�N�V�����t�@�C���ǂݍ��݊֐�(�t�@�C����,�������ރA�N�V�����f�[�^�̃|�C���^)
	int ReadActionFileFromFile(const std::string&);
	//����������A�N�V�����t�@�C���ǂݍ��݊֐�(�f�[�^�̐擪�A�h���X�A�f�[�^�T�C�Y)
	int ReadActionFileFromMem(const void*,unsigned int);
	int num; //�o�^���ꂽ�A�N�V�����̐�c
	//�摜�n���h���̎Q��
	using RefGHandlesType = std::unordered_map<std::string,int>;
	static RefGHandlesType _ref_ghandles;
	static bool _ghandle_ref_flag; //�摜�����Q�ƃt���O
private:
	ActionData(const ActionData&) = delete;
	const ActionData& operator = (const ActionData&) = delete;
	//�����ςݕ����񂩂�A�N�V�����t�@�C���ǂݍ��݊֐�(�f�[�^������S)
	int ReadActionFileFromSeparatedString(std::vector<std::string>&&);
};

class ActionDataAdmin{
public:
	~ActionDataAdmin();
	ActionDataAdmin(const ActionDataAdmin&) = delete;
	static ActionDataAdmin& GetInstance();
	int Init();
	ActionData& GetActionData(const std::string&);
	void SetRefGHandles(ActionData::RefGHandlesType&&);
	void SetRefGHandles(const ActionData::RefGHandlesType&);
	void SetRefGhandleFlag(bool);
	void Clear();
private:
	ActionDataAdmin();
	std::unordered_map<std::string, std::unique_ptr<ActionData>> action_datas;
};

//�A�N�V�����N���X
class Action
{
public:
	Action(const ActionData&); //�R���X�g���N�^(�A�N�V�����f�[�^�̎Q��)

	~Action(void); //�f�X�g���N�^
	//	Action(const Action& action); //�R�s�[�R���X�g���N�^
	void Update(); //�A�j���[�V�����̍X�V(�X�V���Ă����݂̊e�f�[�^���擾���Ȃ��Ɣ��f����Ȃ�)
	int GetCurrentGraph() const; //���݂̉摜���擾
	int GetCurrentFrame() const; //���݂̃t���[�����擾(0�ŃA�j���[�V�����I����)
	bool CheckCurrentActionTag(const std::string&) const; //���݂̃A�N�V�����̃^�O�������ƈ�v���邩�m�F(true�ň�v)
	int StartActionByTag(const std::string&); //�^�O��p���ĔC�ӂ̃A�N�V������I�����X�^�[�g
	int StartActionByTag(const std::string&, bool); //�^�O��p���ĔC�ӂ̃A�N�V������I�����X�^�[�g
	int ProgressAnim(); //�A�N�V��������R�}�i�߂�
	const std::string GetCurrentActionTag()const; //���݂̃A�N�V�����^�O���擾�B�I�����Ă�����"None"��Ԃ�
	bool CheckActionPlaying()const; //�A�N�V�����Đ��������ׂ�
	//�����蔻��
	//	int GetCurrentHitAreaNum() const; //���ݗL���ȓ����蔻��̐��𓾂�
	int SetCurrentHitAreaToVector(std::vector<HitAreaData*>*) const; //HitAreaData�|�C���^��vector�Ɍ��ݗL���ȓ����蔻��f�[�^������
	int GetGraphSize()const; //�摜�T�C�Y����
private:
	int counter; //�A�j���[�V�����p�J�E���^
	int frame; //�A�j���[�V�����̃t���[��
	int current_anim; //���݂̃A�j���[�V����
	const ActionData& actiondata;
	//	int current_hitarea_num; //���݂̓����蔻��̐�
	bool repeat;
};