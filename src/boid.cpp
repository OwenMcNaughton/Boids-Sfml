#include "boid.h"


float Boid::boidSepNormal = 2.5;
float Boid::boidSharkSepNormal = 0.0;
float Boid::boidCohNormal = 1.0;
float Boid::boidAliNormal = 2.0;

float Boid::boidSepFlag = 2.0;
float Boid::boidSharkSepFlag = 1.5;
float Boid::boidCohFlag = 0.1;
float Boid::boidAliFlag = 0.1;

float Boid::sepDist = 15.0;
float Boid::sharkSepDist = 45.0;

Boid::Boid()
{}

Boid::Boid(sf::Texture * argtex, float x, float y, float a, int w, int h, const std::vector<std::vector<sf::IntRect>>& rects)
    : pos(x, y), trailOn(false), trailIdx(0), alignDist(30.0), cohDist(50.0),
        maxSpeed(2.0), xSpeed(maxSpeed*15), maxForce(0.08), xForce(maxForce*2),
        worldWidth(w), worldHeight(h), vel(cos(a), sin(a))
{
    spriteTex = argtex;
    sprite.setTexture(*spriteTex);
    size = spriteTex->getSize().x/2;
    sprite.setOrigin(size, size);
    sprite.setPosition(pos);
    sprite.scale(.4, .4);

    for(int i = 0; i != rects.size(); i++)
    {
        for(int j = 0; j != rects[0].size(); j++)
        {
            if(rects[i][j].contains(pos.x, pos.y)) {
                rectPos.x = i; rectPos.y = j;
            }
        }
    }

    shark = false;
}

void Boid::sharkify()
{
    shark = true;
    sprite.scale(2, 2);
    cohDist = 200.0;
    maxForce = 0.01;
    maxSpeed = 2.7;
}

void Boid::draw(sf::RenderTarget& window)
{
    if(trailOn)
    {
        for(unsigned i = 0; i != trail.size(); i++)
        {
            window.draw(trail[i]);
        }
    }

    window.draw(sprite);
}

bool Boid::sameRect(const Boid& b)
{
    return true;
    if(rectPos.x == b.rectPos.x && rectPos.y == b.rectPos.y) {
        return true;
    } else {
        return false;
    }
}

bool Boid::rectContainsBoid(int i, int j)
{
    if(rectPos.x == i && rectPos.y == j) {
        return true;
    } else {
        return false;
    }
}

sf::Vector2i Boid::quickRectPos(const std::vector<std::vector<sf::IntRect>>& rects)
{
    if(!(rectPos.x > rects.size() || rectPos.y > rects[0].size())) {
        if(rects[rectPos.x][rectPos.y].contains(pos.x, pos.y)) {
            return rectPos;
        }
    }

    for(int i = 0; i != rects.size(); i++)
    {
        for(int j = 0; j != rects[0].size(); j++)
        {
            if(rects[i][j].contains(pos.x, pos.y)) {
                rectPos.x = i; rectPos.y = j;
                return rectPos;
            }
        }
    }
}

void Boid::update(float dt, sf::RenderWindow& window, const std::vector<Boid>& boids, const std::vector<Boid>& sharks,
                  const std::vector<std::vector<sf::IntRect>>& rects)
{
    sharkFlag = false;

    //rectPos = quickRectPos(rects);

    bundledForce(boids);

    sf::Vector2f sep = tempSep;
    sf::Vector2f sharkSep = sharkSeparation(sharks);
    sf::Vector2f coh = tempCoh;
    sf::Vector2f ali;
    if(!shark)
        ali = tempAli;

    if(shark) {
        VectorMath::mulSIP(sep, 0);
        VectorMath::mulSIP(coh, 3.0);
        VectorMath::mulSIP(ali, 1);
    } else {
        if(sharkFlag) {
            VectorMath::mulSIP(sep, boidSepFlag);
            VectorMath::mulSIP(sharkSep, boidSharkSepFlag);
            VectorMath::mulSIP(coh, boidCohFlag);
            VectorMath::mulSIP(ali, boidAliFlag);
        } else {
            VectorMath::mulSIP(sep, boidSepNormal);
            VectorMath::mulSIP(sharkSep, boidSharkSepNormal);
            VectorMath::mulSIP(coh, boidCohNormal);
            VectorMath::mulSIP(ali, boidAliNormal);
        }
    }

    VectorMath::addIP(acc, sep);
    VectorMath::addIP(acc, sharkSep);
    VectorMath::addIP(acc, coh);
    VectorMath::addIP(acc, ali);

    sf::Vector2f dtacc = VectorMath::mulS(acc, dt);
    VectorMath::divSIP(dtacc, 15000.0);

    VectorMath::addIP(vel, dtacc);
    if(sharkFlag) {
        VectorMath::limit(vel, xSpeed);
    } else {
        VectorMath::limit(vel, maxSpeed);
    }
    sf::Vector2f dtvel = VectorMath::mulS(vel, dt);
    VectorMath::divSIP(dtvel, 15000.0);
    VectorMath::addIP(pos, dtvel);
    VectorMath::mulSIP(acc, (float)0.0);

    float angle = VectorMath::angle(vel);

    sprite.setRotation(angle);
    sprite.setPosition(pos);

    if(trailOn)
    {
        trailUpdate(dt);
    }

    borders();
}

void Boid::borders() {
    if (pos.x < 0) pos.x = worldWidth;
    if (pos.y < 0) pos.y = worldHeight;
    if (pos.x > worldWidth) pos.x = 0;
    if (pos.y > worldHeight) pos.y = 0;
}

void Boid::bundledForce(const std::vector<Boid>& boids)
{
    tempSep.x = 0; tempSep.y = 0;
    tempAli.x = 0; tempAli.y = 0;
    tempCoh.x = 0; tempCoh.y = 0;

    int scount = 0;
    int acount = 0;
    int ccount = 0;

    for(int i = 0; i != boids.size(); i++) {
        float dist = VectorMath::dist(pos, boids[i].pos);
        if(dist > 0.001) {
           if(dist < Boid::sepDist) {
                sf::Vector2f diff = VectorMath::sub(pos, boids[i].pos);
                VectorMath::normalize(diff);
                VectorMath::divSIP(diff, dist);
                VectorMath::addIP(tempSep, diff);
                scount++;
            }
            if(dist * 3 < Boid::cohDist) {
                VectorMath::addIP(tempCoh, boids[i].pos);
                ccount++;
            }
            if(dist * 2 < Boid::alignDist) {
                VectorMath::addIP(tempAli, boids[i].vel);
                acount++;
            }
        }
    }

    if(scount > 0) {
        VectorMath::divSIP(tempSep, (float)scount);
        if(VectorMath::mag(tempSep) > 0) {
            VectorMath::normalize(tempSep);
            VectorMath::mulSIP(tempSep, maxSpeed);
            VectorMath::subIP(tempSep, vel);
            if(sharkFlag) {
                VectorMath::limit(tempSep, xForce);
            } else {
                VectorMath::limit(tempSep, maxForce);
            }
        }
    }

    if(ccount > 0) {
        VectorMath::divSIP(tempCoh, (float)ccount);
        tempCoh = seek(tempCoh);
    }

    if(acount > 0) {
        tempAli /= (float)acount;
        VectorMath::normalize(tempAli);
        VectorMath::mulSIP(tempAli, maxSpeed);
        sf::Vector2f tempAli = VectorMath::sub(tempAli, vel);
        VectorMath::limit(tempAli, maxForce);
    }
}

sf::Vector2f Boid::separation(const std::vector<Boid>& boids)
{
    sf::Vector2f v(0, 0);
    int count = 0;
    for(int i = 0; i != boids.size(); i++) {
        //if(sameRect(b)) {
            float dist = VectorMath::dist(pos, boids[i].pos);
            if((dist > 0.001 && dist < Boid::sepDist)) {
                sf::Vector2f diff = VectorMath::sub(pos, boids[i].pos);
                VectorMath::normalize(diff);
                VectorMath::divSIP(diff, dist);
                VectorMath::addIP(v, diff);
                count++;
            }
        //}
    }

    if(count > 0) {
        VectorMath::divSIP(v, (float)count);
    }

    count = 0;

    if(v.x == 0 && v.y == 0) return v;

    if(VectorMath::mag(v) > 0) {
        VectorMath::normalize(v);
        VectorMath::mulSIP(v, maxSpeed);
        VectorMath::subIP(v, vel);
        if(sharkFlag) {
            VectorMath::limit(v, xForce);
        } else {
            VectorMath::limit(v, maxForce);
        }
    }
    return v;
}

sf::Vector2f Boid::sharkSeparation(const std::vector<Boid>& boids)
{
    sf::Vector2f v(0, 0);
    int count = 0;
    for(int i = 0; i != boids.size(); i++) {
        float dist = VectorMath::dist(pos, boids[i].pos);
        if((dist > 0.001 && dist < Boid::sharkSepDist)) {
            sf::Vector2f diff = VectorMath::sub(pos, boids[i].pos);
            VectorMath::normalize(diff);
            VectorMath::mulSIP(diff, 1);
            sharkFlag = true;
            VectorMath::divSIP(diff, dist);
            VectorMath::addIP(v, diff);
            count++;
        }
    }

    if(count > 0) {
        VectorMath::divSIP(v, (float)count);
    }

    if(v.x == 0 && v.y == 0) return v;

    if(VectorMath::mag(v) > 0) {
        VectorMath::normalize(v);
        if(sharkFlag) {
            VectorMath::mulSIP(v, xSpeed);
        } else {
            VectorMath::mulSIP(v, maxSpeed);
        }
        VectorMath::subIP(v, vel);
        if(sharkFlag) {
            VectorMath::limit(v, xForce);
        } else {
            VectorMath::limit(v, maxForce);
        }
    }
    return v;
}

sf::Vector2f Boid::cohesion(const std::vector<Boid>& boids)
{
    sf::Vector2f sum(0, 0);
    int count = 0;
    for(int i = 0; i != boids.size(); i++) {
        //if((sameRect(b) && !b.shark) || (shark && !b.shark)) {
            //float dist = VectorMath::dist(pos, boids[i].pos);
			if(VectorMath::qdist(pos, boids[i].pos, cohDist)) {
            //if(dist > 0.001 && dist < cohDist) {
                VectorMath::addIP(sum, boids[i].pos);
                count++;
            }
        //}
    }

    if(count > 0) {
        VectorMath::divSIP(sum, (float)count);
        return seek(sum);
    } else {
        return sf::Vector2f(0, 0);
    }
}

sf::Vector2f Boid::alignment(const std::vector<Boid>& boids)
{
    sf::Vector2f sum(0, 0);
    int count = 0;
    for(int i = 0; i != boids.size(); i++) {
        //if(sameRect(b) || (shark && !b.shark)) {
            //float dist = VectorMath::dist(pos, boids[i].pos);
			if(VectorMath::qdist(pos, boids[i].pos, alignDist)) {
            //if(dist > 0.001 && dist < alignDist) {
                VectorMath::addIP(sum, boids[i].vel);
                count++;
            }
        //}
    }

    if(count > 0) {
        sum /= (float)count;

        VectorMath::normalize(sum);
        VectorMath::mulSIP(sum, maxSpeed);
        sf::Vector2f v = VectorMath::sub(sum, vel);
        VectorMath::limit(v, maxForce);
        return v;
    } else {
        return sf::Vector2f(0, 0);
    }
}

sf::Vector2f Boid::seek(sf::Vector2f target)
{
    sf::Vector2f desired = VectorMath::sub(target, pos);
    VectorMath::normalize(desired);
    VectorMath::mulSIP(desired, maxSpeed);

    sf::Vector2f v = VectorMath::sub(desired, vel);
    VectorMath::limit(v, maxForce);
    return v;
}

void Boid::trailUpdate(float dt)
{
    sf::Sprite s;
    s.setTexture(*trailTex);
    s.setPosition(pos);
    if(trail.size() == 1000)
    {
        trail[trailIdx++] = s;
        if(trailIdx == 1000) trailIdx = 0;
    }
    else
    {
        trail.push_back(s);
    }
}

void Boid::setTrail(sf::Texture * argtex)
{
    trailTex = argtex;
    trailOn = true;
}

std::string Boid::toString()
{
    std::string s;
    std::stringstream ss;
    ss << "pos: " << (int)pos.x << "," << (int)pos.y;
    ss << "\nvel: ";
    if(vel.x > 0) ss << "+x,"; else if(vel.x < 0) ss << "-x,"; else ss << "0,";
    if(vel.y > 0) ss << "+y,"; else if(vel.y < 0) ss << "-y,"; else ss << "0,";
    return ss.str();
}

Boid::~Boid()
{
    //dtor
}
