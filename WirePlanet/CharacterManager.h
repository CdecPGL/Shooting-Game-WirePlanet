#pragma once
#include"CharacterBase.h"

class Planet;
template<typename T, class C>
class ObjectHolderTemplate_WithID;
template<typename T, typename U>
class MatrixMap;

class CharacterManager
{
public:
	~CharacterManager();
	CharacterManager(const CharacterManager&) = delete;
	static CharacterManager& GetInstance();

	int Init();
	int End();
	int Reset();
	int Process();
	void Clean();
	void Clear();
	void Update();
	void Draw();

	//キャラの作成、登録、削除
	std::shared_ptr<CharacterBase> CreateObject(CharaAtrribtute)const;
	std::shared_ptr<CharacterBase> CreateObject(CharaAtrribtute, const std::string&)const;
	std::shared_ptr<CharacterBase> CreateAndResistObject(CharaAtrribtute);
	std::shared_ptr<CharacterBase> CreateAndResistObject(CharaAtrribtute, const std::string&);
	int ResistObject(std::shared_ptr<CharacterBase>);
	int DeleteObject(int);

	bool CheckObjectResisted(int)const;

	int GetObjectCount()const;
	std::shared_ptr<CharacterBase> GetSharedPointer(int)const;

	//WirePlanet向けに追加
	void CollisionDetect();
	int GetCollisionDetectedCount()const; //衝突判定回数取得
private:
	bool initialized;
	int id_counter;
	CharacterManager();
	//管理オブジェクト
	std::unique_ptr<ObjectHolderTemplate_WithID<int, CharacterBase>> chara_holder;
	//敵
	std::vector<std::shared_ptr<CharacterBase>> enemies;
	//プレイヤー
	std::vector<std::shared_ptr<CharacterBase>> players;

	std::unique_ptr<MatrixMap<CharaAtrribtute, bool>> _collision_map; //衝突マップ
	void SetCollisionMap(); //衝突マップセット

	int _col_det_cnt;
};
