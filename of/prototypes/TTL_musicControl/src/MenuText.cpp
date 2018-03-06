//
//  MenuText.cpp
//  TTL_musicControl
//
//  Created by Greg Schomburg on 2/28/18.
//

#include "MenuText.h"
void MenuText::setup()
{
    mCurrentColor = ofFloatColor(1, 1, 1, 0);
    isActive=true;
}
void MenuText::update(){
    
}
void MenuText::draw(){
    ofPushMatrix();
    ofTranslate(mPosition);
    ofSetColor(mCurrentColor);
    font->drawString(mText, 0, 0);
    ofPopMatrix();
}
void MenuText::animateToActiveColor(){
//    if(!isActive){
        animateToColor(activeColor, .5);
//    }
    isActive=true;
}

void MenuText::animateToInactiveColor(){
//    if(isActive){
        animateToColor(inactiveColor, 1);
//    }
    isActive = false;
}

void MenuText::animateOut(){
    isActive = false;
    animateToColor(ofFloatColor(1,1,1,0));
}

void MenuText::animateToColor(ofFloatColor color, float duration){
     Tweenzor::add(&mCurrentColor, mCurrentColor, color, 0, duration, EASE_LINEAR);
}
