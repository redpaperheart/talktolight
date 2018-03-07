//
//  LightModeDisco.h
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"

class LightModeDisco : public LightMode {
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
    
    // variations
    void addLight() override;
    std::shared_ptr<LightCircle> addLightRadius(float radius, float dist, float inDur=0.25, float outDur=0.5, bool filled=true);
    void addLightIdle();
    void addLightSnare();
    void addLightKick();
    void addLightHighhat();
    void removeLight() override;
    
  private:
    
    float mSensitivity;
    void visualizeAudio();
};
