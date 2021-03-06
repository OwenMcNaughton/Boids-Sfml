#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <thread>

#include "boid.h"
#include "assets.h"
#include "timer.h"
#include "vectormath.h"

int width = 1360;
int height = 768;

std::vector<std::vector<sf::IntRect>> rects;
sf::RenderWindow window(sf::VideoMode(width, height), "Boids", sf::Style::Fullscreen);
std::vector<Boid> boids;
std::vector<Boid> sharks;

void boidUpdate(int start, int end)
{
    Timer timer;
    while(window.isOpen()) {
        float dt = timer.getDelta();
        for(int i = start; i != end; i++) {
            boids[i].update(dt, window, boids, sharks, rects);
        }
        timer.printFps();
    }
}

int main()
{
    std::srand(std::time(0));

    sf::Color bg(25, 25, 110, 255);

    float box = width/Assets::boxCount;

    sf::Color grey(150, 150, 150, 255);
    sf::Color red(200, 20, 20, 255);

    for(int i = 0; i != Assets::boxCount; i++) {
        std::vector<sf::IntRect> smallRects;
        for(int j = 0; j != Assets::boxJ; j++) {
            sf::Color fill(0, 0, 0, 0);
            sf::Color out(grey);
            sf::IntRect r(i*box, j*box, box, box);
            smallRects.push_back(r);
        }
        rects.push_back(smallRects);
    }

    bool showGrid = false;
    std::vector<std::vector<sf::RectangleShape>> fakeRects;
    for(int i = 0; i != Assets::boxCount; i++) {
        std::vector<sf::RectangleShape> smallRects;
        for(int j = 0; j != Assets::boxJ; j++) {
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

    Timer timer;

    Assets assets;

    sf::Text text("hello", *assets.getFont("arial"));
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::White);

    for(int i = 0; i != 1200; i++) {
        float a = std::rand() % 360;
        boids.push_back(Boid(assets.getTexture("arrow"),
                                rand() % width, rand() % height,
                                a, width, height, rects));
    }

    float a = std::rand() % 360;
    Boid b(assets.getTexture("shark"),
                    sf::Mouse::getPosition(window).x + 20, sf::Mouse::getPosition(window).y + 20,
                    a, width, height, rects);
    b.sharkify();
    sharks.push_back(b);

    std::thread t1(boidUpdate, 0, 400);
    std::thread t2(boidUpdate, 400, 800);
    std::thread t3(boidUpdate, 800, 1200);

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
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Down) {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        Boid::boidSepNormal -= .1;
                        std::cout << "Separation force: " << Boid::boidSepNormal << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                        Boid::boidCohNormal -= .1;
                        std::cout << "Cohesion force: " << Boid::boidCohNormal << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        Boid::boidAliNormal -= .1;
                        std::cout << "Alignment force: " << Boid::boidAliNormal << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
                       !sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                        Boid::sepDist -= 1;
                        std::cout << "Separation dist: " << Boid::sepDist << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) &&
                       sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                        Boid::boidSharkSepFlag -= .1;
                        std::cout << "Shark sep force: " << Boid::boidSharkSepFlag << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) &&
                       sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                        Boid::sharkSepDist -= 1;
                        std::cout << "Shark sep dist: " << Boid::sharkSepDist << std::endl;
                    }
                } else if(event.key.code == sf::Keyboard::Up) {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        Boid::boidSepNormal += .1;
                        std::cout << "Separation force: " << Boid::boidSepNormal << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                        Boid::boidCohNormal += .1;
                        std::cout << "Cohesion force: " << Boid::boidCohNormal << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        Boid::boidAliNormal += .1;
                        std::cout << "Alignment force: " << Boid::boidAliNormal << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
                       !sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                        Boid::sepDist += 1;
                        std::cout << "Separation dist: " << Boid::sepDist << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) &&
                       sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                        Boid::boidSharkSepFlag += .1;
                        std::cout << "Shark sep force: " << Boid::boidSharkSepFlag << std::endl;
                    } else
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) &&
                       sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                        Boid::sharkSepDist += 1;
                        std::cout << "Shark sep dist: " << Boid::sharkSepDist << std::endl;
                    }
                } else if(event.key.code == sf::Keyboard::G) {
                    showGrid = !showGrid;
                }
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
            } break;
            case sf::Event::MouseWheelMoved:
            {
            } break;
            }
        }

        window.clear(bg);

        if(showGrid) {
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

                    window.draw(fakeRects[i][j]);
                }
            }
        }
        for(int i = 0; i != boids.size(); i++) {
            //boids[i].update(dt, window, boids, sharks, rects);
            boids[i].draw(window);
        }

        for(int i = 0; i != sharks.size(); i++) {
            sharks[i].update(dt, window, boids, sharks, rects);
            sharks[i].draw(window);
        }

        text.setString(timer.fpsToString());
        window.draw(text);

        window.display();

        timer.printFps();
    }

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
