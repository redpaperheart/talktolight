//
//  LightModeGoodNight
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"
#include "GlowShapes.h"

class LightModeGoodNight : public LightMode
{
public:
    void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
    void update() override;
    void draw() override;
    void reloadShaders() override;
    
    //states
    void animateIn() override; // init, set circles sizes etc.
    void animateOut(float duration=1) override; // clean up, reset, e.g. remove additional circles, when switching modes.
    
    void setQuestionState() override;
    void setResponseState() override;
    void setNormalState() override;
    
//    void randomizePosition();
    LightCircle spotLight;
    LightCircle moon;
    LightCircle shadow;
    struct Star{
        ofVec2f pos;
        float glow;
        float sinOffset;
        float sinPhase;
    };
    float mStarAlpha = 0;
protected:
    
    float mBaseRadius = 200; //starting radius for the circle
    float mCenterBodyRadius = 300; //radius of a body/shadow under the projector
    int mIdleCounter = 0; //last time the idle swapped
    int mIdleDuration = 4000;
    void visualizeAudio(); //maybe pull out into mode base class?
    std::vector<Star> stars;
};
