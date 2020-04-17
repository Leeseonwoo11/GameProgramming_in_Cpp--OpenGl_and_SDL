#include "LaserPool.h"
#include "Game.h"
#include "Actor.h"

#include <iostream>

LaserPool::LaserPool(Game * game, int poolsize)
	:Actor(game),
	myGame(game),
	PoolSize(poolsize)
{
	for (int i = 0; i < PoolSize; i++) // �ʱ� Ǯ ������� ������ Ǯ ����
	{
		Laser* temp = new Laser(myGame);
		temp->SetState(Actor::EPaused);
		PooledLaser.push_back(temp);
	}
}

void LaserPool::GeneratePool() //  Ǯ�� �۾Ƽ� ������ �������� ���� ������� ���� �Լ�
{
	int count = 0;
	for (int i = 0; i < 10; i++) // ������ 10�� ���ؼ� Ǯ�� �߰����ش�.
	{
		Laser* temp = new Laser(myGame);
		temp->SetState(Actor::EPaused);
		PooledLaser.push_back(temp);
		count++; //  ������ ������ üũ�Ѵ�.
	}
	PoolSize += count; // ������ ��ŭ Ǯ����� �߰����ش�.
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
	GeneratePool(); // ���� Ǯ���� ��밡���� �������� ���ٸ� ������ �����ش�.
	for (int i = 0; i < PoolSize; i++)//�� Ŀ�� Ǯ���� ��밡���� �������� �������ش�.
	{
		if (PooledLaser[i]->GetState() == Actor::EPaused)
		{
			PooledLaser[i]->SetDeathTimer(1.0f);
			return PooledLaser[i];
		}
	}
	return nullptr;
}
