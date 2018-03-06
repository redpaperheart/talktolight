//
//  LightMode.h
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxTweenzor.h"

#include "LightCircle.h"
#include "Model.h"

class LightMode {
  public:
    
    virtual void setup( std::shared_ptr<LightCircle> assistantLightRef );
    virtual void update();
    virtual void draw();
    
    // audio
    virtual void setAudioLevel(float level); // [0-1]
    
    //states
    virtual void animateIn(); // init, set circles sizes etc.
    virtual void animateOut(); // clean up, reset, e.g. remove additional circles, when switching modes.
    
    virtual void setQuestionState();
    virtual void setResponseState();
    virtual void setNormalState();
    
    
    // variations
    virtual void addLight(){};
    virtual void removeLight(){};
    
    virtual void addColors();
    virtual void removeColors();
    
    virtual void bigger();
    virtual void smaller();
    
  protected:
    
    ofVec2f getCenter();
    ofVec2f getRandomPointInScreen();
    void updateSize(float scale = 1.0f);
    
    std::shared_ptr<LightCircle> mAssistantLightRef;
    
    Model       *mModel;
    
    bool        mUseColors = false;
    float       mScale = 1.0f;
    
    float       mRadiusOrig = 100;
    
    float       mAudioLevel = 0;
    float       mPrevAudioLevel = 0;
    
    std::vector<ofFloatColor> mColorPalette;
    std::vector<std::shared_ptr<LightCircle>> mCircles;

};
