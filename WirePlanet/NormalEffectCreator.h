#pragma once
#include "CreatorTemplate.h"
#include"Effect.h"
#include<unordered_map>
class NormalEffectCreator :
	public CreatorTemplate < Effect >
{
public:
	NormalEffectCreator();
	~NormalEffectCreator();
	int Init()override;
	std::shared_ptr<Effect> Create(const std::string&)override;
	bool CheckInitIDExistence(const std::string&)const override;
private:
	int LoadFile();
	enum EffectKind{ Error, Animation_Effect };
	static EffectKind GetEffectKindEnumFromString(const std::string&);
	struct InitData{
		EffectKind effect_kind;
		std::string file_name;
	};
	std::unordered_map<std::string, InitData> init_datas;
	std::string directory; //ノーマルエフェクトファイルのディレクトリ
};
