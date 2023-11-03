#include "Slider.h"
#include <iostream>

Slider::Slider(std::string paramText, float* paramValue, sf::Vector2f position, int minValue, int maxValue, int Xsize)
{



	m_paramValue = paramValue;
	m_position = position;
	m_minValue = minValue;
	m_maxValue = maxValue;

	m_Xsize = Xsize;
	m_pixelStep = m_Xsize / (m_maxValue - m_minValue);
	m_Ysize = 8;
	m_circleRadius = 12;
	m_axisRectangle = sf::RectangleShape(sf::Vector2f(Xsize + m_circleRadius, m_Ysize ));
	m_axisRectangle.setFillColor(sf::Color(0x404040FF));
	m_axisRectangle.setPosition(m_position);
	m_circle = sf::CircleShape(m_circleRadius);
	m_circle.setFillColor(sf::Color(0xBFBFBFFF));
	m_circle.setOrigin(m_circleRadius, m_circleRadius);
	m_circle.setPosition(static_cast<int>((*m_paramValue - m_minValue) * m_pixelStep + m_position.x + 9), m_position.y + m_circleRadius / 3);
	//m_circle.setPosition(m_position.x, m_position.y + m_circleRadius/3);
	m_font.loadFromFile("./Fonts/OpenSans-Regular.ttf");

	m_paramText.setFont(m_font);
	m_paramText.setString(paramText + " :");
	m_paramText.setPosition(position.x, position.y - 40);
	m_paramText.setCharacterSize(20);

	m_valueText.setFont(m_font);
	m_valueText.setString(std::to_string(static_cast<int>(*m_paramValue)));
	m_valueText.setPosition(position.x + Xsize + m_circleRadius + 20, position.y - 10);
	m_valueText.setCharacterSize(20);
}


void Slider::Draw(sf::RenderWindow& window)
{
	window.draw(m_axisRectangle);
	window.draw(m_circle);

	window.draw(m_paramText);
	window.draw(m_valueText);

}

void Slider::Move(float xPos)
{
	if (xPos < m_position.x)
		xPos = m_position.x;
	else if (xPos > (m_position.x + m_Xsize))
		xPos = m_position.x + m_Xsize;

	float range = m_maxValue - m_minValue;
	*m_paramValue = static_cast<int>(m_minValue + ((xPos - m_position.x) / m_Xsize) * range);
	m_circle.setPosition(static_cast<int>((*m_paramValue - m_minValue) * m_pixelStep + m_position.x + 9), m_position.y + m_circleRadius/3);

	m_valueText.setString(std::to_string(static_cast<int>(*m_paramValue)));

}