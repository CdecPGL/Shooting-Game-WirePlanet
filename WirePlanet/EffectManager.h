#pragma once

#include"Effect.h"

template<class C>
class ObjectHolderTemplate_WithoutID;

class EffectManager
{
public:
	~EffectManager();
	EffectManager(const EffectManager&) = delete;
public:
	static EffectManager& GetInstance();

	int Init();
	int End();
	int Reset();
	int Process();
	void Clean();
	void Clear();
	void Update();
	void Draw();

	std::shared_ptr<Effect> CreateObject(const std::string&, const std::string& = "NULL"); //エフェクト作成(種類、初期化ID)
	int DeleteObject(std::weak_ptr<Effect>);
	int DeleteObject(Effect*);

	int GetObjectCount()const;
private:
	std::unique_ptr<ObjectHolderTemplate_WithoutID<Effect>> effect_holder;
	EffectManager();
	bool initialized;
};
