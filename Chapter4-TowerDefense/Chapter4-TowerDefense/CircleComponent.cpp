#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor * owner)
	:Component(owner),
	mRadius(0.0f)
{
}

float CircleComponent::GetRadius() const
{
	return mOwner->GetScale()* mRadius;
}

const Vector2 & CircleComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

bool Intersect(const CircleComponent & a, const CircleComponent & b)
{
	Vector2 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();
	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;
	return radiiSq >= distSq; // 반지름의 제곱이 더 크면 둘이 겹친것이므로 true 리턴 아니면 false 리턴
}
