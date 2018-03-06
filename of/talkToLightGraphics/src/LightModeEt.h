//
//  LightModeEt.hp
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"
#include "ETRing.h"

class LightModeEt : public LightMode
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
    
    
protected:
    
    int mQuestionCount = 0;
    bool mIsSoundReactive = false;
    
    int ringSwapDuration=4000;
    
    float startRadius = 220;
    float currentRotation = 0;
    int ringCount = 6;
    int ringSegments = 20;
    
    int mActiveRingId = 0;
    int mLastRingSwap = 0; //last time the rings swapped, used in idle
    
    void visualizeAudio(); //maybe pull out into mode base class?
    void drawDotRing();
    
    std::vector<std::shared_ptr<ETRing>> mRings;
};
