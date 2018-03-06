//
//  LightCircleAssistant.hpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/31/17.
//
//

#pragma once

#include "LightCircle.h"
#include "ofMain.h"
#include "ofxTweenzor.h"
#include "Model.h"

class LightCircleAssistant : public LightCircle {
  public:
    LightCircleAssistant(ofVec2f pos = ofVec2f(0), float rad = 0);
    ~LightCircleAssistant();
    
    void update() override;
    void draw() override;
    
    
    
  protected:
    
    //ring functions
    void drawRing(vector<ofVec2f> outside);//, vector<ofVec2f> inside);
    
    void updateOffsets( vector<ofVec2f> offsets, int _scale);
    void resetOffsets(vector<ofVec2f> offsets);
    
    void plotCurves( vector<ofVec2f> points, float radius, vector<ofVec2f> offsets );
    
    float mSoundMultiplier = 125;
    float mMinThreshold = 0.05;
    float mMagnitude = 0;
    float mXenoSpeed = 0.11;
    float mResetSpeed = 0.8;
    float mOffsetLimits = 5;
    
    //array of all the points
    vector<ofVec2f> outerPoints;
    vector<ofVec2f> offsetPoints;
    
    //path based on the arrays
    ofPath mShape;
    
    int mRingResolution = 10; // the number of points in the circle
    
    bool mDrawDebug = false;

};
