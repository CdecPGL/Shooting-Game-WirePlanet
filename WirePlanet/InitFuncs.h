#include"DxLib.h"

namespace InitFuncs{
	int InitGame(); //�Q�[��������
	int EndGame(int=0); //�Q�[���I������(0:�ʏ�I�� 1:�ُ�I�� 2:�s�����m�ŏI��)
	int InitDxLibrary(); //DX���C�u�����̏������B�����f�[�^(InitDatas)�̓ǂݍ��݂��s���B
	int InitFileSystem(int,int); //�t�@�C���V�X�e��������(���������[�h(0:�J���f�[�^ 1:�A�[�J�C�u�f�[�^),���y�ǂݍ��݃��[�h(0:�܂Ƃ߂� 1:����))
	int InitFactory(); //�t�@�N�g��������
	int PrepareManager(); //�}�l�[�W������
	int ClearObjectInManager(); //�}�l�[�W�����̃I�u�W�F�N�g����������
	int CleanManager(); //�}�l�[�W�����̕s�v�ȃf�[�^���폜����
	int ReloadScriptFile(); //�t�@�C���ēǍ�
	int LoadAnimationData(); //�A�j���[�V�����t�@�C���̓ǂݍ���
	int LoadSaveData(); //�Z�[�u�f�[�^�ǂݍ���(0�Ő��� 1:�ŏ��ǂݍ��� 0�����ŃG���[)
	int SaveSaveData(); //�Z�[�u�f�[�^��������(0�Ő��� 0�����ŃG���[)
	int DeleteFileCash();

}