#include "obstacle.h"

Obstacle::Obstacle(float r, float x, float y)
    : radius(r), pos(x, y)
{
    circle = sf::CircleShape(r, 30);
    circle.setPosition(pos.x-r, pos.y-r);
    sf::Color f(150, 150, 255, 255);
    circle.setFillColor(f);
    sf::Color o(50, 50, 200, 255);
    circle.setOutlineColor(o);
    circle.setOutlineThickness(3);
}

void Obstacle::draw(sf::RenderTarget& window)
{
    window.draw(circle);
}

void Obstacle::resize(float d)
{
    if(d > 0) {
        circle.setScale(circle.getScale().x*1.1, circle.getScale().y*1.1);
    } else {
        circle.setScale(circle.getScale().x*.9, circle.getScale().y*.9);
    }
}

bool Obstacle::contains(float x, float y)
{
    sf::Vector2f v2(x, y);
    if(VectorMath::dist(v2, pos) < radius) {
        return true;
    } else {
        return false;
    }
}

Obstacle::~Obstacle()
{
    //dtor
}
