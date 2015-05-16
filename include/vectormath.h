#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <SFML/Graphics.hpp>
#include <cmath>

#define PI 3.14159265
#define TAYLOR_2 2/(2)
#define TAYLOR_3 3/(3*2)
#define TAYLOR_4 4/(4*3*2)
#define TAYLOR_5 5/(5*4*3*2)
#define TAYLOR_6 6/(6*5*4*3*2)
#define TAYLOR_7 7/(7*6*5*4*3*2)

class VectorMath
{
    public:
        VectorMath();
        virtual ~VectorMath();

        static float mag(sf::Vector2f v);
        static void normalize(sf::Vector2f& v);
        static void limit(sf::Vector2f& v, float max);
        static float dist(const sf::Vector2f& v1, const sf::Vector2f& v2);
		static bool qdist(const sf::Vector2f& v1, const sf::Vector2f& v2, float d);

        static float sin(float x);
        static float cos(float x);

        static float angle(const sf::Vector2f& v);

        static sf::Vector2f addS(sf::Vector2f& v, float s);
        static void addSIP(sf::Vector2f& v, float s);
        static sf::Vector2f add(const sf::Vector2f& v1, const sf::Vector2f& v2);
        static void addIP(sf::Vector2f& v1, const sf::Vector2f& v2);

        static sf::Vector2f subS(sf::Vector2f& v, float s);
        static void subSIP(sf::Vector2f& v, float s);
        static sf::Vector2f sub(const sf::Vector2f& v1, const sf::Vector2f& v2);
        static void subIP(sf::Vector2f& v1, const sf::Vector2f& v2);

        static sf::Vector2f mulS(sf::Vector2f& v, float s);
        static void mulSIP(sf::Vector2f& v, float s);
        static sf::Vector2f mul(const sf::Vector2f& v1, const sf::Vector2f& v2);
        static void mulIP(sf::Vector2f& v1, const sf::Vector2f& v2);

        static sf::Vector2f divS(sf::Vector2f& v, float s);
        static void divSIP(sf::Vector2f& v, float s);
        static sf::Vector2f div(const sf::Vector2f& v1, const sf::Vector2f& v2);
        static void divIP(sf::Vector2f& v1, const sf::Vector2f& v2);

        static float q_rsqrt(float number);
    protected:
    private:
};

#endif // VECTORMATH_H
