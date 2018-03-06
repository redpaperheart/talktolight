//
//  Particle.cpp
//  splashCircles
//
//  Created by Andrew Cotter on 6/19/17.
//
//

#include "Particle.hpp"


ParticleRef Particle::create()
{
    ParticleRef ref = std::shared_ptr<Particle>( new Particle());
    return ref;
}

Particle::Particle()
:theta(ofRandom(360))
,distance(ofRandom(100))
,age(0)
,filled(false)
{
    
}

void Particle::update()
{
    this->age++;
}

void Particle::draw()
{
    ofPushMatrix();
    ofRotate(theta);
    
    if (this->filled)
    {
        ofFill();
    }else
    {
        ofNoFill();
    }
    
    ofDrawCircle( 0, distance, this->age );
    
    ofPopMatrix();
}
