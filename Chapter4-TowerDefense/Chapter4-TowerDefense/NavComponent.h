#pragma once
#include "MoveComponent.h"
#include "Math.h"

class NavComponent :
	public MoveComponent
{
public:
	NavComponent(class Actor* owner, int updateorder = 10);
	void Update(float deltatime) override;
	void StartPath(const class Tile* start);
	void TurnTo(const Vector2& pos);
private:
	const class Tile* mNextNode;
};

