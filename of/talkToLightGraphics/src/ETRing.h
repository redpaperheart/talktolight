//
//  ETRing.hpp
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/26/2018.
//
//

#pragma once

#include "ofMain.h"
#include "GlowShapes.h"

#ifdef __linux__
#include "ofxTweenzor/src/ofxTweenzor.h"
#elif _WIN32 || _WIN64 || __APPLE__
#include "ofxTweenzor.h"
#endif

class ETRing {
  public:
	ETRing();
	~ETRing();
    
    int mShapeCount=10; //number of shapes to draw in the ring
    float mRotation=0;
    float mRotationSpeed=0; // degrees per frame to rotate
    float mRotationScale=1;
    float mRadius=100;
    float mShapeScale = 1; //scale of each of the shapes
    ofVec2f mPosition; //center of the ring
    
    int mShapeId = 0; //0-circle 1-rect 2-?
    
    float mThickness = 5; //affects base radius of the circles and width height of rects
    float mCircGlow = 30;
    float mRectGlow = 80;
    
    bool    mIsDying = false;
    bool    mIsDead = false;
    
    void setup(int shapeCount, float radius, float rotationSpeed, ofVec2f position);
    void update();
    void draw();
    
    float targetShapeScale = 0;
    float lerpSpeed = .3;
    bool lerpEnabled = false;
    void setLerpEnabled(bool _val=true);
    
    
    void animateScaleTo(float scale = 1, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
    void onAnimationComplete(std::function<void()> callback);
    void pulse(float scaleA, float scaleB, float durationA, float durationB, float delay=0);
    void onAnimationShapeScaleComplete(float* arg);
    
  private:
    std::function<void()> mCallback = nullptr;

};
