//
//  LightMode.h
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"

class LightModeCat : public LightMode {
  public:
    
    void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
    void update() override;
//    void draw() override;
    
    //states
    void animateIn() override; // init, set circles sizes etc.
    void animateOut() override; // clean up, reset, e.g. remove additional circles, when switching modes.
    
    void setQuestionState() override;
    void setResponseState() override;
    void setNormalState() override;
    
    // audio
//    void setAudioLevel(float level) override; // [0-1]
    
    // variations
    void addLight() override;
    void removeLight() override;
    
//    void addColors() override;
//    void removeColors() override;
    
//    void bigger() override;
//    void smaller() override;
    
  private:
    
    

};
