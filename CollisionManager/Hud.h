#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Slider.h"
struct Hud
{
	Hud();
	void ClickEvent(sf::RenderWindow&, sf::Vector2f);
	void AddSlider(Slider&);
	void Draw(sf::RenderWindow& window);
	sf::RectangleShape m_paramsRect;
	std::vector<Slider*> m_sliders;

	sf::Texture m_clearButtonTexture;
	sf::Sprite m_clearButton;
	sf::Texture m_playPauseButtonTexture;
	sf::Sprite m_playPauseButton;
};