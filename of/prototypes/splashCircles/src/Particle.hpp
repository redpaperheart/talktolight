//
//  Particle.hpp
//  splashCircles
//
//  Created by Andrew Cotter on 6/19/17.
//
//

#pragma once

#include "ofMain.h"

class Particle;

typedef std::shared_ptr<Particle> ParticleRef;

class Particle
{
public:
    static ParticleRef create ();
    Particle();
    ~Particle();
    void update();
    void draw();
    int getAge(){ return this->age; };
    bool isFilled(){ return this->filled; };
private:
    float theta;
    float distance;
    float age;
    bool  filled;
    ofColor mColor;
};
