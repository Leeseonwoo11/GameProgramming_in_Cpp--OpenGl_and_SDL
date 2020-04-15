#include "CircleComponent.h"
#include "Actor.h"



CircleComponent::CircleComponent(Actor * owner)
	:Component(owner),
mRadius(0.0f)
{

}

float CircleComponent::GetRadius() const
{
	return mOwner->GetScale() * mRadius;
}

const Vector2 & CircleComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

bool Intersect(const CircleComponent & a, const CircleComponent & b) // 충돌감지
{
	Vector2 diff = a.GetCenter() - b.GetCenter();// 두 원 사이의 거리
	float distSq = diff.LengthSq(); // 사이 거리의 제곱을 구한다.(제곱으로 비교해야 음수가안생김)

	float radiiSq = a.GetRadius() + b.GetRadius(); //  각 원의 반지름의 합
	radiiSq *= radiiSq; //  합의 제곱을 구한다.
	
	return distSq <= radiiSq; // 반지름의 제곱이 더 크면 둘이 충돌한것이고
								// 거리의 제곱이 더 크면 둘이 충돌을 안한것이다.
}