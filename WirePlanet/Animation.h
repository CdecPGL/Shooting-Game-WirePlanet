#pragma once
/**�A�j���[�V�����N���X
*Version1.2 2013/12/27
*Version2.0 2014/1/22 Animation�N���X�̌��J�֐����̂͂قڕύX�Ȃ��BAnimationDataAdmin��S�Č������B��V���O���g���ɕύX�B�ł��邾�����|�C���^���g��Ȃ��悤�ύX�BAnimation���C�u������AnimLib���O��Ԃɔz�u�BAnimationDataAdmin�ւ̃A�N�Z�X�֐���z�uAnimation�N���X�ɒǉ��BAnimation�Ƀf�t�H���g�R���X�g���N�^��ǉ�
*Version2.1 2014/1/27 DxLibrary3Das2D���g�����`��֐��ǉ�
*Version2.2 2014/2/12 FreeXYExtDraw()��ǉ�
*Version2.3 2014/5/26 �A�j���[�V�����f�[�^���ォ��Z�b�g����֐�SetAnimationData()�ǉ��B�ēǂݍ��ݏC���BAnimationDataAdmin�C���BAnimation::CheckAnimationPlaying(),GetAnomatopnTagList()�ǉ��B
*Version3.0 2014/5/29 �A�j���[�V�����t�@�C���`��V2��ǉ��B�ڍׂ�AnimationData.h
*Version4.0 2014/7/22 �A�j���[�V�����t�@�C���`��V3��ǉ��B�ڍׂ�AnimationData.h
*Version5.0 2014/9/04 �o�C�i���t�@�C������̃A�j���[�V�����t�@�C���ǂݍ��݂ɑΉ��B���łɓǂݍ��܂ꂽ�摜�n���h�����Q�Ƃ��ẴA�j���[�V�����t�@�C���쐬���\�ɂ����B�A�j���[�V�����f�[�^��`�Ńs�N�Z���P�ʂ̎w�肪�\�ɁB�摜�̕ێ��A�ǂݍ��ݕ��@�ύX�B
	AnimationdataAdmin::GetAnimationDataPointer�̈����Ɏ����ǂݍ��݃t���O�ǉ��BAnimDataV2�ȏ�ŏc���̃T�C�Y���ق�A�j���ɑΉ��B
*Version5.1 2014/9/06 AnimationDataManager�̊g���q�����ǉ��@�\��p�~
*Version5.2 2014/12/18 DataScript����̓ǂݍ��݂ɑΉ��BDS����̓ǂݍ��ݎ��̓o�[�W�����������Ŏw�肷��K�v������BAnimationDataAdmin���V���O���g�����B����ɔ���Animation�N���X����ada���폜�B���C�u���������N��pragma��ǉ�
*Version6.0 2014/12/21 DataScript�W���f�[�^�\���ɑΉ������o�[�W����V4��ǉ��BV2�ȏ�Ő擪��V1�Ƃ̎��ʂ�����K�v���Ȃ��Ȃ����BV1�旘�p����ꍇ�͓ǂݍ��ݑO��ADA�Ńo�[�W������ݒ肷��B
*Version6.1 2014/12/23 V3,4�Ńt���[���f�[�^�̎�����`�ɑΉ�
*Version6.2 2014/12/28 �}�l�[�W���[�ɓo�^�����ɃA�j���f�[�^��ǂݍ��݉\��
*Version6.3 2014/12/28 boostlibrary��1.57.0�ɕύX�Bv1��؂�̂�
*Version6.3.1 2015/1/9 Animation�N���X�R���X�g���N�^��AnimationData���Z�b�g����Ȃ��o�O���C��
*Version6.4 2015/1/10 �t���[���̊J�n�ԍ���1����0�ɕύX
*Version6.5 2015/2/27 �C�ӂ̉摜���w��\��
*Version6.6 2015/3/5 DxLibrary3a2�֘A���폜
*Version6.7 2015/3/20 Animation::Draw�̈����Ɋg�嗦,���]�t���O��ǉ��BVector2D<double>�Œǉ��BGetAnimationDataPointer�̎����ǂݍ��݃t���O�̏����l��false�ɕύX�A�t���O�I�t���̃o�O�C��
*/

#include "AnimationDataAdmin.h"
#include <vector>
#include <string.h>
#include <string>
#include "Vector2D.h"

#define ANIMATION_DRAW_CENTER_LEFT 1
#define ANIMATION_DRAW_CENTER_RIGHT 2
#define ANIMATION_DRAW_CENTER_UP 4
#define ANIMATION_DRAW_CENTER_DOWN 8

/**�A�j���[�V�������C�u�������O���
*/
namespace AnimLib{
	/**�A�j���[�V�����N���X
	*/
	class Animation
	{
	public:
		/**
		* @brief �R���X�g���N�^
		*/
		Animation();
		/**
		* @brief �R���X�g���N�^
		* @param �A�j���[�V�����f�[�^��
		*/
		Animation(const std::string&);
		/**
		* @brief �R���X�g���N�^
		* @param �A�j���[�V�����f�[�^
		*/
		Animation(std::shared_ptr<const AnimationData>);
		/**
		* @brief �f�X�g���N�^
		*/
		virtual ~Animation();
		/**
		* @brief �A�j���[�V�����f�[�^�Z�b�g
		* @param �A�j���[�V�����f�[�^��
		* @return -1�Ŏ��s�@�O�Ő���
		*/
		int AttachAnimationData(const std::string&);
		/**
		* @brief �A�j���[�V�����f�[�^�Z�b�g
		* @param �A�j���[�V�����f�[�^
		* @return -1�Ŏ��s�@�O�Ő���
		*/
		int AttachAnimationData(std::shared_ptr<const AnimationData>);
		/**
		* @brief �A�j���[�V�����̍X�V(�X�V���Ă����݂̊e�f�[�^���擾���Ȃ��Ɣ��f����Ȃ�)
		*/
		virtual void Update();
		/**
		* @brief ���݂̉摜���擾
		* @return �摜�n���h��
		*/
		int GetCurrentGraph() const;
		/**
		* @brief ���݂̉摜�̃T�C�Y���擾(�T�C�Y���i�[����ϐ��̃|�C���^)
		*/
		void GetCurrentGraphSize(int*,int*);
		/**
		* @brief ���݂̃t���[�����擾
		* @return ���݂̃t���[��(0-)�B-1�ŃA�j���[�V�������Đ�
		*/
		int GetCurrentFrame() const;
		/**
		* @brief ���݂̃A�j���[�V�����̃^�O�������ƈ�v���邩�m�F
		* @param ��r����^�O
		* @return ��v��true
		*/
		bool CheckCurrentAnimationTag(const std::string&) const;
		/**
		* @brief ���݂̃A�j���[�V�����̃^�O���擾����
		* @return �^�O(����������None)
		*/
		string GetCurrentAnimationTag() const;
		/**
		* @brief �^�O��p���ĔC�ӂ̃A�j���[�V������I�����X�^�[�g
		* @param �^�O
		* @return 0�Ő����A-1�Ŏ��s
		*/
		virtual int StartAnimationByTag(const std::string&);
		/**
		* @brief �^�O��p���ĔC�ӂ̃A�j���[�V������I�����X�^�[�g(Loop�w���)
		* @param �^�O
		* @return 0�Ő����A-1�Ŏ��s
		*/
		virtual int StartAnimationByTag(const std::string&,bool);
		/**
		* @brief �A�j���[�V�����̒�~
		*/
		void StopAnimation();
		/**
		* @brief ��Ԃ����Z�b�g
		*/
		void Reset();
		/**
		* @brief �A�j���[�V��������R�}�i�߂�
		* @return 0�Ő����A-1�Ŏ��s
		*/
		virtual int ProgressAnimation();
		/**
		* @brief �`��
		* @param ���Wx,y,�p�x,�g�嗦,���]�t���O,(���S���W�w��:ANIMATION_DRAW_CENTER_*_*���g���B�f�t�H���g�ł͒��S)
		*/
		void Draw(double, double, double,double,bool, char = 0);
		/**
		* @brief �`��
		* @param ���W,�p�x,�g�嗦,���]�t���O,(���S���W�w��:ANIMATION_DRAW_CENTER_*_*���g���B�f�t�H���g�ł͒��S)
		*/
		void Draw(const Vector2D<double>& pos, double angle, double ext,bool rf, char c = 0){ Draw(pos.x, pos.y, angle, ext,rf, c); }
		/**
		* @brief �T�C�Y���w�肵�ĕ`��
		* @param ���Wx,y,�p�x,�T�C�Y,(���S���W�w��:ANIMATION_DRAW_CENTER_*_*���g���B�f�t�H���g�ł͒��S)
		*/
		void FreeSizeDraw(double, double, double, int, char = 0);
		/**
		* @brief �c���g�嗦���w�肵�ĕ`��
		* @param ���Wx,y,�p�x,�g�嗦x,y,(���S���W�w��:ANIMATION_DRAW_CENTER_*_*���g���B�f�t�H���g�ł͒��S)
		*/
		void FreeXYExtDraw(double, double, double, double,double, char = 0);
		/**
		* @brief �A�j���[�V�����f�[�^�̂��A�j���[�V�����t�@�C��������Z�b�g(�݊��p)
		* @param �A�j���[�V�����t�@�C����
		* @return 0���� -1���s
		*/
		int SetAnimationData(const std::string&);
		/**
		* @brief �A�j���[�V�����Đ������m�F
		* @return false�Đ����Ă��Ȃ� true�Đ���
		*/
		bool CheckAnimationPlaying()const;
		/**
		* @brief �A�j���[�V�����^�O���X�g���擾
		* @param ���X�g���i�[����vector
		*/
		void GetAnimationTagList(std::vector<std::string>*)const;
		/**
		* @brief �A�j���[�V�����^�O���X�g���擾
		* @return �^�O���X�g���i�[����vector
		*/
		std::vector<std::string> GetAnimationTagList()const;
	private:
		shared_ptr<const AnimationData> animationdata; //�A�j���[�V�����f�[�^�̃|�C���^
		int counter; //�A�j���[�V�����p�J�E���^
		int frame; //�A�j���[�V�����̃t���[��
		int current_anim; //���݂̃A�j���[�V����
		bool _roop_flag;
		bool _playing;
	};
};
