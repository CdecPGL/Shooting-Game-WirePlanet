#pragma once

#include<string>
#include<memory>
#include"DxLib.h"

class CSVLoader;
class DataScript;
struct lua_State;
struct ActionData;

namespace FileLoadUtil{
	//���[�h�����f�[�^���ǂ����邩(Remain:�c��,Unload:�j��,Set:�ݒ�ɏ]��)
	enum class LoadedData{ Remain, Unload, Set };
	static LoadedData loaded_data = LoadedData::Set; //���[�h�����f�[�^���ǂ����邩(�f�t�H���g�ł͐ݒ�ɏ]��)
	void SetLoadedDataTreat(LoadedData);
	//�t�@�C�����[�h�}�l�[�W������O���t�B�b�N�n���h���쐬�B�g�p�@��LoadGraph�Ɠ����B(�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int CreateGraphFromFLM(const std::string&, bool = true);
	//�t�@�C�����[�h�}�l�[�W�����番���O���t�B�b�N�n���h���쐬�B�g�p�@��LoadDivGraph�Ɠ����B(�t�@�C�����A�����A�������A�c�����A�������ꂽ�摜�̕��A�����A�n���h����ۑ�����o�b�t�@�A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)(���얢�m�F)
	int CreateDivGraphFromFLM(const std::string&, int, int, int, int, int, int*, bool = true);
	//�t�@�C�����[�h�}�l�[�W������T�E���h�n���h���쐬�B�g�p�@��LoadSoundMem�Ɠ����B(�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int CreateSoundMemFromFLM(const std::string&, bool = true);
	//�t�@�C�����[�h�}�l�[�W������s�N�Z���V�F�[�_�[�n���h���쐬�B�g�p�@��LoadPixelShader�Ɠ����B(�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int CreatePixelShaderFromFLM(const std::string&, bool = true);
	//�t�@�C�����[�h�}�l�[�W������CSV�t�@�C����ǂݍ���(�ǂݍ��ݐ��CSVLoader�N���X�|�C���^�A�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int LoadCSVFromFLM(CSVLoader*, const std::string&, bool = true);
	//�t�@�C�����[�h�}�l�[�W������f�[�^�X�N���v�g��ǂݍ���(�ǂݍ��ݐ��DataScript�N���X�|�C���^�A�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int LoadDataScriptFromFLM(DataScript*, const std::string&, bool = true);
	//�t�@�C�����[�h�}�l�[�W������Lua�t�@�C����ǂݍ���(�ǂݍ��ݐ��lua_State�|�C���^�A�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int LoadLuaScriptFromFLM(lua_State*, const std::string&, bool = true);
	//�t�@�C�����[�h�}�l�[�W������ActionData��ǂݍ���(�ǂݍ��ݐ��lua_State�|�C���^�A�t�@�C�����A�ǂݍ��݌�Ƀf�[�^��j�����邩�̃t���O)
	int LoadActionDataFromFLM(ActionData*, const std::string&, bool = true);
}
