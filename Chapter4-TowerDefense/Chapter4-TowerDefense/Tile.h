#pragma once
#include "Actor.h"
#include <vector>
// ĭ �Ѱ� �Ѱ��� Ÿ���� �����̴�. 
// Ÿ���� �ֺ� ����Ÿ���� �ּҸ� ���� �� �ִ� mAdjacent ���Ͱ� �����Ѵ�.
// �� Ÿ���� ���°� �ִ�. ���¿� ���� Ÿ���� ���� �޶�����.
class Tile :
	public Actor
{
public:
	friend class Grid;
	enum TileState 
	{
		EDefault,
		EPath,
		EStart,
		EBase
	};
	Tile(class Game* game);
	void SetTileState(TileState state);
	TileState GetTileState() const { return mTileState; }
	void ToggleSelect();
	const Tile* GetParent() const { return mParent; }
private:
	std::vector<Tile*>mAdjacent;
	Tile* mParent;	//
	float f;		// g���� h���� ��, �� ���� ũ�� �� Ÿ�Ϸ� ���µ� �Ҹ�Ǵ� ����� ũ�ٴ°�
	float g;		// ����Ÿ�Ͽ��� ���� �� Ÿ�ϰ����� �̵��ϴµ� �Ҹ�Ǵ� �Ÿ���(����Ÿ���� 0)
	float h;		// �޸���ƽ �� (�� Ÿ�Ͽ��� ��ǥŸ�ϱ����� �Ÿ��� ����ġ(����Ÿ���� �޸���ƽ�� 0))
	bool mInOpenSet; // ��������Ǻ���: ������Ͽ� �ִ� Ÿ������ �ƴ��� true false
	bool mInClosedSet; //��������Ǻ���:  ������Ͽ� �ִ� Ÿ������ �ƴ��� true false
	bool mBlocked; //  ����������� �ƴ��� (Ÿ���� ���� ��������� �ȴ�.)

	void UpdateTexture();
	class SpriteComponent* mSprite;
	TileState mTileState;
	bool mSelected;
};

