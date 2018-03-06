//
//  ParticleSystem.cpp
//  splashCircles
//
//  Created by Andrew Cotter on 6/19/17.
//
//

#include "ParticleSystem.hpp"

ParticleSystemRef ParticleSystem::create()
{
    ParticleSystemRef ref = std::shared_ptr<ParticleSystem>(new ParticleSystem);
    return ref;
}

ParticleSystem::ParticleSystem()
:mPos(ofVec2f(ofGetWidth()/2, ofGetHeight()/2))
,mMaxParticles(60)
,mMaxAge(200)
{
    
}

void ParticleSystem::setup()
{
    
}

void ParticleSystem::update()
{
    this->addParticle();
    
    for( auto& p : mParticles )
    {
        p->update();
    }
    
    for (auto p = mParticles.begin(); p != mParticles.end();)
    {
        if ((*p)->getAge() >= mMaxAge)
        {
            p = mParticles.erase(p);
        } else
        {
            ++p;
        }
    }
}

void ParticleSystem::draw()
{
    for ( auto& p : particles)
    {
        p->draw();
    }
}

void ParticleSystem::addParticle()
{
    mParticles.push_back(Particle::create());
}
