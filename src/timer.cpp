#include "timer.h"

Timer::Timer()
    : frames(0), seconds(0)
{
    lastFps = "                  ";
}

float Timer::getDelta()
{
    sf::Time elapsed = dtClock.restart();
    return elapsed.asMicroseconds();
}

void Timer::printFps()
{
    if(fpsClock.getElapsedTime().asMicroseconds() >= 1000000)
    {
        seconds++;
        std::cout << "FPS: " << frames << std::endl;
        frames = 0;
        fpsClock.restart();
    }
    else
    {
        frames++;
    }
}

std::string Timer::fpsToString()
{
    std::stringstream ss;
    if(fpsClock.getElapsedTime().asMicroseconds() >= 1000000)
    {
        seconds++;
        ss << frames;
        frames = 0;
        fpsClock.restart();
        lastFps = ss.str();
        return lastFps;
    }
    else
    {
        frames++;
        return lastFps;
    }
}



Timer::~Timer()
{
    //dtor
}
