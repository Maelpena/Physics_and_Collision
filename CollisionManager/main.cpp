#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "Verlet.h"
#include <vector>
#include <iostream>
#include "Link.h"
#include "Slider.h"
#include "Hud.h"
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional> 

using namespace std;

void Draw(sf::RenderWindow& window, Hud* hud)
{

    window.clear();

    Verlet::GetInstance()->Draw(window);
    hud->Draw(window);

    window.display();

}

std::mutex myMutex;
std::condition_variable condition;
std::queue<std::function<void()>> tasks;
bool stop = false;

//
int main()
{









    sf::ContextSettings settings(0, 0, 1);

    sf::RenderWindow window(sf::VideoMode(1300, 1000), "SFML works!", sf::Style::Default, settings);
    sf::Clock Clock;

    float timer = 0.f;
    float timer2 = 0.f;
    float deltaTime;

    float fpsTarget = 60;
    float fixedUpdateTimer = 1 / fpsTarget;
    window.setFramerateLimit(fpsTarget);

    vector<sf::Vector2f> spawnerPos;
    spawnerPos.push_back({ 500.f, 200.f });
    spawnerPos.push_back({ 400.f, 225.f });
    spawnerPos.push_back({ 600.f, 225.f });
    spawnerPos.push_back({ 300.f, 250.f });
    spawnerPos.push_back({ 700.f, 250.f });
    spawnerPos.push_back({ 250.f, 275.f });
    spawnerPos.push_back({ 750.f, 275.f });
    spawnerPos.push_back({ 200.f, 300.f });
    spawnerPos.push_back({ 800.f, 300.f });
    spawnerPos.push_back({ 150.f, 325.f });
    spawnerPos.push_back({ 850.f, 325.f });
    float spawnerCount = spawnerPos.size();

    float circleSpawnRate = 50.f;
    sf::Vector2f spawnPos(500.f, 200.f);
    sf::Vector2f spawnPos2(400.f, 250.f);
    sf::Vector2f spawnPos3(600.f, 250.f);

    vector<Slider> sliders;

    Slider subStepSlider("SubStep", &(Verlet::GetInstance()->m_subStep), sf::Vector2f(1030, 200), 1, 20, 200.f);
    Slider circleSpawnRateSlider( "Circle spawn rate", &circleSpawnRate, sf::Vector2f(1030, 300), 0, 50, 200.f);
    Slider spawnerSlider("Circle Spawners", &spawnerCount, sf::Vector2f(1030, 400), 1, spawnerPos.size(), 200.f);
    Slider radiusSlider("Circle Radius", &Circle::globalRadius, sf::Vector2f(1030, 500), 10, 30, 200.f);

    
    Hud* hud = new Hud;
    hud->AddSlider(circleSpawnRateSlider);
    hud->AddSlider(subStepSlider);
    hud->AddSlider(spawnerSlider);
    hud->AddSlider(radiusSlider);


    sf::Texture clearButtonTexture;
    clearButtonTexture.loadFromFile("./Sprites/Clear_Button.png");
    sf::Sprite clearButton;
    clearButton.setTexture(clearButtonTexture);
    clearButton.setScale({ 3.f, 3.f });
    clearButton.setPosition({1050.f, 100.f});


    sliders.push_back(subStepSlider);
    sliders.push_back(circleSpawnRateSlider);

    while (window.isOpen())
    {

        sf::Event event;
        sf::Time elapsed = Clock.getElapsedTime();
        deltaTime = elapsed.asSeconds();
        Clock.restart();

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) 
                {
                    hud->ClickEvent(window, mousePos);

                    float mouseDistToConstraints = Verlet::VectorDistance(Verlet::GetInstance()->m_constraintShape.getPosition(), mousePos);
                    if (mouseDistToConstraints < (Verlet::GetInstance()->m_constraintShape.getRadius() - Circle::globalRadius))
                    {
                        Verlet::GetInstance()->m_circles.push_back(new Circle(true, mousePos, true, {0,0}));
                        bool dragging = true;
                        while (dragging) {
                            while (window.pollEvent(event)) {
                                if (event.type == sf::Event::MouseButtonReleased &&
                                    event.mouseButton.button == sf::Mouse::Left)
                                {
                                    dragging = false;
                                    Verlet::GetInstance()->m_circles.back()->m_isFixed = true;


                                    break;

                                }
                                while (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && !(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right))
                                {
                                    mousePos = sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
                                    unsigned int vecSize = Verlet::GetInstance()->m_circles.size();
                                    float dist = Verlet::VectorDistance(Verlet::GetInstance()->m_circles[vecSize - 1]->m_currentPos, mousePos);

                                    if (dist < (Circle::nextRadius + Verlet::GetInstance()->m_circles[vecSize - 1]->m_radius))
                                        break;

                                    sf::Vector2f dir = Verlet::GetInstance()->m_circles[vecSize - 1]->m_currentPos - mousePos;
                                    sf::Vector2f normalizedDist = dir / dist;


                                    sf::Vector2f circlePosition = Verlet::GetInstance()->m_circles[vecSize - 1]->m_currentPos - (Circle::nextRadius + Verlet::GetInstance()->m_circles[vecSize - 1]->m_radius) * normalizedDist;
                                    float circleDistToConstraints = Verlet::VectorDistance(Verlet::GetInstance()->m_constraintShape.getPosition(), circlePosition);

                                    if (circleDistToConstraints > (Verlet::GetInstance()->m_constraintShape.getRadius() - Circle::nextRadius))
                                        break;
                                    Circle* c = new Circle(false, circlePosition, true, {0,0});
                                    Verlet::GetInstance()->m_links.push_back(new Link(Verlet::GetInstance()->m_circles[vecSize - 1], c));
                                    Verlet::GetInstance()->m_circles.push_back(c);
                                }

                                Draw(window, hud);
                            }
                        }
                    }

                }
            }
        }


        Verlet::GetInstance()->Update(fixedUpdateTimer);

        timer += deltaTime;
        timer2 += deltaTime;

        if (timer > (1/ circleSpawnRate) && sf::Mouse::isButtonPressed(sf::Mouse::Right) && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (size_t i = 0; i < spawnerCount; i++)
            {
                sf::Vector2f direction = (mousePos - spawnerPos[i]) / Verlet::VectorDistance(mousePos, spawnerPos[i]);
                Verlet::GetInstance()->m_circles.push_back(new Circle(false, spawnerPos[i], false, direction));
            }
            timer = 0.f;
        }
        if (timer2 > 1.f)
        {
            std::cout << "Count = " << Verlet::GetInstance()->m_circles.size() << std::endl;
            timer2 = 0.f;
        }

        Draw(window, hud);
    }

    return 0;
}



