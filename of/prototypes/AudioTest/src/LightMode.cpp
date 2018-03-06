//
//  LightMode.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightMode.h"


void LightMode::setup( std::shared_ptr<LightCircle> assistantLightRef  )
{
    //Tweenzor::init();
    
    mColorPalette = { ofFloatColor::red, ofFloatColor::blue, ofFloatColor::yellow };
    mAssistantLightRef = assistantLightRef;
    mModel = Model::getInstance();
}

void LightMode::update()
{
    //Tweenzor::update(ofGetElapsedTimeMillis());
    //mAssistantLightRef->update();
    
    // remove dead circles
    for (auto i = mCircles.begin(); i != mCircles.end();) {
        
        if (!(*i)->isAlive()) {
            i = mCircles.erase(i);
        }
        else {
            (*i)->update();
            ++i;
        }
    }
    
}

void LightMode::draw()
{
    ofPushMatrix();
    for (auto circle : mCircles) {
        circle->draw();
    }
    //mAssistantLightRef->draw();
    ofPopMatrix();
}

ofVec2f LightMode::getCenter()
{
    return ofGetWindowSize() * 0.5f;
}

ofVec2f LightMode::getRandomPointInScreen()
{
    return ofVec2f(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
}

void LightMode::setAudioLevel(float level)
{
    mPrevAudioLevel = mAudioLevel;
    mAudioLevel = level;
}

void LightMode::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightMode::animateIn" << endl;
    removeColors();
    mAssistantLightRef->animateRadius(0);
    mAssistantLightRef->animatePosition( ofGetWindowSize()*0.5f );
}

void LightMode::animateOut()
{
    // reset, clean up anything when switching of this mode.
    cout << "LightMode::animateOut" << endl;
    
    // animate out all additional lights
    for (int i = 0; i < mCircles.size(); i++ ) {
        mCircles[i]->animateOut(0.7f, i*0.02f);
    }
    
    // reset color mode
    mUseColors = false;
    
    // reset size
    mScale = 1.0f;
}


void LightMode::setQuestionState()
{
    cout << "LightMode::setQuestionState" << endl;
    float rad = (mAssistantLightRef->mRadius < 1)? mRadiusOrig : mAssistantLightRef->mRadius;
    
    mAssistantLightRef->onAnimationComplete( [this, rad](){
        mAssistantLightRef->animateRadius(rad, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
    } );
    mAssistantLightRef->animateRadius(rad * 1.2, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}



void LightMode::setResponseState()
{
    mAssistantLightRef->setSoundReactiveness(true);
}

void LightMode::setNormalState()
{
    cout << "LightMode::setNormalState" << endl;
    mAssistantLightRef->setSoundReactiveness(false);
    mAssistantLightRef->animateRadius(0, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}

void LightMode::addColors()
{
    mUseColors = true;
    
    ofFloatColor *newColor = &mColorPalette.at((int)ofRandom(mColorPalette.size()));
    ofFloatColor *lastColor = newColor;
    
    mAssistantLightRef->animateColor(ofFloatColor(*lastColor));
    
    for (auto circle : mCircles) {
        // randomize without repeating last color
        do {
            newColor = &mColorPalette.at((int)ofRandom(mColorPalette.size()));
        }
        while (lastColor == newColor);
        lastColor = newColor;
        circle->animateColor(ofFloatColor(*newColor));
    }
}

void LightMode::removeColors()
{
    mUseColors = false;
    mAssistantLightRef->animateColor(ofFloatColor::white);
    for (auto circle : mCircles) {
        circle->animateColor(ofFloatColor::white);
    }
}

void LightMode::bigger()
{
    updateSize(mScale*1.5f);
}

void LightMode::smaller()
{
    updateSize(mScale/1.5f);
}

void LightMode::updateSize(float scale)
{
    mScale = scale;
    mAssistantLightRef->animateRadius(mRadiusOrig * mScale);
    for (auto circle : mCircles) {
        if (!circle->isDying()) {
            circle->animateRadius(mRadiusOrig * mScale);
        }
    }
}

