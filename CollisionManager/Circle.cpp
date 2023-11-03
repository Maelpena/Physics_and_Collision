#include "Circle.h"
#include "Verlet.h"
#include <iostream>

float Circle::globalRadius = 20;
float Circle::maxRadius = globalRadius;
float Circle::minRadius = 5;
float Circle::nextRadius = globalRadius;
float Circle::lastGlobalRadius = globalRadius;


Circle::Circle() : Circle(false, { rand() % 500 + 250.f, rand() % 400 + 150.f }, false, {0,0})
{
	
}
Circle::Circle(sf::Vector2f direction) : Circle(false, {rand() % 500 + 250.f, rand() % 400 + 150.f}, false, direction)
{

}
Circle::Circle(bool isFixed, sf::Vector2f position, bool isLinked, sf::Vector2f direction)
{
	if (lastGlobalRadius != globalRadius)
	{
		lastGlobalRadius = globalRadius;
		Circle::nextRadius = globalRadius;

	}
	//m_radius = static_cast<float>(rand() % 3) + 5;
	m_isLinked = isLinked;


	if (Circle::nextRadius > Circle::maxRadius)
	{
		Circle::maxRadius = Circle::nextRadius;
		Verlet::GetInstance()->UpdateGrid();
	}

	m_radius = Circle::nextRadius;
	
	//Circle::globalRadius = 33;
	Circle::nextRadius = Circle::globalRadius + rand() % 16 - 8;
	std::cout << Circle::nextRadius << std::endl;
	//Circle::nextRadius = Circle::globalRadius ;
	
	//Circle::globalRadius = 5;


	//std::cout << "m_radius" << m_radius;
	//m_radius = 4.f;
	m_shape = sf::CircleShape(m_radius);
	//m_shape.setFillColor(sf::Color::White);
	m_shape.setFillColor(sf::Color( 50, rand() % 140, rand() % 230, 255));
	m_currentPos = position;
	m_lastPos = m_currentPos - (direction *  20.f / Verlet::GetInstance()->m_subStep);
	//sf::Vector2f dir(0.f, 0.1f);
	m_shape.setPosition(m_currentPos);
	m_shape.setPointCount(32);
	m_shape.setOrigin(m_radius, m_radius);
	m_isFixed = isFixed;
}


void Circle::Update(float deltaTime)
{
	const sf::Vector2f velocity = m_currentPos - m_lastPos;
	m_lastPos = m_currentPos;
	m_currentPos = m_currentPos + velocity + m_acceleration * (deltaTime * deltaTime);
	m_acceleration = {};
	m_shape.setPosition(m_currentPos);

}

void Circle::Accelerate(sf::Vector2f acceleration)
{
	m_acceleration += acceleration;
}

Circle::~Circle() 
{
}