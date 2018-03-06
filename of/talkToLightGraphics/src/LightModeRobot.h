//
//  LightModeRobot.hp
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"
#include "RobotEye.h"

class LightModeRobot : public LightMode
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
    float baseRadius = 100;
    float mAlphaTarget=1;
    float mAlpha=0;
    
    RobotEye bigEye;
    RobotEye littleEye;
    void visualizeAudio();
};
