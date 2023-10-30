#pragma once
#include <SFML/Graphics.hpp>
class Circle;
struct Cell
{
	Cell();
	std::vector<Circle*> circles;
};