#include <cstdlib>
#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "boid.h"
#include "assets.h"
#include "timer.h"
#include "vectormath.h"
#include "obstacle.h"

int main()
{
    std::srand(std::time(0));

    int width = 512 << 0;
    int height = 320 << 0;
    float box = width/8;

    sf::Color grey(150, 150, 150, 255);
    sf::Color red(200, 20, 20, 255);

    std::vector<Obstacle> obstacles;

    std::vector<std::vector<sf::IntRect>> rects;
    for(int i = 0; i != 8; i++) {
        std::vector<sf::IntRect> smallRects;
        for(int j = 0; j != 5; j++) {
            sf::Color fill(0, 0, 0, 0);
            sf::Color out(grey);
            sf::IntRect r(i*box, j*box, box, box);
            smallRects.push_back(r);
        }
        rects.push_back(smallRects);
    }

    std::vector<std::vector<sf::RectangleShape>> fakeRects;
    for(int i = 0; i != 8; i++) {
        std::vector<sf::RectangleShape> smallRects;
        for(int j = 0; j != 5; j++) {
            sf::Vector2f s(box, box);
            sf::Vector2f p(i*box, j*box);
            sf::Color fill(0, 0, 0, 0);
            sf::Color out(150, 150, 150, 255);
            sf::RectangleShape r(s);
            r.setPosition(p);
            r.setFillColor(fill);
            r.setOutlineColor(out);
            r.setOutlineThickness(1);
            smallRects.push_back(r);
        }
        fakeRects.push_back(smallRects);
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "Boids");

    Timer timer;

    Assets assets;

    sf::Text text("hello", *assets.getFont("arial"));
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::White);

    std::vector<Boid> boids;
    for(int i = 0; i != 120; i++) {
        float a = std::rand() % 360;
        boids.push_back(Boid(assets.getTexture("arrow"),
                                sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,
                                a, width, height, rects));
    }

    std::vector<Boid> sharks;
    float a = std::rand() % 360;
    Boid b(assets.getTexture("shark"),
                    sf::Mouse::getPosition(window).x + 20, sf::Mouse::getPosition(window).y + 20,
                    a, width, height, rects);
    b.sharkify();
    sharks.push_back(b);

    while (window.isOpen())
    {
        float dt = timer.getDelta();

        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type) {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    float a = std::rand() % 360;
                    Boid b(assets.getTexture("shark"),
                                    sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,
                                    a, width, height, rects);
                    b.sharkify();
                    sharks.push_back(b);
                    break;
                }
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    Obstacle o(20, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                    obstacles.push_back(o);
                }
            }
            case sf::Event::MouseWheelMoved:
                for(int i = 0; i != obstacles.size(); i++) {
                    if(obstacles[i].contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                        obstacles[i].resize(event.mouseWheel.delta);
                    }
                }
            }
        }

        window.clear();


        for(int i = 0; i != fakeRects.size(); i++) {
            for(int j = 0; j != fakeRects[0].size(); j++) {
                bool empty = true;
                for(auto b : boids) {
                    if(b.rectContainsBoid(i, j)) {
                        empty = false;
                        break;
                    }
                }
                if(!empty) {
                    fakeRects[i][j].setOutlineColor(red);
                    fakeRects[i][j].setOutlineThickness(3);
                } else {
                    fakeRects[i][j].setOutlineColor(grey);
                    fakeRects[i][j].setOutlineThickness(1);
                }

                //window.draw(fakeRects[i][j]);
            }
        }

        for(int i = 0; i != obstacles.size(); i++) {
            obstacles[i].draw(window);
        }

        for(int i = 0; i != boids.size(); i++) {
            boids[i].update(dt, window, boids, sharks, obstacles, rects);
            boids[i].draw(window);
        }

        for(int i = 0; i != sharks.size(); i++) {
            sharks[i].update(dt, window, boids, sharks, obstacles, rects);
            sharks[i].draw(window);
        }



        text.setString(timer.fpsToString());
        window.draw(text);

        window.display();

        timer.printFps();
    }

    return 0;
}
