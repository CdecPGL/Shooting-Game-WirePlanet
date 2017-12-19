#pragma once
#include "DxLib.h"
#include <unordered_map>
#include <string.h>
#include <string>
#include<memory>

class DataBlock;

using namespace std;

/*�A�j���[�V�����t�@�C���ɂ���
�`���͓�B�g���q��ganim���]�܂����BVersion2�ȏ�AVersion1�͓ǂݍ��ݑO�ɐݒ肷��B�f�t�H���g�ł�V2�ȏ�
1.AnimFile_v1��Lua��p��������(Version1)��AnimLibV5�ł͓��얢����---�Ή��j��---
���߂Ɂhsize=**�h�ŃA�j���T�C�Y���w��B�c���͓������Ȃ��Ƃ����Ȃ��B
����"ANIMATION_DATA_UNIT_NUM=**"�ŃA�j���[�V�����f�[�^���j�b�g�̐����w��B
���Ƃ̓A�j���[�V�����f�[�^���j�b�g���`���Ă����B
(n=0~**-1)��("
adun_tag="�^�O��";
adun_graphic_filename="�摜�t�@�C���̃p�X";
adun_interval=�A�j���[�V�����ɂ�����摜�؂�ւ��̊Ԋu;
adun_repeat_flag=�J��Ԃ���;
")

2.AnimLib_v2��DatScript��p��������(version2)�BDataScript�̕��@�ɏ]���B(Version2)
����u���b�N(��ł��Ȃ��ƃG���[)
version{�A�j���[�V�������C�u�����̃o�[�W����(2.0);}
size{��,����;}
graph_pass{
//�摜�t�@�C���̃p�X�B�����w��\�B�ォ��0,1,2�c�Ɣԍ����U����B
aaa.png;
bbb.png;
�c
}
�A�j���[�V������`�u���b�N
�^�O��{
interval,�A�j���؂�ւ��Ԋu(�t���[��)(�ȗ���1);
repeat,�J��Ԃ����ǂ���(0or1)(�ȗ���0);
//��������e�t���[���̉摜��ł̈ʒu(1�t���[�����珇�Ԃ�)(x,y�u���b�N���W(�w�肵���T�C�Y��)�A�摜�ԍ�[�ȗ���0])
FrameDefS;
0,0,0;
1,0,0;
2,0,0;
�c
FrameDefE;
}

3.AnimLib_v2��DatScriptV5��p��������(version3)�BDataScript�̕��@�ɏ]���B(Version5)
����u���b�N(��ł��Ȃ��ƃG���[)
version{�A�j���[�V�������C�u�����̃o�[�W����(3.1);}
size{�f�t�H���g��,����;}
graph_pass{
//�摜�t�@�C���̃p�X�B�����w��\�B�ォ��0,1,2�c�Ɣԍ����U����B�C�ӂŐ����̔ԍ���t������B����ꍇ�͂��ׂĂɂ���K�v������B�ȗ�����Əォ��(0,1,2�c�c�ƐU����)
aaa.png[,�摜�ԍ�];
bbb.png[,�摜�ԍ�];
�c
}
�A�j���[�V������`�u���b�N
animation{
	�^�O��{
		interval,�A�j���؂�ւ��Ԋu(�t���[��)(�ȗ���1);
		repeat,�J��Ԃ����ǂ���(0or1)(�ȗ���0);
		size,��,����;(�ȗ��Ńf�t�H���g�l)
		//�t���[�����B�e�t���[���̉摜��ł̈ʒu(1�t���[�����珇�Ԃ�)(x,y���W(�w�肵���T�C�Y��)�A�摜�ԍ�[�ȗ���0])
		frame{
			[AutomaticGeneration,[�摜�ԍ�];] //������w�肷��ƁA�摜�ԍ��Ŏw�肵���摜�����玩���Ńt���[�����w�肷��B���̎�����ȍ~�̃p�����[�^�͖��������̂Œ��ӁB
			[coordinates,pixcel or block = block;] //�ʒu�̒�`�@�B(pixecl�Ńs�N�Z���P�ʁAblock�Ńu���b�N�P�ʁB�f�t�H���g�ł�block)
			0,0,0;
			1,0,0;
			2,0,0;
			�c
		}
	}
	�^�O��{
		�c�c
	}
	�c�c
}
4.DatScript�W���f�[�^�\��v1��p��������(version4)�BDataScript_v5�̕��@�ɏ]���B
�f�[�^ID��ganim�B
��`��v3.1�ɏ]���B

*/
namespace AnimLib{

	//�A�j���[�V�����f�[�^���j�b�g
	struct AnimationDataUnit{
		//�R���X�g���N�^(�^�O,���摜�n���h��,�Ԋu,�J��Ԃ�,�T�C�Y)
		AnimationDataUnit() = default;
		AnimationDataUnit(const string, int, int, bool, int);
		~AnimationDataUnit(); //�f�X�g���N�^
		string tag; //�^�Oc
		vector<int> ghandle; //�摜�n���h��(0�͌��摜)c
		int num; //�A�j���[�V��������c
		int size_x,size_y; //�摜�T�C�Y
		int interval; //�摜�ύX�Ԋu(�t���[���P��)(0�ŃA�j���[�V�������Ȃ�)c
		bool repeat_flag; //true�ŌJ��Ԃ�c
//		std::unordered_map<std::string,std::vector<std::vector<std::string>>> additionnal_data;
		//�t���[�����
		struct FrameInfo{
			int graph_num = 0;
			int pos_x = 0;
			int pos_y = 0;
		};
		std::vector<FrameInfo> frame_info;
	};

	//�A�j���[�V�����f�[�^
	struct AnimationData{
		AnimationData(); //�����AnimationData���쐬
		//�f�X�g���N�^
		~AnimationData();
		//�ǂݍ��񂾃t�@�C�����̊m�F(true�ň�v)
		bool CheckFileName(string)const;

		string animfilename; //�ǂݍ��񂾃t�@�C����
		std::vector<std::unique_ptr<AnimationDataUnit>> animationdataunit; //�A�j���[�V�����f�[�^c
		int num; //�o�^���ꂽ�A�j���[�V�����̐�c
		std::vector<int> _ghandles; //�摜�n���h��

		void SetGhandlesToAnimDataUnit(); //�A�j���f�[�^���j�b�g�ɉ摜�n���h�����Z�b�g����
	};
};
