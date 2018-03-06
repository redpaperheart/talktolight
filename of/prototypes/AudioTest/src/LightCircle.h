//
//  Light.hpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 5/15/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxTweenzor.h"
#include "Model.h"

class LightCircle {
  public:
    LightCircle(ofVec2f pos = ofVec2f(0), float rad = 0);
    ~LightCircle();
    
    virtual void update();
    virtual void draw();
    
//    void animateIn(float duration = 0.5, float delay = 0);
    virtual void animateOut(float duration = 1.0, float delay = 0);
    
    virtual void animateRadius(float radius, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
    virtual void animatePosition( ofVec2f pos, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
    virtual void animateColor(ofFloatColor color, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
    
    virtual void onAnimationComplete(std::function<void()> callback);
    
    virtual void setFilled(bool filled)         { mFilled = filled; }
    virtual void setColor(ofFloatColor color)   { mColor = color; }
    virtual bool isAlive()                      { return !mIsDead; }
    virtual bool isDying()                      { return mIsDying; }
    virtual bool isDead()                       { return mIsDead; }
    virtual bool isRadiusTweening()             { return Tweenzor::getTween(&mRadius) != NULL; }
    virtual bool isPositionTweening()           { return Tweenzor::getTween(&mPosition) != NULL; }
    
    virtual void setSoundReactiveness(bool val){ mIsSoundReactive = val; }
  
    float   mRadius = 0;
    ofVec2f mPosition;
    
    // bad design, cat mode values should not be here:
    float   mCatNextUpdate = 0;
    ofVec2f mCatTarget;
    
  protected:
    virtual void onAnimateOutComplete(float* arg);
    //virtual void onAnimateInComplete(float* arg);
    virtual void onAnimateRadiusComplete(float* arg);
    
    static int  sIdCounter; // is that in use?
    
    bool        mIsDying = false;
    bool        mIsDead = false;
    
    bool        mFilled = false;
    int         mId = 0;
    float       mThickness = 10;
   
    ofFloatColor mColor = ofFloatColor::white;
    
    std::function<void()> mCallback = nullptr;
    
    Model       *mModel;
    
    bool  mIsSoundReactive = false;
    
    
    //ring functions
//    void drawRing(vector<ofVec2f> outside, vector<ofVec2f> inside);
//    
//    void updateOffsets( vector<ofVec2f> offsets, int _scale);
//    void resetOffsets(vector<ofVec2f> offsets);
//    
//    void plotCurves( vector<ofVec2f> points, vector<ofVec2f> offsets );
//    
//    void resizeRing(vector<ofVec2f> outer, vector<ofVec2f> inner);
//    
//    //void resizeRingRes(vector<ofVec2f> outer, vector<ofVec2f> inner);
//    
//    //circle params
//    //float mRad;
//    //float mThickness;
//    //bool mFilled;
//    //ofVec2f mPos;
//    //ofColor mColor;
//    float mPreviousRadius = 0;
//    float mSoudMultiplier = 125;
//    float mMinThreshold = 0.05;
//    float mMagnitude = 0;
//    float mXenoSpeed = 0.11;
//    float mResetSpeed = 0.8;
//    float mOffsetLimits = 5;
//    //array of all the points
//    vector<ofVec2f> outerPoints;
//    vector<ofVec2f> innerPoints;
//    vector<ofVec2f> offsetPoints;
//    
//    //path based on the arrays
//    ofPath mShape;
//    
//    int mRingResolution = 10; // the number of points in the circle
//    //int previousN;
//    
//    bool mDrawDebug = false;

};
