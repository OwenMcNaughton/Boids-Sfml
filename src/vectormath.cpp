#include "vectormath.h"

VectorMath::VectorMath()
{
    //ctor
}

float VectorMath::mag(sf::Vector2f v)
{
    return q_rsqrt(v.x*v.x + v.y*v.y);
}

float VectorMath::q_rsqrt(float number)
{
    unsigned int i = *(unsigned int*) &number;

    i += 127 << 23;
    i >>= 1;

    return *(float*) &i;
}

void VectorMath::normalize(sf::Vector2f& v)
{
    float m = mag(v);
    if(m != 0 && m != 1) {
        v /= m;
    }
}

void VectorMath::limit(sf::Vector2f& v, float maximum)
{
    if(mag(v) > maximum) {
        normalize(v);
        v *= maximum;
    }
}

float VectorMath::dist(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return q_rsqrt(dx*dx + dy*dy);
}

float VectorMath::sin(float x)
{
    return x - pow(x, TAYLOR_3) + pow(x, TAYLOR_5);
}

float VectorMath::cos(float x)
{
    return x - pow(x, TAYLOR_2) + pow(x, TAYLOR_4);
}

float VectorMath::angle(const sf::Vector2f& v) {
    float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
    return angle;
}

sf::Vector2f VectorMath::addS(sf::Vector2f& v, float s) {
    sf::Vector2f r(v.x + s, v.y + s);
    return r;
}

void VectorMath::addSIP(sf::Vector2f& v, float s) {
    v.x += s;
    v.y += s;
}

sf::Vector2f VectorMath::add(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    sf::Vector2f r(v1.x + v2.x, v1.y + v2.y);
    return r;
}

void VectorMath::addIP(sf::Vector2f& v1, const sf::Vector2f& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
}

sf::Vector2f VectorMath::subS(sf::Vector2f& v, float s) {
    sf::Vector2f r(v.x - s, v.y - s);
    return r;
}

void VectorMath::subSIP(sf::Vector2f& v, float s) {
    v.x -= s;
    v.y -= s;
}

sf::Vector2f VectorMath::sub(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    sf::Vector2f r(v1.x - v2.x, v1.y - v2.y);
    return r;
}

void VectorMath::subIP(sf::Vector2f& v1, const sf::Vector2f& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
}

sf::Vector2f VectorMath::mulS(sf::Vector2f& v, float s) {
    sf::Vector2f r(v.x * s, v.y * s);
    return r;
}

void VectorMath::mulSIP(sf::Vector2f& v, float s) {
    v.x *= s;
    v.y *= s;
}

sf::Vector2f VectorMath::mul(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    sf::Vector2f r(v1.x * v2.x, v1.y * v2.y);
    return r;
}

void VectorMath::mulIP(sf::Vector2f& v1, const sf::Vector2f& v2) {
    v1.x *= v2.x;
    v1.y *= v2.y;
}

sf::Vector2f VectorMath::divS(sf::Vector2f& v, float s) {
    sf::Vector2f r(v.x / s, v.y / s);
    return r;
}

void VectorMath::divSIP(sf::Vector2f& v, float s) {
    v.x /= s;
    v.y /= s;
}

sf::Vector2f VectorMath::div(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    sf::Vector2f r(v1.x / v2.x, v1.y / v2.y);
    return r;
}

void VectorMath::divIP(sf::Vector2f& v1, const sf::Vector2f& v2) {
    v1.x /= v2.x;
    v1.y /= v2.y;
}

VectorMath::~VectorMath()
{
    //dtor
}
