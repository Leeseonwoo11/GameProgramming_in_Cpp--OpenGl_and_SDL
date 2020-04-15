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

bool Intersect(const CircleComponent & a, const CircleComponent & b) // �浹����
{
	Vector2 diff = a.GetCenter() - b.GetCenter();// �� �� ������ �Ÿ�
	float distSq = diff.LengthSq(); // ���� �Ÿ��� ������ ���Ѵ�.(�������� ���ؾ� �������Ȼ���)

	float radiiSq = a.GetRadius() + b.GetRadius(); //  �� ���� �������� ��
	radiiSq *= radiiSq; //  ���� ������ ���Ѵ�.
	
	return distSq <= radiiSq; // �������� ������ �� ũ�� ���� �浹�Ѱ��̰�
								// �Ÿ��� ������ �� ũ�� ���� �浹�� ���Ѱ��̴�.
}