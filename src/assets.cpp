#include "assets.h"

Assets::Assets()
{
    addTexture("arrow", "res\\arrow.png");
    addTexture("circle", "res\\circle.png");
    addTexture("trail", "res\\trail.png");
    addTexture("shark", "res\\shark.png");

    addFont("arial", "res\\arial.ttf");
}

void Assets::addTexture(std::string key, std::string path)
{
    sf::Texture tex;
    tex.loadFromFile(path);
    textures[key] = tex;
}

void Assets::addFont(std::string key, std::string path)
{
    sf::Font font;
    font.loadFromFile("res\\arial.ttf");
    fonts[key] = font;
}

sf::Texture * Assets::getTexture(std::string key)
{
    return &textures[key];
}

sf::Font * Assets::getFont(std::string key)
{
    return &fonts[key];
}


Assets::~Assets()
{
    //dtor
}
