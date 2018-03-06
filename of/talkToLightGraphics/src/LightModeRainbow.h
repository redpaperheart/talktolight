//
//  LightModeRainbow.h
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/18/18.
//
//

#pragma once

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"

class LightModeRainbow : public LightMode {
  public:
    void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
    void update() override;
    void draw() override;
    void resize(int w, int h) override;
    void reloadShaders() override;

    //status
    void animateIn() override;
    void animateOut(float duration=1) override;

    void setQuestionState() override;
    void setResponseState() override;
    void setNormalState() override;

    // variations
    void addLight() override {};
    void removeLight() override {};
  
  private:
    void drawPlane(float x, float y, float w, float h);

  private:
    struct Spotlight {
        float t = 0;
        float pos = 0;
        float rad = 0;
        float vel = 0;
        float life = 0;
        float startTime = 0;
        float saturation = 0;
        bool alive = 0;
    } ;
    
    bool  mIsResponse = false;
    float mSensitivity;
    float mGlobalFade = 0;
    float mQuestionHighlight = 0;
    const float mFboScale = 0.5;
    const int mMaxSpotLights = 28;
    
    ofFbo mFbo;
    ofShader mCircleShader;
    ofShader mStripeShader;
    ofPlanePrimitive mPlane;
    
    std::deque<float> mSoundHistory;
    std::vector<Spotlight> mSpotlights;
    
};
