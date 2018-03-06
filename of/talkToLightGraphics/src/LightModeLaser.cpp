//
//  LightModeLaster.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightModeLaser.h"


void LightModeLaser::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    LightMode::setup( assistantLightRef );
    mRadiusOrig = 10;
    mScale = 1;

    mLaserPointer = std::make_shared<LightCircleTrail>(getRandomPointInScreen(), 0);
    mLaserPointer->setColor(ofFloatColor::fromHex(0xed1c24));
    mLaserPointer->setFilled(true);
    mLaserPointer->animateRadius(mRadiusOrig * mScale);
    mLaserPointer->mCatTarget = getRandomPointInScreen();
    mLaserPointer->mCatTarget.x = ofMap(mLaserPointer->mCatTarget.x, 0, ofGetWidth(), ofGetWidth()*0.25, ofGetWidth()*0.75);
    mLaserPointer->mCatTarget.y = ofMap(mLaserPointer->mCatTarget.y, 0, ofGetHeight(), 110+mLaserPointer->mRadius*10, ofGetHeight()-110-mLaserPointer->mRadius);
}

void LightModeLaser::update()
{
    
    if(mCurState == STATE_OUT) return;
    mLaserPointer->update();
    if(mCurState == STATE_OUT_START) return;
    
    ofVec2f target;
    float duration=0;
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
            
            target = getRandomPointInScreen();
            duration = ofRandom(0.1f, 4.0);
            newLaserTarget(target, duration);
            break;
        case Model::STATE_QUESTION:
            //nothing
            target = getRandomPointInCenter();
            duration = ofRandom(0.2f, 0.3);
            newLaserTarget(target, duration);
            break;
        case Model::STATE_RESPONSE:
            if(mModel->mBeat.snare()>=.9){
                target = getRandomPointInScreen();
                duration = ofRandom(0.05f, 0.1);
                newLaserTarget(target, duration, true);
            }
            break;
    }
    
    int counter = 0;
    ofVec2f sinTarget = ofVec2f( sin(ofGetElapsedTimef()*3 + counter) * 30,
                                cos(ofGetElapsedTimef()*3 + counter) * 90 );
    mLaserPointer->mPosition.interpolate(mLaserPointer->mCatTarget + sinTarget, 0.1);
}

void LightModeLaser::newLaserTarget(ofVec2f target, float duration, bool force){
    if (force || !mLaserPointer->isPositionTweening()) {
        if (force || ofGetElapsedTimef() > mLaserPointer->mCatNextUpdate ) {
            
            //make sure the target keeps the circle inside the screen
            target.x = ofMap(target.x, 0, ofGetWidth(), ofGetWidth()*0.25, ofGetWidth()*0.75);
            target.y = ofMap(target.y, 0, ofGetHeight(), 110+mLaserPointer->mRadius, ofGetHeight()-100-mLaserPointer->mRadius);
            Tweenzor::add(&mLaserPointer->mCatTarget, mLaserPointer->mCatTarget, target, 0, min(duration, 2.0f));
            mLaserPointer->mCatNextUpdate = ofGetElapsedTimef() + duration;
        }
    }
}
ofVec2f LightModeLaser::getRandomPointInCenter(){
    ofVec2f pt = getCenter();
    //offset by small random ammounts
    float dis=200;
    pt += ofVec2f(ofRandom(-dis, dis), ofRandom(-dis, dis));
    return pt;
    
}

void LightModeLaser::draw()
{
    if(mCurState == STATE_OUT) return;
    
    ofPushStyle();
    ofFill();
    ofSetColor(mLaserPointer->getColor(), 100);
    mLaserPointer->draw();
    ofPopStyle();
}

void LightModeLaser::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeLaster::animateIn" << endl;
    mCurState = STATE_IN;
    mLaserPointer->animatePosition(getCenter());
}

void LightModeLaser::animateOut(float duration)
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeLaster::animateOut" << endl;
    mLaserPointer->animatePosition(getCenter()-ofVec2f(0, ofGetHeight() +200));
    LightMode::animateOut(duration);
}

void LightModeLaser::setQuestionState()
{
    cout << "LightModeLaster::setQuestionState" << endl;
    mLaserPointer->animatePosition(getCenter());
    isWaitingForResponse = true;
}

void LightModeLaser::setResponseState()
{
    isWaitingForResponse = false;
}

void LightModeLaser::setNormalState()
{
    cout << "LightModeLaster::setNormalState" << endl;
}
