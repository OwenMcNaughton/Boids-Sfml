#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include <sstream>

class Timer
{
    public:
        Timer();

        float getDelta();
        void printFps();
        std::string fpsToString();

        virtual ~Timer();
    protected:
    private:
        sf::Clock fpsClock;
        sf::Clock dtClock;

        std::string lastFps;

        int frames;
        int seconds;
};

#endif // TIMER_H
