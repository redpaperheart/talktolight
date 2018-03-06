//
//  RobotEye
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/26/2018.
//
//

#pragma once

#include "ofMain.h"

#ifdef __linux__
#include "ofxTweenzor/src/ofxTweenzor.h"
#elif _WIN32 || _WIN64 || __APPLE__
#include "ofxTweenzor.h"
#endif

class RobotEye {
  public:
    
    
    ofVec2f mPosition; //center of eye
    
    float mOuterRadius=100;
    float mTargetOuterRadius=100;
    float mInnerPercentage=.4; //inner percentage of the outer ring
    float mInnerBlur=0;
    float mOuterBlur=0;
    
    ofFloatColor mColor;
    ofFloatColor mInnerBlurColor;
    ofFloatColor mOuterBlurColor;
    
    float mLerpSpeed=.2;
    float mTargInnerSize=.4;
    float mBreathSin=0;
    float mBreathSinSpeed=.01;
    
    float isBreathing = true;
    
    void setup(ofVec2f position, float innerPercentage, float outerRadius);
    void update();
    void draw();
    
    bool mLerpRadiusEnabled = false;
    void setLerpEnabled(bool _val=true);
    
    void animateRadiusTo(float radius = 400, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);

};
