//
//  LightMode.h
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#pragma once

#include "ofMain.h"

#ifdef __linux__ 
	#include "ofxTweenzor/src/ofxTweenzor.h"
#elif _WIN32 || _WIN64 || __APPLE__
	#include "ofxTweenzor.h"

#endif

#include "LightCircle.h"
#include "Model.h"

class LightMode {
  public:
    
    enum State{
        STATE_IN_START,
        STATE_IN,
        STATE_OUT_START,
        STATE_OUT
    };
    
    virtual void setup( std::shared_ptr<LightCircle> assistantLightRef );
    virtual void update();
    virtual void draw();
    
    // audio
    //virtual void setAudioLevel(float level); // [0-1]
    
    //states
    virtual void animateInWithDelay(float delay); // init, set circles sizes etc.
    virtual void onDelayComplete(float *arg);
    virtual void animateIn(); // init, set circles sizes etc.
    
    virtual void animateOut(float duration=1); // clean up, reset, e.g. remove additional circles, when switching modes.
    virtual void onAnimateOutComplete(float* arg); //called once animated out completely
    
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
    
    virtual void reloadShaders() {};
    virtual void resize(int w, int h) {};
    
    const State getState() const { return mCurState; }
    
  protected:
    float mTimer=0; //value used for tweening time, used for
    
    ofVec2f getCenter();
    ofVec2f getRandomPointInScreen();
    void updateSize(float scale = 1.0f);
    
    
   
    
    std::shared_ptr<LightCircle> mAssistantLightRef;
    
    State       mCurState = STATE_OUT;
    
	Model       *mModel;
    
    bool        mUseColors = false;
    float       mScale = 1.0f;
    
    float       mRadiusOrig = 100;
    
//    float       mAudioLevel = 0;
    float       mPrevAudioLevel = 0;
    
    bool        isWaitingForResponse = false;
    
    std::vector<ofFloatColor> mColorPalette;
    std::vector<std::shared_ptr<LightCircle>> mCircles;

};
