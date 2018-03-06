//
//  ParticleSystem.hpp
//  splashCircles
//
//  Created by Andrew Cotter on 6/19/17.
//
//


#pragma once

#include "ofMain.h"
#include "Particle.hpp"

class ParticleSystem;

typedef std::shared_ptr<ParticleSystem> ParticleSystemRef;

class ParticleSystem
{
public:
    static ParticleSystemRef create();
    ParticleSystem();
    void setup();
    void update();
    void draw();
    void addParticle();
    
private:
    ofVec2f mPos;
    int mMaxParticles;
    int mMaxAge;
    
    vector<ParticleRef> mParticles;
};
