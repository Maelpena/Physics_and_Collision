#include "CollisionGrid.h"
#include "Circle.h"
CollisionGrid::CollisionGrid()
{

}

CollisionGrid::CollisionGrid(sf::Vector2f size)
{
	m_gridSize = size;
	m_cellSize = Circle::maxRadius * 2;
	int nbCellsX = m_gridSize.x / m_cellSize;
	int nbCellsY = m_gridSize.y / m_cellSize;

	for (int i = 0; i <= nbCellsX; i++)
	{
		std::vector<Cell*> newRow;
		m_cells.push_back(newRow);

		for (int j = 0; j <= nbCellsY; j++)
		{
			m_cells.at(i).push_back(new Cell());
		}
	}

}

void CollisionGrid::UpdateGrid()
{
	int nbCellsX = m_gridSize.x / m_cellSize;
	int nbCellsY = m_gridSize.y / m_cellSize;
	for (int i = 0; i <= nbCellsX; i++)
	{
		for (int j = 0; j <= nbCellsY; j++)
		{
			delete(m_cells[i][j]);
		}
		m_cells[i].clear();
	}
	m_cells.clear();

	m_cellSize = Circle::maxRadius * 2;
	nbCellsX = m_gridSize.x / m_cellSize;
	nbCellsY = m_gridSize.y / m_cellSize;

	for (int i = 0; i <= nbCellsX; i++)
	{
		std::vector<Cell*> newRow;
		m_cells.push_back(newRow);

		for (int j = 0; j <= nbCellsY; j++)
		{
			m_cells.at(i).push_back(new Cell());
		}
	}

}
