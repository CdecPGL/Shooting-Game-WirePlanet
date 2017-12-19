#pragma once

#include"CharacterBase.h"
#include"CreatorTemplate.h"
#include<string>
#include<unordered_map>

//シングルトン

class CharacterFactory
{
public:
	~CharacterFactory();
	CharacterFactory(const CharacterFactory&) = delete;
	static CharacterFactory& GetInstance();

	int Init();
	std::shared_ptr<CharacterBase> CreateObject(CharaAtrribtute, const std::string& = "NULL"); //キャラ作成。(キャラ属性、キャラ初期化ID)
	bool CheckInitDataExistence(CharaAtrribtute, const std::string&)const;
	void ClearInitDataList();

private:
	using creator_map = std::unordered_map<CharaAtrribtute, std::unique_ptr<CreatorTemplate<CharacterBase>>>;
	bool initialized;
	CharacterFactory();
	creator_map creators;
};

