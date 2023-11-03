#include "Hud.h"
#include"Verlet.h"
Hud::Hud()
{
    m_clearButtonTexture.loadFromFile("./Sprites/Clear_Button.png");
    m_clearButton.setTexture(m_clearButtonTexture);
    m_clearButton.setScale({ 3.f, 3.f });
    m_clearButton.setPosition({ 1030.f, 800.f });

    m_paramsRect = sf::RectangleShape(sf::Vector2f(300, 1000));
    m_paramsRect.setPosition(1000, 0);
    m_paramsRect.setFillColor(sf::Color(255, 200, 50, 100));
    m_font.loadFromFile("./Fonts/OpenSans-Regular.ttf");
    m_textInfo.setFont(m_font);
    m_textInfo.setString("Use Right click to spawn circles\nUse Left click to create ropes.\nPlay with the settings !");
    m_textInfo.setPosition(1015, 50);
    m_textInfo.setCharacterSize(18);
}

void Hud::ClickEvent(sf::RenderWindow& window, sf::Vector2f mousePos)
{
    if (m_clearButton.getGlobalBounds().contains(mousePos))
    {
        Verlet::GetInstance()->Clear();
    }
    sf::Event event;
    for (auto& slider : m_sliders)
    {
        if (slider->m_circle.getGlobalBounds().contains(mousePos))
        {

            bool dragging = true;
            while (dragging) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::MouseButtonReleased &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        dragging = false;
                        break;
                    }
                    else if (event.type == sf::Event::MouseMoved)
                    {
                        slider->Move(static_cast<float>(event.mouseMove.x));
                    }

                    window.clear();

                    Verlet::GetInstance()->Draw(window);
                    Draw(window);       
                    window.display();
                }
            }
        }
    }
}
void Hud::AddSlider(Slider& slider)
{
    m_sliders.push_back(&slider);
}

void Hud::Draw(sf::RenderWindow& window)
{
    window.draw(m_paramsRect);
    window.draw(m_clearButton);
    for (auto& slider : m_sliders)
    {
        slider->Draw(window);
    }
    window.draw(m_textInfo);
}

