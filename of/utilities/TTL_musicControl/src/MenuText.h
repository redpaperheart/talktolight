//
//  MenuText.hpp
//  TTL_musicControl
//
//  Created by Greg Schomburg on 2/28/18.
//

#pragma once

#include "ofMain.h"
#include "ofxTweenzor.h"

class MenuText
{
public:
    void setup();
    void update();
    void draw();
    
    ofVec2f mPosition;
    ofFloatColor mCurrentColor;
    string mText = "";
    
    std::shared_ptr<ofTrueTypeFont> font;
    
    string mOSCId=""; //used for osc messages
    
    
    ofFloatColor highlightColor;
    ofFloatColor whiteColor;
    ofFloatColor dimColor;
    
    ofFloatColor activeColor;
    ofFloatColor inactiveColor;
    
    bool isActive;
    
    void animateToActiveColor();
    void animateToInactiveColor();
    void animateOut();
    void animateToColor(ofFloatColor color, float duration =1);
protected:
    
};

