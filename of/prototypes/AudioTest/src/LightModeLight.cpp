//
//  LightModeCat.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightModeLight.h"


void LightModeLight::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    //Tweenzor::init();
    //mColorPalette = { ofFloatColor::red, ofFloatColor::blue, ofFloatColor::yellow };
    LightMode::setup( assistantLightRef );
    mRadiusOrig = 200;
}

void LightModeLight::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeLight::animateIn" << endl;
    //LightMode::animateIn();
    removeColors();
    mAssistantLightRef->animateRadius(mRadiusOrig);
    mAssistantLightRef->animatePosition( ofGetWindowSize()*0.5f );
}

void LightModeLight::animateOut()
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeLight::animateOut" << endl;
    LightMode::animateOut();
}

void LightModeLight::setQuestionState()
{
    cout << "LightModeLight::setQuestionState" << endl;
    float rad = (mAssistantLightRef->mRadius < 1)? mRadiusOrig : mAssistantLightRef->mRadius;
    
    mAssistantLightRef->onAnimationComplete( [this, rad](){
        mAssistantLightRef->animateRadius(rad, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
    } );
    mAssistantLightRef->animateRadius(rad * 1.2, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}

void LightModeLight::setResponseState()
{
    cout << "LightModeLight::setResponseState" << endl;
    mAssistantLightRef->setSoundReactiveness(true);
}

void LightModeLight::setNormalState()
{
    cout << "LightModeLight::setNormalState" << endl;
    mAssistantLightRef->setSoundReactiveness(false);
}


void LightModeLight::addLight()
{
    int numNewLights = mCircles.size() < 3 ? 1 : 2;
    
    for (int i = 0; i < numNewLights; i++ ) {
        //add more than one light
        auto c = std::make_shared<LightCircle>(getCenter(), 0);
        if (mUseColors) {
            c->setColor(mColorPalette.at((int)ofRandom(mColorPalette.size())));
        }
        c->animateRadius(mRadiusOrig * mScale);
        
        //mCircles.insert(mCircles.begin() + (int)ofRandom(mCircles.size()), c);
        mCircles.push_back(c);
    }
    repositionLights();

}

void LightModeLight::removeLight()
{
    int numOfLights = 0;
    for (int i = 0; i < mCircles.size(); i++ ) {
        if( !mCircles[i]->isDying() ) numOfLights++;
    }
    if(numOfLights < 1) return;
    
    int numLights = max(1, (int)floor(float(numOfLights) * 0.25f));
    int i = 0;
    int count = 0;
    while( count < numLights ){
        if(mCircles[i]->isDying()){
            i++;
            continue;
        }else{
            mCircles[i]->animateOut(0.7f, i*0.02f);
            i++;
            count++;
        }
    }
    repositionLights();
}

void LightModeLight::repositionLights()
{
    // reposition lights
    int numOfLights = 1;
    
    for (int i = 0; i < mCircles.size(); i++ ) {
        if( !mCircles[i]->isDying() ) numOfLights++;
    }
    
    int count = 0;
    for (int i = 0; i < mCircles.size()+1; i++ ) {
        
        float x = ofGetWindowWidth() * (float(count + 1) / float(numOfLights + 1));
        float y = ofGetWindowHeight() * 0.5f;
        if( i < 1){
            mAssistantLightRef->animatePosition( ofVec2f(x, y) );
        }else{
            if( mCircles[i-1]->isDying() ) continue;
            mCircles[i-1]->animatePosition( ofVec2f(x, y) );
            //Tweenzor::add(&mCircles[i-1]->mPosition, mCircles[i-1]->mPosition, ofVec2f(x, y), 0.0f, 0.5f, EASE_IN_OUT_QUAD);
        }
        count++;
    }
}

