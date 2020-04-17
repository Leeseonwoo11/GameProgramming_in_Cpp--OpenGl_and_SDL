#pragma once
#include "Laser.h"
#include <vector>
class LaserPool: public Actor
{
public:
	LaserPool(class Game* game ,int poolsize);
	void GeneratePool();

	Laser* GetLaser();

private:
	int PoolSize = 0;
	std::vector<Laser*>PooledLaser;
	class Game* myGame;
};

