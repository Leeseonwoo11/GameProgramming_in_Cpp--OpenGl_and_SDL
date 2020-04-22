#pragma once
#include "Actor.h"
#include <vector>
// 칸 한개 한개의 타일의 액터이다. 
// 타일은 주변 인접타일의 주소를 담을 수 있는 mAdjacent 백터가 존재한다.
// 각 타일은 상태가 있다. 상태에 따라서 타일의 색이 달라진다.
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
	float f;		// g값과 h값의 합, 이 값이 크면 이 타일로 오는데 소모되는 비용이 크다는것
	float g;		// 시작타일에서 지금 이 타일가지의 이동하는데 소모되는 거리값(시작타일은 0)
	float h;		// 휴리스틱 값 (이 타일에서 목표타일까지의 거리의 추정치(시작타일은 휴리스틱이 0))
	bool mInOpenSet; // 열린목록판별값: 열린목록에 있는 타일인지 아닌지 true false
	bool mInClosedSet; //닫힌목록판별값:  닫힌목록에 있는 타일인지 아닌지 true false
	bool mBlocked; //  막힌블록인지 아닌지 (타워가 들어서면 막힌블록이 된다.)

	void UpdateTexture();
	class SpriteComponent* mSprite;
	TileState mTileState;
	bool mSelected;
};

