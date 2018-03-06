//
//  LightModeSpotlight
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"

class LightModeSpotlight : public LightMode
{
public:
    void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
    void update() override;
    void draw() override;
    void reloadShaders() override;
    
    //states
//    void onDelayComplete(float *arg) override;
    void animateIn() override; // init, set circles sizes etc.
    void animateOut(float duration=1) override; // clean up, reset, e.g. remove additional circles, when switching modes.
    
    void setQuestionState() override;
    void setResponseState() override;
    void setNormalState() override;
    
    void randomizePosition();
    LightCircle spotLight;
protected:
    
    float mBaseRadius = 200; //starting radius for the circle
    float mCenterBodyRadius = 300; //radius of a body/shadow under the projector
    int mIdleCounter = 0; //last time the idle swapped
    int mIdleDuration = 4000;
    ofVec2f getNonCenter();
    void visualizeAudio(); //maybe pull out into mode base class?

};
