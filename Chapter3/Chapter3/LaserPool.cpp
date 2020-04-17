#include "LaserPool.h"
#include "Game.h"
#include "Actor.h"

#include <iostream>

LaserPool::LaserPool(Game * game, int poolsize)
	:Actor(game),
	myGame(game),
	PoolSize(poolsize)
{
	for (int i = 0; i < PoolSize; i++) // 초기 풀 사이즈로 레이져 풀 생성
	{
		Laser* temp = new Laser(myGame);
		temp->SetState(Actor::EPaused);
		PooledLaser.push_back(temp);
	}
}

void LaserPool::GeneratePool() //  풀이 작아서 리턴할 레이져가 없어 재생성을 위한 함수
{
	int count = 0;
	for (int i = 0; i < 10; i++) // 생성을 10개 더해서 풀에 추가해준다.
	{
		Laser* temp = new Laser(myGame);
		temp->SetState(Actor::EPaused);
		PooledLaser.push_back(temp);
		count++; //  생성한 개수를 체크한다.
	}
	PoolSize += count; // 생성한 만큼 풀사이즈에 추가해준다.
}


Laser * LaserPool::GetLaser()
{
	for (int i = 0; i < PoolSize; i++)
	{
		if (PooledLaser[i]->GetState() == Actor::EPaused)
		{
			PooledLaser[i]->SetDeathTimer(1.0f);
			return PooledLaser[i];
		}
	}
	GeneratePool(); // 만약 풀에서 사용가능한 레이져가 없다면 생성을 더해준다.
	for (int i = 0; i < PoolSize; i++)//더 커진 풀에서 사용가능한 레이져를 리턴해준다.
	{
		if (PooledLaser[i]->GetState() == Actor::EPaused)
		{
			PooledLaser[i]->SetDeathTimer(1.0f);
			return PooledLaser[i];
		}
	}
	return nullptr;
}
