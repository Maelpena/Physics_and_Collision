#pragma once
#include <SFML/Graphics.hpp>
#include "Cell.h"

struct CollisionGrid
{
	CollisionGrid();
	CollisionGrid(sf::Vector2f);
	void UpdateGrid();
	float m_cellSize;
	sf::Vector2f m_gridSize;
	std::vector<std::vector<Cell*>> m_cells;
};