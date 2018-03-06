//
//  LightModeCat.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightModeCat.h"


void LightModeCat::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    //Tweenzor::init();
    //mColorPalette = { ofFloatColor::red, ofFloatColor::blue, ofFloatColor::yellow };
    LightMode::setup( assistantLightRef );
    LightMode::animateOut();
    
    mRadiusOrig = 10;
}

void LightModeCat::update()
{
//    Tweenzor::update(ofGetElapsedTimeMillis());
    LightMode::update();
    
    
    if (!mAssistantLightRef->isPositionTweening() && !mModel->isAssistantActive()) {
        if (ofGetElapsedTimef() > mAssistantLightRef->mCatNextUpdate ) {
            float duration = ofRandom(0.5f, 4.0f);
            ofVec2f target = getRandomPointInScreen();
            Tweenzor::add(&mAssistantLightRef->mCatTarget, mAssistantLightRef->mCatTarget, target, 0, min(duration, 2.0f));
            mAssistantLightRef->mCatNextUpdate = ofGetElapsedTimef() + duration;
        }
        mAssistantLightRef->mPosition.interpolate(mAssistantLightRef->mCatTarget, 0.04);
    }
    
    for (auto c : mCircles) {
        if (!c->isPositionTweening()) {
            // move target to a new position
            if (ofGetElapsedTimef() > c->mCatNextUpdate ) {
                float duration = ofRandom(0.5f, 4.0f);
                ofVec2f target = getRandomPointInScreen();
                Tweenzor::add(&c->mCatTarget, c->mCatTarget, target, 0, min(duration, 2.0f));
                c->mCatNextUpdate = ofGetElapsedTimef() + duration;
            }
            c->mPosition.interpolate(c->mCatTarget, 0.04);
        }
    }
    
}

void LightModeCat::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeCat::animateIn" << endl;
    //LightMode::animateIn();
    removeColors();
    mAssistantLightRef->animateRadius(mRadiusOrig);
}

void LightModeCat::animateOut()
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeCat::animateOut" << endl;
    LightMode::animateOut();
}

void LightModeCat::setQuestionState()
{
    cout << "LightModeCat::setQuestionState" << endl;
    float rad = (mAssistantLightRef->mRadius < 30)? 30 : mAssistantLightRef->mRadius;
    
    mAssistantLightRef->onAnimationComplete( [this, rad](){
        mAssistantLightRef->animateRadius(rad, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
    } );
    mAssistantLightRef->animateRadius(rad * 1.2, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}

void LightModeCat::setResponseState()
{
    mAssistantLightRef->setSoundReactiveness(true);
}

void LightModeCat::setNormalState()
{
    cout << "LightModeCat::setNormalState" << endl;
    mAssistantLightRef->setSoundReactiveness(false);
    mAssistantLightRef->animateRadius(mRadiusOrig * mScale, 0.5);
}


void LightModeCat::addLight()
{

    int numNewLights = ceil(float(mCircles.size()+1) * 0.3f);
    
    for (int i = 0; i < numNewLights; i++ ) {

        auto c = std::make_shared<LightCircle>(getRandomPointInScreen(), 0);
        if (mUseColors) {
            c->setColor(mColorPalette.at((int)ofRandom(mColorPalette.size())));
        }
        //c->setFilled(true);
        c->animateRadius(mRadiusOrig * mScale);
        //c->mCatTarget = getRandomPointInScreen();
        //c->mCatNextUpdate = ofGetElapsedTimef() + ofRandom(1.0f, 3.0f);
        mCircles.push_back(c);
    }
    
}

void LightModeCat::removeLight()
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
}

