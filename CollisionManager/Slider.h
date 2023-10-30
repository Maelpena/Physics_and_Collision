#pragma once
#include <SFML/Graphics.hpp>

struct Slider {
	Slider(std::string, float*, sf::Vector2f, int, int, int);
	void Draw(sf::RenderWindow& window);

	void Move(float xPos);

	float* m_paramValue;
	sf::Text m_paramText;
	sf::Vector2f m_position;
	sf::Font m_font;
	int m_minValue;
	int m_maxValue;
	float m_Xsize;
	int m_Ysize;
	float m_pixelStep;
	float m_circleRadius;

	sf::RectangleShape m_axisRectangle;
	sf::CircleShape m_circle;

};