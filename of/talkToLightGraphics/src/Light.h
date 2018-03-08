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
#include "LightModeLaser.h"
#include "LightModeDisco.h"
#include "LightModeLight.h"
#include "LightModeNLights.h"
#include "LightModeScanner.h"
#include "LightModeSpotlight.h"
#include "LightModeGoodNight.h"
#include "LightModeEt.h"
#include "LightModeRainbow.h"
#include "LightModeFirefly.h"
#include "LightModeSpaceship.h"
#include "LightCircle.h"
#include "LightCircleAssistant.h"

#include "Model.h"



class Light {
  public:
    
    void setup();
    void update();
    void draw();
    
    void resize(int w, int h);
    void reloadShaders();
    
    void setMode(Model::Mode mode);
    
    void changeState(Model::State state);



    Model::Mode startMode = Model::MODE_SPACESHIP;

    //void setAudioLevel(float level); // [0-1]
    
  private:
    
    std::shared_ptr<LightMode>          mModeIdle;
    std::shared_ptr<LightModeLight>     mModeLight;
    std::shared_ptr<LightModeDisco>     mModeDisco;
    std::shared_ptr<LightModeLaser>     mModeLaser;
	std::shared_ptr<LightModeNLights>   mModeNLights;
	std::shared_ptr<LightModeScanner>   mModeScanner;
    std::shared_ptr<LightModeSpotlight> mModeSpotLight;
    std::shared_ptr<LightModeSpaceship> mModeSpaceship;
    std::shared_ptr<LightModeEt>        mModeET;
    std::shared_ptr<LightModeFirefly>   mModeFireFly;
    std::shared_ptr<LightModeRainbow>   mModeRainbow;
    std::shared_ptr<LightModeGoodNight>   mModeGoodNight;
    std::shared_ptr<LightMode>          mModeCurrent;
    
    std::vector<std::shared_ptr<LightMode>> mModes;
    
    Model *mModel;
    
    std::shared_ptr<LightCircleAssistant> mAssistantLightRef;

};
