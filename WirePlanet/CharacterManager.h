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

	//�L�����̍쐬�A�o�^�A�폜
	std::shared_ptr<CharacterBase> CreateObject(CharaAtrribtute)const;
	std::shared_ptr<CharacterBase> CreateObject(CharaAtrribtute, const std::string&)const;
	std::shared_ptr<CharacterBase> CreateAndResistObject(CharaAtrribtute);
	std::shared_ptr<CharacterBase> CreateAndResistObject(CharaAtrribtute, const std::string&);
	int ResistObject(std::shared_ptr<CharacterBase>);
	int DeleteObject(int);

	bool CheckObjectResisted(int)const;

	int GetObjectCount()const;
	std::shared_ptr<CharacterBase> GetSharedPointer(int)const;

	//WirePlanet�����ɒǉ�
	void CollisionDetect();
	int GetCollisionDetectedCount()const; //�Փ˔���񐔎擾
private:
	bool initialized;
	int id_counter;
	CharacterManager();
	//�Ǘ��I�u�W�F�N�g
	std::unique_ptr<ObjectHolderTemplate_WithID<int, CharacterBase>> chara_holder;
	//�G
	std::vector<std::shared_ptr<CharacterBase>> enemies;
	//�v���C���[
	std::vector<std::shared_ptr<CharacterBase>> players;

	std::unique_ptr<MatrixMap<CharaAtrribtute, bool>> _collision_map; //�Փ˃}�b�v
	void SetCollisionMap(); //�Փ˃}�b�v�Z�b�g

	int _col_det_cnt;
};
