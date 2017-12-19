#pragma once

#include<memory>
#include<string>
#include"Laser.h"

template<typename T>
class ObjectHolderTemplate_WithoutID;
struct CollisionDatas::LaserCollisionObject;

class LaserManager
{
public:
	LaserManager(const LaserManager&) = delete;
	~LaserManager();
	static LaserManager& GetInstance();
	int Init();
	int Reset();
	std::shared_ptr<Laser> CreateLaser(const std::string&);
	int DeleteLaser(Laser*);
	void Process();
	void Update();
	void Draw();
	std::vector<CollisionDatas::LaserCollisionObject> GetLaserCollionObjects()const;
private:
	LaserManager();
	std::unique_ptr<ObjectHolderTemplate_WithoutID<Laser>> _laser_holder;
	int _laser_ghandle;
	int _laser_col_ghandle;
};
