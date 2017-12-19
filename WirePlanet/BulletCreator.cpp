#include "BulletCreator.h"
#include"StraightBullet.h"
#include"Boomerang.h"
#include"GuidedBullet.h"
#include"SVLoader.h"
#include"boost\lexical_cast.hpp"
#include"FileLoadUtility.h"

using namespace std;

const string Bullet_Init_Data_File("BulletInitData.csv"); //�e������k�f�[�^�t�@�C����
const int Bullet_Init_Param_Count(5); //�e�̏������f�[�^�v�f��
const string StraightBullet_ID("Straight");
const string Boomerang_ID("Boomerang");
const string GuidedBullet_ID("Guided");
//const string FallBullet_ID("Fall");

BulletCreator::InitData::InitData(InitFunc mif, const string& maf,int m_bm,int m_bp,int m_col) :init_func(mif), action_file(maf),blend_mode(m_bm),blend_param(m_bp),col(m_col){}

//�e�N���G�[�^
int BulletCreator::Init(){
	//�������֐�����ƃZ�b�g
	init_func_list.insert(pair<string,InitFunc>(StraightBullet_ID,&BulletCreator::GetStraightBullet));
	init_func_list.insert(pair<string, InitFunc>(Boomerang_ID, &BulletCreator::GetBoomerang));
	init_func_list.insert(pair<string, InitFunc>(GuidedBullet_ID, &BulletCreator::GetGuidedBullet));
//	init_func_list.insert(pair<string, InitFunc>(FallBullet_ID, &GetFallBullet));

	//�������f�[�^�ǂݍ���
	if (LoadInitData()){
		printf("BulletCreator�̏������Ɏ��s���܂����B:�������f�[�^�̓ǂݍ��݂Ɏ��s���܂����B\n");
		return -1;
	}

	printf("BulletCreator�����������܂����B\n");
	CreatorTemplate<CharacterBase>::Init();
	return 0;
}

shared_ptr<CharacterBase> BulletCreator::Create(const string& init_id){
	auto it = init_datas.find(init_id);
	if (it == init_datas.end()){ return nullptr; }
	return (this->*((*it).second.init_func))((*it).second.blend_mode,(*it).second.blend_param,(*it).second.col,(*it).second.action_file);
}

bool BulletCreator::CheckInitIDExistence(const string& init_id)const{
	if (init_datas.find(init_id) == init_datas.end()){ return false; }
	return true;
}

int BulletCreator::LoadInitData(){
	CSVLoader csvl;
	if (FileLoadUtil::LoadCSVFromFLM(&csvl,Bullet_Init_Data_File,true)){ return -1; }
	for (auto it = csvl.begin() + 1; it != csvl.end(); ++it){
		if ((*it).size() != Bullet_Init_Param_Count + 1){
			printf("BulletCreator::LoadInitData()�ŃG���[���������܂����B: �t�@�C������ǂݍ��񂾃f�[�^�̃p�����[�^������v���܂���B�X�L�b�v���Ď��̃f�[�^��ǂݍ��݂܂��B(%s:%d/%d)\n", (*it).size() == 0 ? "NULL" : (*it)[0].c_str(), (*it).size(), Bullet_Init_Param_Count + 1);
			continue;
		}
		vector<string>& i = (*it);
		using namespace boost;
		try{
			auto it = init_func_list.find(i[1]);
			if (it == init_func_list.end()){
				printf("BulletCreator::LoadInitData()�ŃG���[���������܂����B: �������f�[�^�̕ϊ��Ɏ��s���܂����B�w�肳�ꂽ����͑��݂��܂���B\n");
				return -1;
			}
			char * end_ptr=nullptr;
			int col = strtol(i[5].c_str(),&end_ptr, 0);
			if (end_ptr!=&(i[5].c_str()[i[5].size()])){ throw boost::bad_lexical_cast(); }
			InitData ninid((*it).second, i[2], lexical_cast<int>(i[3]), lexical_cast<int>(i[4]), col);
			init_datas.insert(pair<string, InitData>(i[0], ninid));
		}
		catch (bad_lexical_cast&){
			printf("BulletCreator::LoadInitData()�ŃG���[���������܂����B: �������f�[�^�̕ϊ��Ɏ��s���܂����B\n");
			return -1;
		}
	}
	return 0;
}

shared_ptr<CharacterBase> BulletCreator::GetStraightBullet(int bm, int bp,int col, const string& af){
	return make_shared<StraightBullet>(bm,bp,col,af);
}

shared_ptr<CharacterBase> BulletCreator::GetBoomerang(int bm, int bp,int col, const string& af){
	return make_shared<Boomerang>( bm, bp,col, af);
}

shared_ptr<CharacterBase> BulletCreator::GetGuidedBullet(int bm, int bp, int col, const string& af){
	return make_shared<GuidedBullet>(bm, bp, col, af);
}
