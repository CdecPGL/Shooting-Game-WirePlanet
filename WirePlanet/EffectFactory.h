#pragma once

#include"Effect.h"
#include<unordered_map>
#include"CreatorTemplate.h"

class EffectFactory
{
public:
	EffectFactory(const EffectFactory&) = delete;
	~EffectFactory();
	static EffectFactory& GetInstance();

	int Init();
	std::shared_ptr<Effect> CreateObject(const std::string&, const std::string& = "NULL"); //�L�����쐬�B(�G�t�F�N�g���,�L����������ID)
	bool CheckInitDataExistence(const std::string&, const std::string& = "NULL")const;
	void ClearInitDataList();

private:
	EffectFactory();
	using creator_map = std::unordered_map < std::string, std::unique_ptr<CreatorTemplate<Effect>> > ;
	bool initialized;
	creator_map creators;
};
