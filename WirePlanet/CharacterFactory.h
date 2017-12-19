#pragma once

#include"CharacterBase.h"
#include"CreatorTemplate.h"
#include<string>
#include<unordered_map>

//�V���O���g��

class CharacterFactory
{
public:
	~CharacterFactory();
	CharacterFactory(const CharacterFactory&) = delete;
	static CharacterFactory& GetInstance();

	int Init();
	std::shared_ptr<CharacterBase> CreateObject(CharaAtrribtute, const std::string& = "NULL"); //�L�����쐬�B(�L���������A�L����������ID)
	bool CheckInitDataExistence(CharaAtrribtute, const std::string&)const;
	void ClearInitDataList();

private:
	using creator_map = std::unordered_map<CharaAtrribtute, std::unique_ptr<CreatorTemplate<CharacterBase>>>;
	bool initialized;
	CharacterFactory();
	creator_map creators;
};

