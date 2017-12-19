#include "NormalEffectCreator.h"

//#include"InitDatas.h"
#include"FileLoadUtility.h"
#include"SVLoader.h"
#include"boost\lexical_cast.hpp"
#include"AnimationEffect.h"

using namespace std;

const string EFCT_LIST_NAME("efct_list.csv");
const int LIST_ITEM_NUM(3); //���X�g�̍��ڐ�

NormalEffectCreator::NormalEffectCreator()
{
}


NormalEffectCreator::~NormalEffectCreator()
{
}

int NormalEffectCreator::Init(){
	if (LoadFile()){
		printf("NormalEffectCreator�̏������Ɏ��s���܂����BNormalEffect���X�g�̓ǂݍ��݂Ɏ��s���܂����B\n");
		return -1;
	}
	CreatorTemplate<Effect>::Init();
	printf("NormalEffectCreator������������܂����B\n");
	return 0;
}

shared_ptr<Effect> NormalEffectCreator::Create(const string& init_id){
	auto it = init_datas.find(init_id);
	if (it == init_datas.end()){ return nullptr; }
	shared_ptr<Effect> out;
	switch ((*it).second.effect_kind)
	{
	case Animation_Effect:
		out = make_shared<AnimationEffect>((*it).second.file_name);
		return out;
		break;
	default:
		break;
	}
	return nullptr;
}

bool NormalEffectCreator::CheckInitIDExistence(const string& init_id)const{
	return !(init_datas.find(init_id)==init_datas.end());
}

int NormalEffectCreator::LoadFile(){
	try{
		//���X�g���擾
//		string list_pass = InitDatas::GetInitDatas("Resource").at("NormalEffectList");
		string list_pass = EFCT_LIST_NAME;
		//���X�g��ǂݍ���
		CSVLoader scvl;
//		scvl.LoadCSV(list_pass);
		if (FileLoadUtil::LoadCSVFromFLM(&scvl, EFCT_LIST_NAME, true)){
			return -2;
		}
		//Directory�擾
		/*const string dir = list_pass.substr(0, list_pass.find_last_of("\\")) + "\\";
		directory = dir;*/
		for (vector<string>& s : scvl){
			if (s.size() != LIST_ITEM_NUM){
				printf("���X�g�̍��ڐ����s���ł��B(%d/%d)\n", s.size(),LIST_ITEM_NUM);
				continue;
			}
			InitData inid;
			//�G�t�F�N�g�̎�ގ擾
			inid.effect_kind = GetEffectKindEnumFromString(s[1]);
			switch (inid.effect_kind){
			case Animation_Effect:
				//�A�j���t�@�C�����擾
				inid.file_name = s[2];
				break;
			default:
				printf("NormalEffect�̎�ނ��s���ł��B(%s)\n", s[1].c_str());
				continue;
				break;
			}
			init_datas.insert(unordered_map<string, InitData>::value_type(s[0],inid));
		}
		return 0;
	}
	catch (out_of_range& e){
		printf("�����f�[�^���s�����Ă��܂��B(%s)\n", e.what());
		return -1;
	}
	/*catch (InitDatas::init_datas_error& e){
		printf("�����f�[�^�̃Z�N�V�������s�����Ă��܂��B(%s)\n", e.what());
		return -1;
	}*/
}

//�����񂩂�G�t�F�N�g��ʗ񋓑̎擾
NormalEffectCreator::EffectKind NormalEffectCreator::GetEffectKindEnumFromString(const string& str){
	if (str == "AnimationEffect"){
		return Animation_Effect;
	}
	else{
		return Error;
	}
}
