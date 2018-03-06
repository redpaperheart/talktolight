//
//  Light.cpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 5/15/17.
//
//

#include "Light.h"



void Light::setup()
{
    Tweenzor::init();
    //mColorPalette = { ofFloatColor::red, ofFloatColor::blue, ofFloatColor::yellow };
    
    mModel = Model::getInstance();
    
    mAssistantLightRef = std::make_shared<LightCircleAssistant>( ofGetWindowSize()*0.5f, 0 );
    
    mModeIdle = std::make_shared<LightMode>();
    mModeIdle->setup( mAssistantLightRef );
    
    mModeCat = std::make_shared<LightModeCat>();
    mModeCat->setup( mAssistantLightRef );
    
    mModeDisco = std::make_shared<LightModeDisco>();
    mModeDisco->setup( mAssistantLightRef );
    
    mModeLight = std::make_shared<LightModeLight>();
    mModeLight->setup( mAssistantLightRef );
    
    mModeCurrent = mModeIdle;
}

void Light::update()
{
    Tweenzor::update(ofGetElapsedTimeMillis());
    
    mAssistantLightRef->update();
   
    if(mModeCurrent) mModeCurrent->update();
    //mModeIdle->update();
    //mModeCat->update();
    //mModeDisco->update();
    //mModeLight->update();
    
    
    
    // animate circles depending on mode
//    if (mMode == QUESTION) {
//        auto c = mCircles.back();
//        if (!c->isTweening()) {
//            c->mRad = 0.9 * c->mRad + 0.1 * (100.0 + 20.0 * sin(ofGetElapsedTimef()));
//            
//        }
//    }
//    else if (mMode == RESPONSE) {
//        if (mAudioLevel - mPrevAudioLevel > 0.015) {
//            int column = round(ofRandom(2, 8));
//            float x = column * ofGetWindowWidth() / 10.0;
//            auto c = std::make_shared<LightCircle>(ofVec2f(x, ofGetWindowHeight() / 2.0), ofRandom(30, 50) * mSizeFactor);
//            
//            c->setFilled(true);
//            c->animateRad(ofRandom(30, 50), 0.25 * mSizeFactor);
//            c->onAnimationComplete([c]{
//                c->animateOut(3.0);
//            });
//            mCircles.push_back(c);
//        }
//    }
    
    //cout << "isAssistantActive(): " << mModel->isAssistantActive() << endl;
}

void Light::draw()
{
    //mModeCurrent->draw();
    ofPushMatrix();
    mAssistantLightRef->draw();
    
    mModeIdle->draw();
    //ofTranslate(5,5);
    mModeCat->draw();
    //ofTranslate(5,5);
    mModeDisco->draw();
    //ofTranslate(5,5);
    mModeLight->draw();
    ofPopMatrix();
}

void Light::changeState(Model::State state){
    
    //if (state == mModel->getCurState()) return;
    
    if(!mModeCurrent) return;
    
    switch(state){
        case Model::STATE_NONE:
            break;
        case Model::STATE_NORMAL:
            mModeCurrent->setNormalState();
            break;
        case Model::STATE_QUESTION:
            mModeCurrent->setQuestionState();
            break;
        case Model::STATE_RESPONSE:
            mModeCurrent->setResponseState();
            break;
    }
    
    mModel->setCurState(state);
    
}

void Light::setMode(Model::Mode mode)
{
    if (mode == mModel->mCurMode) return;
    
    mModeCurrent->animateOut();
    switch(mode){
        case Model::MODE_NONE:
            break;
        case Model::MODE_IDLE:
            mModeCurrent = mModeIdle;
            break;
        case Model::MODE_CAT:
            mModeCurrent = mModeCat;
            break;
        case Model::MODE_DISCO:
            mModeCurrent = mModeDisco;
            break;
        case Model::MODE_LIGHTS:
            mModeCurrent = mModeLight;
            break;
    }
    mModeCurrent->animateIn();
    
    mModel->mCurMode = mode;
    
    mModel->mModeChangeTime = ofGetElapsedTimef();
    
    
    // create new circles depending on mode
//    if (mode == QUESTION) {
//        auto c = std::make_shared<LightCircle>(getCenter(), ofGetWindowWidth() * 2.0);
//        c->animateIn(2.0);
//        mCircles.push_back(c);
//    }
}



void Light::setAudioLevel(float level)
{
    mPrevAudioLevel = mAudioLevel;
    mAudioLevel = level;
    
    if(!mModeCurrent) return;
    mModeCurrent->setAudioLevel(level);
}

void Light::addLights()
{
    if(!mModeCurrent) return;
    mModeCurrent->addLight();
}

void Light::removeLights()
{
    if(!mModeCurrent) return;
    mModeCurrent->removeLight();
}

void Light::addColors()
{
    if(!mModeCurrent) return;
    mModeCurrent->addColors();
}

void Light::removeColors()
{
    if(!mModeCurrent) return;
    mModeCurrent->removeColors();
}

void Light::bigger()
{
    if(!mModeCurrent) return;
    mModeCurrent->bigger();
}

void Light::smaller()
{
    if(!mModeCurrent) return;
    mModeCurrent->smaller();
}

