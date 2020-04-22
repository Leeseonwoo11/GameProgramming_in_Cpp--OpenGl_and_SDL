#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(Game * game)
	:Actor(game),
	mSelectedTile(nullptr)
{
	mTiles.resize(NumRows); // 타일백터의 사이즈를 지정된 값으로 초기화한다.
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	for (size_t i = 0; i < NumRows; i++) // 만들어놓은 타일백터에 타일을 생성해서 집어넣어준다.
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame()); //타일을 생성한다.
			mTiles[i][j]->SetPosition(Vector2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize)); // 지정된 위치에 맞게 포지션을 정해준다.
		}
	}

	GetStartTile()->SetTileState(Tile::EStart); // 시작점의 타일을 정해준다.
	GetEndTile()->SetTileState(Tile::EBase); //  목표점의 타일을 정해준다.

	for (size_t i = 0; i < NumRows; i++) // 각 타일을 돌면서 인접타일을 타일클래스안에 추가해준다. (각각의 타일은 최소 2개 최대 4개의 인접타일을 가진다.)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0) 
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}
			if (i < NumRows-1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}
			if (j < NumCols-1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	FindPath(GetEndTile(), GetStartTile()); // 최초에 아무 타워도 없을 때 적이 지나갈 경로를 찾아준다.
	UpdatePathTiles(GetStartTile()); //  찾아낸 경로에 맞게 타일을 업데이트한다.

	mNextEnemy = EnemyTime; // 초기 적 생성 쿨타임을 초기화 해준다.
}
void Grid::SelectTile(size_t row, size_t col) // 선택한 타일
{
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) // 선택한 타일이 시작/목표 타일이 아니라면
	{
		if (mSelectedTile) //mSelectedTile이 널이 아니라면 이전에 클릭한 타일이 저장되어있는것이므로
		{				   
			mSelectedTile->ToggleSelect(); // 원래의 택스쳐로 바꾸기위해 toogle실행
		}
		mSelectedTile = mTiles[row][col]; // mSelectedTile을 초기화 해준다.
		mSelectedTile->ToggleSelect(); // 선택한 타일모양으로 toggle해준다.
	}

}
void Grid::ProcessClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

/*
	FindPath 순서
	0 모든 타일을 돌면서 mInOpenSet/mInClosedSet/g값을 초기화해준다. 

	1  current를 시작타일로 만들어줌
	2 시작값 닫힌목록값으로 초기화

	3 current의 인접타일을 돈다.
		3.0 해당 인접타일이 막혀있다면 다음 인접타일로 넘어간다. (mInClosedSet ==false && mInOpenSet == false )

		3.1 해당 인접타일이 닫힌목록에 있지 않고 열린목록에도 없다면 
			3.1.1 인접타일의 mParent를 current타일로 초기화 해준다.
			3.1.2 인접타일의 h,g,f값을 구한다.
			3.1.3 해당 인접타일을 열린목록(openSet)에 넣어준다.
			3.1.4 인접타일의 열린목록 판별값을 true로 바꿔준다.
			3.1.5 current의 모든 인접타일을 다 돌때까지 3을 반복한다.

		3.2 해당 인접타일이 닫힌목록에 없지면 열린목록에는 있다면 (mInClosedSet ==false && mInOpenSet == true )
			3.2.1 current타일에서 해당인접타일까지의 거리값(newG)이
				해당인접타일(neighbor)의 g값보다 작다면(해당 인접타일의 g값은 다른타일을 돌아돌아 와서 g값이 클수있기때문에)
			3.2.2 해당인접타일의 부모를 currnet타일로 바꿔준다.
			3.2.3 해당인접타일의 g값도 newG값으로 바꿔준다.
			3.2.4 해당인접타일의 f값도 바꿔준다.
			3.2.5 current의 모든 인접타일을 다 돌때까지 3을 반복한다.

	4 current의 인접타일을 다 돌아서 열린목록에 넣어주었다면 열린목록에서 f 값이 가장작은 타일을 구함
	5 current타일을 5번에서 구한 타일로 바꿔준다.
	6 5번에서 구한 타일을 열린목록에서 삭제해주고 열린목록에 없다고 mInOpenSet 를 false로 초기화한다.
	7 바꿔진 current타일을 닫힌목록에 있다고 mInClosedSet값을 true로 초기화한다.
	8 current타일이 goal 타일이 아니라면 3번으로 넘어간다.

	9 current타일이 goal 이 된다면 루프를 빠져나간다.
	10 true 로 리턴되었다면 start의 부모를 따라가다보면 goal 타일이 나오게 된다.
*/

bool Grid::FindPath(Tile * start, Tile * goal) // 시작점에서 목표점까지 경로가 존재한다면 true 없다면 false 를 리턴한다.
{												// 경로가 있다면 시작점(start)의 parent를 따라가다보면 결국 목표점의 tile을 가리키게된다.
	for (size_t i = 0; i < NumRows; i++)		
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j]->g = 0.0f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;
	Tile* current = start; // 현재 타일을 시작타일로 초기화한다.
	current->mInClosedSet = true; // 시작점이므로 시작타일을 닫힌목록에 넣어준다.

	do
	{
		for (Tile* neighbor : current->mAdjacent)
		{
			if (neighbor->mBlocked)
			{
				continue;
			}

			if (!neighbor->mInClosedSet)
			{
				if (!neighbor->mInOpenSet)
				{
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				else
				{
					float newG = current->g + TileSize;
					if (newG < neighbor->g)
					{
						neighbor->mParent = current;
						neighbor->g = newG;
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}
		if (openSet.empty())
		{
			break;
		}
		auto iter = std::min_element(openSet.begin(), openSet.end(), 
			[](Tile* a, Tile* b) 
			{
				return a->f < b->f; 
			});// 열린목록에서 가장 작은 f값을 찾아낸다.

		current = *iter;// 현재타일을 열린목록에서 가장 작은 f값을 가진 타일로 지정한다.
		openSet.erase(iter); // 열린목록에서 가장작은 f값을 가진 타일을 지워준다.
		current->mInOpenSet = false; // 이 값을 열린목록판단값을 false로 헤제해준다. 
		current->mInClosedSet = true; // 닫힌목록판단값을 true로 활성화해준다.
	}
	while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::BuildTower() 
{
	if (mSelectedTile && !mSelectedTile->mBlocked) //  선택된 타일이 nullptr이 아니고 막혀있는 상태가 아닐때
	{
		mSelectedTile->mBlocked = true; // 선택된 타일을 막혀있다고 초기화한다.
		if (FindPath(GetEndTile(), GetStartTile())) //  그리고 시작점에서 목표점까지 경로가 존재하는지 파악한다.
		{
			// 지정한 부분을 막았는데도 경로가 존재한다면 지정한 타일에 타워를 세운다.
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else// 지정한 부분을 막는다면 시작점에서 목표점까지 경로가 존재하지 않는다면
		{
			mSelectedTile->mBlocked = false; //선택된 타일을 막아놨다고 한것을 다시 열어놓는다.
			FindPath(GetEndTile(), GetStartTile()); // 막아논 블록때문에 막힌길로 안내하는 상태이므로 경로를 다시 찾아 제대로 된 경로를 만들어 놓는다.
		}
		UpdatePathTiles(GetStartTile()); //제대로된 경로에 맞게 업데이트를 한다.
	}
}

Tile * Grid::GetStartTile()
{
	return mTiles[3][0]; // 시작지점의 타일
}

Tile * Grid::GetEndTile()
{
	return mTiles[3][15]; // 목표지점의 타일 
}

void Grid::UpdateActor(float deltatime)
{
	Actor::UpdateActor(deltatime);

	mNextEnemy -= deltatime;
	if (mNextEnemy <= 0.0f) // 적 생성 쿨타임이 돌아오면
	{
		new Enemy(GetGame()); // 적을 생성하고
		mNextEnemy += EnemyTime; // 쿨타임을 돌린다.
	}
}



void Grid::UpdatePathTiles(Tile * start)
{
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	Tile *t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}
