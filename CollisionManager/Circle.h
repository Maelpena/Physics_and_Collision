#include <SFML/Graphics.hpp>

#pragma once
class Circle
{
public:
	Circle();
	Circle(sf::Vector2f);
	Circle(bool, sf::Vector2f, bool, sf::Vector2f);

	~Circle();
	void Update(float);
	void Accelerate(sf::Vector2f);	

	sf::CircleShape m_shape;
	static float globalRadius;
	static float maxRadius;
	static float minRadius;
	static float nextRadius;
	static float lastGlobalRadius;
	float m_radius;
	bool m_isLinked;
	sf::Vector2f m_currentPos;
	sf::Vector2f m_lastPos;
	sf::Vector2f m_acceleration;
	bool m_isFixed;
};
