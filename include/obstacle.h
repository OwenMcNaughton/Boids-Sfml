#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "vectormath.h"

class Obstacle
{
    public:
        Obstacle(float r, float x, float y);

        void draw(sf::RenderTarget& window);
        void resize(float d);

        bool contains(float x, float y);

        virtual ~Obstacle();

        float radius;
        sf::Vector2f pos;
        sf::CircleShape circle;

    protected:
    private:

};

#endif // OBSTACLE_H
