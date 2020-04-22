#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor * owner, int updateorder)
	:MoveComponent(owner,updateorder),
	mNextNode(nullptr)
{
}

void NavComponent::Update(float deltatime)
{
	if (mNextNode)
	{
		Vector2 diff = mOwner->GetPosition()- mNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f))
		{
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}

	MoveComponent::Update(deltatime);
}

void NavComponent::StartPath(const Tile * start)
{
	mNextNode = start->GetParent();
	TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2 & pos)
{
	Vector2 dir = pos - mOwner->GetPosition();
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}
