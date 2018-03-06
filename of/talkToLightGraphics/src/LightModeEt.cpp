//
//  LightModeEt.cpp
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#include "LightModeEt.h"

void LightModeEt::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );
//    mRadiusOrig = 150;
    mActiveRingId = 0;
    for(int i=0; i<ringCount; i++){
        auto ring = std::make_shared<ETRing>();
        ring->mShapeScale = 0;
        ring->mThickness = 12;
        if(i % 2 == 0){
            //circle
            ring->setup(12 + (i*3), startRadius+(startRadius*i*.4), .25, ofVec2f(0,0));
            ring->mShapeId = 0;
        }else{
            //rect
            ring->setup(12 + (i*3), startRadius+(startRadius*i*.4), -.25, ofVec2f(0,0));
            ring->mShapeId = 1;
        }
        mRings.push_back(ring);
    }
    
    reloadShaders();
}

void LightModeEt::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeEt::animateIn" << endl;

    mCurState = STATE_IN;
    mActiveRingId = 0;
    mRings.at(mActiveRingId)->animateScaleTo(1.0, .25, 0.0);
    mLastRingSwap = ofGetElapsedTimeMillis()-ringSwapDuration;
}

void LightModeEt::animateOut(float duration)
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeEt::animateOut" << endl;
    for (auto ring : mRings) {
        ring->setLerpEnabled(false);
        ring->animateScaleTo(0, .2, 0);
        ring->mRotationScale = 1;
    }
    LightMode::animateOut();
}

void LightModeEt::update()
{
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    if(mCurState == STATE_OUT_START) return;
    
    //update rings
    for (auto ring : mRings) {
        ring->update();
    }
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
            //cycle through rings every ringSwapDuration
            if(mLastRingSwap + ringSwapDuration < ofGetElapsedTimeMillis()){
                mLastRingSwap = ofGetElapsedTimeMillis();
                //swap to another ring, pick a random one, not the current ring
                int rand = (int)ofRandom(1, ringCount-1);
                int newActiveRing = (mActiveRingId + rand) % ringCount;
                //swap them, old out new in
                mRings.at(mActiveRingId)->animateScaleTo(0, .5, 0);
                mRings.at(newActiveRing)->animateScaleTo(1, 1, .1);

                mActiveRingId = newActiveRing;
            }
            break;
        case Model::STATE_QUESTION:
            //nothing
            break;
        case Model::STATE_RESPONSE:
             //rings are audio reactive
            visualizeAudio();
            break;
    }
}

void LightModeEt::visualizeAudio()
{
    //called from update
    if(mModel->mBeat.snare()>=1){
        //bump up a random ring
        int r = (int)ofRandom(ringCount);
        mRings.at(r)->targetShapeScale = 1.5;
    }
    for (int i=0; i < mRings.size(); i++) {
        if(mRings.at(i)->targetShapeScale<.1){
            mRings.at(i)->targetShapeScale=0;
        }else{
            mRings.at(i)->targetShapeScale*=.95;
        }
    }
}

void LightModeEt::draw()
{
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    
    //draw each ring
    ofPushMatrix();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofTranslate( this->getCenter() );
    for (auto ring : mRings) {
        ring->draw();
    }
    ofPopStyle();
    ofPopMatrix();
}

void LightModeEt::setQuestionState()
{
    cout << "LightModeEt::setQuestionState" << endl;
    isWaitingForResponse = true;

    mRings.at(mActiveRingId)->animateScaleTo(0, .2, 0);
    for (int i=0; i < mRings.size(); i++) {
        mRings.at(i)->setLerpEnabled(false);
        mRings.at(i)->pulse(1.4, .5, .2, 1, i*.2);
        mRings.at(i)->mRotationScale = .3;
    }
}

void LightModeEt::setResponseState()
{
    cout << "LightModeEt::setResponseState" << endl;
    isWaitingForResponse = false;
     for (auto ring : mRings) {
         ring->mRotationScale = 1.0;
         ring->setLerpEnabled(true);
         ring->targetShapeScale = 0.0;
     }
}

void LightModeEt::setNormalState()
{
    cout << "LightModeEt::setNormalState" << endl;
    for (auto ring : mRings) {
        ring->setLerpEnabled(false);
        ring->animateScaleTo(0.0, 0.2, 0.0);
        ring->mRotationScale = .5;
    }
}

void LightModeEt::reloadShaders()
{
    GlowShapes::reloadShaders();
}

