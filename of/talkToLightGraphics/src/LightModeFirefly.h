//
//  LightModeFirefly.h
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/18/18.
//
//

#pragma once

#include <random>

#include "ofMain.h"
#include "LightCircle.h"
#include "LightMode.h"

class LightModeFirefly : public LightMode {
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
    
private:
    float randGaussian(); // 0, 1 approx
    float randGaussian(float a, float b);
    
private:
    struct Firefly {
        bool alive = false;
        float t = 0;
        float rad = 0;
        float life = 0;
        float startTime = 0;
        float saturation = 0;
        ofVec2f pos;
        ofFloatColor col;
    };
    
    bool mIsResponse = false;
    float mGlobalFade = 0;
    float mSmoothVol = 0;
    float mVolumeAlpha = 0;
    float mPrevVol = 0;
    float mLastTime = 0;
    float mElapsedTime = 0;
    float mCenterAttraction = 0;
    const int mMaxFireflies = 150;
    
    std::vector<Firefly> mFireflies;
    std::vector<ofColor> mPalette;
    
    std::mt19937 mBase;
    std::normal_distribution<float>	mNormDist{0.5f, 0.4f};

    // vbo setup
    ofShader mShader;
    vector <float> mSizes;
    vector <float> mUniqueOffsets;
    vector <ofVec3f> mPoints;
    vector <ofFloatColor> mColors;
    ofVbo mVbo;
    
};
