#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(Game * game)
	:Actor(game),
	mSelectedTile(nullptr)
{
}

void Grid::ProcessClick(int x, int y)
{
}

bool Grid::FindPath(Tile * start, Tile * goal)
{
	return false;
}

void Grid::BuildTower()
{
}

Tile * Grid::GetStartTile()
{
	return nullptr;
}

Tile * Grid::GetEndTile()
{
	return nullptr;
}

void Grid::UpdateActor(float deltatime)
{
}

void Grid::SelectTile(size_t row, size_t col)
{
}

void Grid::UpdatePathTiles(Tile * start)
{
}
