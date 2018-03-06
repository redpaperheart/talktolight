//
//  LightModeLaser
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightCircleTrail.h"
#include "LightMode.h"

class LightModeLaser : public LightMode {
  public:
    
    void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
    void update() override;
    void draw() override;
    
    //states
    void animateIn() override; // init, set circles sizes etc.
    void animateOut(float duration=1) override; // clean up, reset, e.g. remove additional circles, when switching modes.
    
    void setQuestionState() override;
    void setResponseState() override;
    void setNormalState() override;
    
    
    std::shared_ptr<LightCircleTrail> mLaserPointer;
    ofVec2f getRandomPointInCenter();
  private:
    void newLaserTarget(ofVec2f target, float duration = 3, bool force=false);
};
