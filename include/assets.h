#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>

#include <tr1/unordered_map>
#include <string>

class Assets
{
    public:
        Assets();

        void addTexture(std::string key, std::string path);
        sf::Texture * getTexture(std::string key);

        void addFont(std::string key, std::string path);
        sf::Font * getFont(std::string key);

        static int boxCount;
        static int boxJ;

        virtual ~Assets();
    protected:
    private:
        std::tr1::unordered_map<std::string, sf::Texture> textures;
        std::tr1::unordered_map<std::string, sf::Font> fonts;
};

#endif // ASSETS_H
