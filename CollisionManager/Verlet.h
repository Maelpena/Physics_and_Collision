#pragma once
#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "CollisionGrid.h"
class Link;
class Verlet
{

protected:
	Verlet();
	static Verlet* VerletInstance;

public:
	Verlet(Verlet& other) = delete;
	void operator = (const Verlet&) = delete;
	static Verlet* GetInstance();

	void Update(float);
	void Draw(sf::RenderWindow&);
	void Clear();
	void UpdatePosition(float);
	void ApplyForces();
	void ApplyConstraints();
	void Collide();
	void GridCollisionResolution();
	void UpdateLinks();
	void SolveCollisions(Cell*, Cell*);
	void SolveCollision(Circle*, Circle*);
	void SolveArrayCellsCollison(std::vector<Cell*>);
	void CheckCollisions(float);
	static float VectorDistance(sf::Vector2f, sf::Vector2f);
	void UpdateGrid();


	sf::CircleShape m_constraintShape;
	sf::Vector2f m_gravity;
	float m_subStep;
	float m_subDt;
	std::vector<Circle*> m_circles;
	std::vector<Link*> m_links;
	CollisionGrid m_grid;
};