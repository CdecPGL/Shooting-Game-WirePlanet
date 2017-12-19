#pragma once

//�V�[���}�l�[�W���[
//2014/04/28 Version1.0 RPG3m���ڐA
//2014/05/08 Version2.0 �V�[���̕ύX�����ύX�B�ύX���Ɍ��݂̃V�[���Ǝ��̃V�[�����ǂ�����̂��̎w����@��ύX�B
//2014/06/14 Version2.1 �V�[���̕ێ��A�o�^�A�o�^������ObjectHolderTemplate_WithID�ɒu������
//2014/06/25 Version2.2 �V�[����Process()��Update()��Draw()�ɕ���

#include<map>
#include<memory>
#include<string>
#include"DataContainer.h"
#include<unordered_map>
#include<tuple>
#include"Scene.h"

template<typename T,class C>
class ObjectHolderTemplate_WithID;
class SceneChangeEffect;

//�V�[���ύX������w��񋓑�
//���݂̃V�[�����ǂ����邩(END:�I���������s�����j���͂��Ȃ� INTERRUPT:�I���������s��Ȃ� DISPOSE:�I���������s���j������)
enum CSCrntSceneProc{END,INTERRUPT,DISPOSE};
//���̃V�[�����ǂ����邩(INIT:���������� RESTART:���������Ȃ�(���������������珉��������))
enum CSNextSceneProc{INIT,RESTART};

//�V���O���g��

class SceneManager
{
public:
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	static SceneManager& GetInstance();
	int Init(); //������
	int End(); //�I������
	int Process(); //�Ǘ�����(-1�ŃG���[ 1�ŃQ�[���I��)
	void Clean(); //���Q�Ƃ̃V�[�����폜
	void Clear(); //�S�폜

	void Update(); //�V�[���X�V
	void Draw(); //�V�[���`��

	int EndGameRequest(); //�Q�[���I�����N�G�X�g

	std::shared_ptr<Scene> CreateObject(const std::string&,const std::string& ="NULL"); //�V�[���쐬(������id,�C�ӂ�id)
	int DeleteObject(const std::string&); //�V�[���폜(id)
	bool CheckObjectResisted(const std::string&)const; //�w��ID�̃I�u�W�F�N�g�����݂��Ă邩
	std::shared_ptr<Scene> FindObject(const std::string&)const; //�w��ID�̃I�u�W�F�N�g���擾
	int GetObjectCount()const; //�o�^����Ă���I�u�W�F�N�g�����擾
	std::string GetNewID()const; //�d�����Ȃ�ID���擾

	int ChangeSceneRequest(const string&,CSCrntSceneProc = DISPOSE,CSNextSceneProc = INIT); //ID�ŃV�[���̕ύX�˗�(id,���݂̃V�[�����ǂ����邩�A�ύX�V�[�����ǂ����邩)(-1�̎��͎��s�ŕύX����Ȃ��B)
	std::shared_ptr<Scene> CreateAndChangeScene(const std::string&, const std::string& = "NULL", CSCrntSceneProc = DISPOSE, CSNextSceneProc = INIT); //�V�[�����쐬���ĕύX(������ID�A�C�ӂ�ID,���݂̃V�[�����ǂ����邩�A�ύX�V�[�����ǂ����邩)
	shared_ptr<Scene> GetCurrentScene()const; //���݂̃V�[���̃|�C���^���擾
	std::string GetCurrentSceneID()const; //���݂̃V�[����ID���擾
private:
	bool initialized; //���������ꂽ��

	bool _game_end_reuest;

	std::unique_ptr<ObjectHolderTemplate_WithID<std::string, Scene>> scene_holder; //�V�[���z���_�[

	SceneManager();
	std::shared_ptr<Scene> current_scene;
	struct Request{
		Request(const std::string&,CSCrntSceneProc,CSNextSceneProc);
		std::string ID;
		CSCrntSceneProc csp; //���݂̃V�[�����ǂ����邩
		CSNextSceneProc nsp; //���̃V�[�����ǂ����邩
	};
	Request request; //�V�[���ύX���N�G�X�g(ID,���݂̃V�[���𒆒f���邩�A�ύX�V�[�����ĊJ���邩)("None"�łȂ�)

	void ChangeScene(); //�V�[���ύX����

	shared_ptr<DataContainer> datacontainer; //�V�[���O���[�o���f�[�^�R���e�i

//	std::unique_ptr<SceneChangeEffect> _scn_cng_efct;

	//�O���[�o���ϐ�
//	shared_ptr<Counter> counter;
};

