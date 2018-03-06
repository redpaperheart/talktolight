//
//  Light.hpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 5/15/17.
//
//

#pragma once

#include "ofMain.h"

#include "LightMode.h"
#include "LightModeCat.h"
#include "LightModeDisco.h"
#include "LightModeLight.h"
#include "LightCircle.h"
#include "LightCircleAssistant.h"
#include "Model.h"



class Light {
  public:
    
    void setup();
    void update();
    void draw();
    
    void setMode(Model::Mode mode);
    //Model::Mode getMode() { return mModel->mCurMode; }
    //void setPreviousMode();
    
    void changeState(Model::State state);
    
    void setAudioLevel(float level); // [0-1]
    
    void addLights();
    void removeLights();
    
    void addColors();
    void removeColors();
    
    void bigger();
    void smaller();
    
    
    
  private:
    
    
    std::shared_ptr<LightMode>          mModeIdle;
    std::shared_ptr<LightModeLight>     mModeLight;
    std::shared_ptr<LightModeDisco>     mModeDisco;
    std::shared_ptr<LightModeCat>       mModeCat;
    
    std::shared_ptr<LightMode>          mModeCurrent;
    
    Model *mModel;
    
    //void addCatLight();
    //void addDiscoLight();
    //void addNormalLight();
    
    //bool        mUseColors = false;
    //float       mSizeFactor = 1;
    float       mAudioLevel = 0;
    float       mPrevAudioLevel = 0;
    //float       mModeChangeTime = 0;
    
    std::shared_ptr<LightCircleAssistant> mAssistantLightRef;
    
    //std::vector<ofFloatColor> mColorPalette;
    //std::vector<std::shared_ptr<LightCircle>> mCircles;

};
