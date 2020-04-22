#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(Game * game)
	:Actor(game),
	mSelectedTile(nullptr)
{
	mTiles.resize(NumRows); // Ÿ�Ϲ����� ����� ������ ������ �ʱ�ȭ�Ѵ�.
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	for (size_t i = 0; i < NumRows; i++) // �������� Ÿ�Ϲ��Ϳ� Ÿ���� �����ؼ� ����־��ش�.
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame()); //Ÿ���� �����Ѵ�.
			mTiles[i][j]->SetPosition(Vector2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize)); // ������ ��ġ�� �°� �������� �����ش�.
		}
	}

	GetStartTile()->SetTileState(Tile::EStart); // �������� Ÿ���� �����ش�.
	GetEndTile()->SetTileState(Tile::EBase); //  ��ǥ���� Ÿ���� �����ش�.

	for (size_t i = 0; i < NumRows; i++) // �� Ÿ���� ���鼭 ����Ÿ���� Ÿ��Ŭ�����ȿ� �߰����ش�. (������ Ÿ���� �ּ� 2�� �ִ� 4���� ����Ÿ���� ������.)
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

	FindPath(GetEndTile(), GetStartTile()); // ���ʿ� �ƹ� Ÿ���� ���� �� ���� ������ ��θ� ã���ش�.
	UpdatePathTiles(GetStartTile()); //  ã�Ƴ� ��ο� �°� Ÿ���� ������Ʈ�Ѵ�.

	mNextEnemy = EnemyTime; // �ʱ� �� ���� ��Ÿ���� �ʱ�ȭ ���ش�.
}
void Grid::SelectTile(size_t row, size_t col) // ������ Ÿ��
{
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) // ������ Ÿ���� ����/��ǥ Ÿ���� �ƴ϶��
	{
		if (mSelectedTile) //mSelectedTile�� ���� �ƴ϶�� ������ Ŭ���� Ÿ���� ����Ǿ��ִ°��̹Ƿ�
		{				   
			mSelectedTile->ToggleSelect(); // ������ �ý��ķ� �ٲٱ����� toogle����
		}
		mSelectedTile = mTiles[row][col]; // mSelectedTile�� �ʱ�ȭ ���ش�.
		mSelectedTile->ToggleSelect(); // ������ Ÿ�ϸ������ toggle���ش�.
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
	FindPath ����
	0 ��� Ÿ���� ���鼭 mInOpenSet/mInClosedSet/g���� �ʱ�ȭ���ش�. 

	1  current�� ����Ÿ�Ϸ� �������
	2 ���۰� ������ϰ����� �ʱ�ȭ

	3 current�� ����Ÿ���� ����.
		3.0 �ش� ����Ÿ���� �����ִٸ� ���� ����Ÿ�Ϸ� �Ѿ��. (mInClosedSet ==false && mInOpenSet == false )

		3.1 �ش� ����Ÿ���� ������Ͽ� ���� �ʰ� ������Ͽ��� ���ٸ� 
			3.1.1 ����Ÿ���� mParent�� currentŸ�Ϸ� �ʱ�ȭ ���ش�.
			3.1.2 ����Ÿ���� h,g,f���� ���Ѵ�.
			3.1.3 �ش� ����Ÿ���� �������(openSet)�� �־��ش�.
			3.1.4 ����Ÿ���� ������� �Ǻ����� true�� �ٲ��ش�.
			3.1.5 current�� ��� ����Ÿ���� �� �������� 3�� �ݺ��Ѵ�.

		3.2 �ش� ����Ÿ���� ������Ͽ� ������ ������Ͽ��� �ִٸ� (mInClosedSet ==false && mInOpenSet == true )
			3.2.1 currentŸ�Ͽ��� �ش�����Ÿ�ϱ����� �Ÿ���(newG)��
				�ش�����Ÿ��(neighbor)�� g������ �۴ٸ�(�ش� ����Ÿ���� g���� �ٸ�Ÿ���� ���Ƶ��� �ͼ� g���� Ŭ���ֱ⶧����)
			3.2.2 �ش�����Ÿ���� �θ� currnetŸ�Ϸ� �ٲ��ش�.
			3.2.3 �ش�����Ÿ���� g���� newG������ �ٲ��ش�.
			3.2.4 �ش�����Ÿ���� f���� �ٲ��ش�.
			3.2.5 current�� ��� ����Ÿ���� �� �������� 3�� �ݺ��Ѵ�.

	4 current�� ����Ÿ���� �� ���Ƽ� ������Ͽ� �־��־��ٸ� ������Ͽ��� f ���� �������� Ÿ���� ����
	5 currentŸ���� 5������ ���� Ÿ�Ϸ� �ٲ��ش�.
	6 5������ ���� Ÿ���� ������Ͽ��� �������ְ� ������Ͽ� ���ٰ� mInOpenSet �� false�� �ʱ�ȭ�Ѵ�.
	7 �ٲ��� currentŸ���� ������Ͽ� �ִٰ� mInClosedSet���� true�� �ʱ�ȭ�Ѵ�.
	8 currentŸ���� goal Ÿ���� �ƴ϶�� 3������ �Ѿ��.

	9 currentŸ���� goal �� �ȴٸ� ������ ����������.
	10 true �� ���ϵǾ��ٸ� start�� �θ� ���󰡴ٺ��� goal Ÿ���� ������ �ȴ�.
*/

bool Grid::FindPath(Tile * start, Tile * goal) // ���������� ��ǥ������ ��ΰ� �����Ѵٸ� true ���ٸ� false �� �����Ѵ�.
{												// ��ΰ� �ִٸ� ������(start)�� parent�� ���󰡴ٺ��� �ᱹ ��ǥ���� tile�� ����Ű�Եȴ�.
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
	Tile* current = start; // ���� Ÿ���� ����Ÿ�Ϸ� �ʱ�ȭ�Ѵ�.
	current->mInClosedSet = true; // �������̹Ƿ� ����Ÿ���� ������Ͽ� �־��ش�.

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
			});// ������Ͽ��� ���� ���� f���� ã�Ƴ���.

		current = *iter;// ����Ÿ���� ������Ͽ��� ���� ���� f���� ���� Ÿ�Ϸ� �����Ѵ�.
		openSet.erase(iter); // ������Ͽ��� �������� f���� ���� Ÿ���� �����ش�.
		current->mInOpenSet = false; // �� ���� ��������Ǵܰ��� false�� �������ش�. 
		current->mInClosedSet = true; // ��������Ǵܰ��� true�� Ȱ��ȭ���ش�.
	}
	while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::BuildTower() 
{
	if (mSelectedTile && !mSelectedTile->mBlocked) //  ���õ� Ÿ���� nullptr�� �ƴϰ� �����ִ� ���°� �ƴҶ�
	{
		mSelectedTile->mBlocked = true; // ���õ� Ÿ���� �����ִٰ� �ʱ�ȭ�Ѵ�.
		if (FindPath(GetEndTile(), GetStartTile())) //  �׸��� ���������� ��ǥ������ ��ΰ� �����ϴ��� �ľ��Ѵ�.
		{
			// ������ �κ��� ���Ҵµ��� ��ΰ� �����Ѵٸ� ������ Ÿ�Ͽ� Ÿ���� �����.
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else// ������ �κ��� ���´ٸ� ���������� ��ǥ������ ��ΰ� �������� �ʴ´ٸ�
		{
			mSelectedTile->mBlocked = false; //���õ� Ÿ���� ���Ƴ��ٰ� �Ѱ��� �ٽ� ������´�.
			FindPath(GetEndTile(), GetStartTile()); // ���Ƴ� ��϶����� ������� �ȳ��ϴ� �����̹Ƿ� ��θ� �ٽ� ã�� ����� �� ��θ� ����� ���´�.
		}
		UpdatePathTiles(GetStartTile()); //����ε� ��ο� �°� ������Ʈ�� �Ѵ�.
	}
}

Tile * Grid::GetStartTile()
{
	return mTiles[3][0]; // ���������� Ÿ��
}

Tile * Grid::GetEndTile()
{
	return mTiles[3][15]; // ��ǥ������ Ÿ�� 
}

void Grid::UpdateActor(float deltatime)
{
	Actor::UpdateActor(deltatime);

	mNextEnemy -= deltatime;
	if (mNextEnemy <= 0.0f) // �� ���� ��Ÿ���� ���ƿ���
	{
		new Enemy(GetGame()); // ���� �����ϰ�
		mNextEnemy += EnemyTime; // ��Ÿ���� ������.
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
