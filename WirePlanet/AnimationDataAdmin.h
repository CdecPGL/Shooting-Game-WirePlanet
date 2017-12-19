#pragma once

#include"AnimationData.h"
#include<vector>
#include<unordered_map>

using namespace std;

class DataScript;

namespace AnimLib{

	class AnimationDataAdmin
	{
	public:
		using RefGHandleType = std::unordered_map < std::string, int > ;
		struct LoadSettings{
			bool ref_ghandles_flag = false;
			const RefGHandleType* ref_ghandles = nullptr;
			bool reload_flag = false;
		};

		~AnimationDataAdmin();
		AnimationDataAdmin(const AnimationDataAdmin&) = delete;
		const AnimationDataAdmin& operator=(const AnimationDataAdmin&) = delete;
		static AnimationDataAdmin& GetInstace();
		int Init(); //������
		shared_ptr<AnimationData> GetAnimationDataPointer(const std::string&,bool=false); //�A�j���t�@�C��������|�C���^���擾(�A�j���f�[�^���A�����ǂݍ��݃t���O)(�܂��ǂݍ��܂�Ă��Ȃ�������t���O��T�̏ꍇ�͎����œǂݍ��݁B�Ȃ��Ŗ����ȃ|�C���^��Ԃ��B)(�g���q���ȗ���.ganim�������ǉ�)
		shared_ptr<AnimationData> ReloadAnimationData(const std::string&); //�w��̃A�j���[�V�����f�[�^���ēǂݍ���(�V�����A�j���[�V�����f�[�^��Ԃ��B���s������p�X�g����폜��,nullptr��Ԃ�)
		shared_ptr<AnimationData> LoadAnimationDataFromBinary(const std::string&,const void*,unsigned int,bool = false); //�o�C�i���f�[�^���炩��A�j���[�V�����f�[�^�ǂݍ��݁B(�f�[�^���A�f�[�^�̐擪�|�C���^�A�f�[�^�T�C�Y�A���ݎ��̍ēǂݍ��݃t���O=false)(���s��nullptr)(V2,3�̂ݑΉ�)
		shared_ptr<AnimationData> LoadAnimationDataFromDataScript(const std::string&,const DataScript&,bool=false); //�f�[�^�X�N���v�g����A�j���[�V�����f�[�^�ǂݍ���(�f�[�^���A�f�[�^�X�N���v�g�A���ݎ��̍ēǂݍ��݃t���O=false)
		void SetGHandles(RefGHandleType&&); //�摜�n���h���Z�b�g
		void SetGHandles(const RefGHandleType&); //�摜�n���h���Z�b�g
		void Clean(); //�g���ĂȂ��f�[�^��|��
		void Clear(); //�S����
		enum class Version{V1,V2_};
		void SetVersion(Version);

		static std::shared_ptr<AnimationData> LoadAnimationDataFromDataScriptWithoutResist(const DataScript&, const LoadSettings&);
	private:
		AnimationDataAdmin();
		Version _version = Version::V2_;

		unordered_map < string, shared_ptr<AnimationData>> animationdata; //�A�j���[�V�����f�[�^(�t�@�C�����A�|�C���^)

		RefGHandleType _ghandles; //�摜�n���h��
		bool _ref_ghandles_flag; //�ǂݍ��ݎ��ɉ摜�n���h�����Q�Ƃ��邩�̃t���O

		int LoadAnimationData(const std::string&,std::shared_ptr<AnimationData>); //�ǂݍ��ݎd�����B_version�ɂ����V1�ȑO��V2�ȍ~���d������B
		int LoadAnimationData(const std::string&, const void*, size_t, std::shared_ptr<AnimationData>); //�ǂݍ��ݎd�����B_version�ɂ����V1�ȑO��V2�ȍ~���d������B

		static int SetAnimationDataFromAnimFileV1(std::shared_ptr<AnimationData>, const std::string&,const RefGHandleType*); //V1�ǂݍ��ݖ{��
		static int SetAnimationDataFromDataScript(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*); //V2�ȍ~�d����

		//SetAnimationDataFromDataScript
		static int SetAnimationDataFromDataScript_V2(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*);

		static int SetAnimationDataFromDataScript_V3(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*);
		static int SetAnimationDataFromDataScript_V4(std::shared_ptr<AnimationData>, const DataScript&, const RefGHandleType*);
		static int SetAnimationDataFromDataBlock_V3(std::shared_ptr<AnimationData>, std::shared_ptr<const DataBlock>, const RefGHandleType*);
	};
};
