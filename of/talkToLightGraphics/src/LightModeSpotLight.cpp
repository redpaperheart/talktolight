//
//  LightModeSpotlight
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#include "LightModeSpotlight.h"


void LightModeSpotlight::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );
    mBaseRadius = 280;
    mCenterBodyRadius = 400;
    
    spotLight.mRadius = mBaseRadius;
    spotLight.mGlowRadius = mBaseRadius * 0.05;
    spotLight.setTargetRadiusEnabled(true);
    spotLight.setFilled(true);
    spotLight.setColor(ofFloatColor::fromHex(0xffef68));
    spotLight.setGlowColor(ofFloatColor::fromHex(0xfce340, 0.5f));
    spotLight.mPosition = getNonCenter();
}

void LightModeSpotlight::reloadShaders()
{
    GlowShapes::reloadShaders();
}

void LightModeSpotlight::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeSpotlight::animateIn" << endl;
    mCurState = STATE_IN;
    
    mIdleCounter = ofGetElapsedTimeMillis();
    spotLight.animatePosition(getNonCenter());
}

void LightModeSpotlight::animateOut(float duration)
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeEt::animateOut" << endl;
    
    //move spot light off screen
    spotLight.animatePosition(getCenter()-ofVec2f(0.0, -ofGetHeight() - mBaseRadius));
    
    LightMode::animateOut(duration);
}

ofVec2f LightModeSpotlight::getNonCenter(){
    ofVec2f randPos = ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
    //push out of the center if its in the center rect
    float centerSpace = mCenterBodyRadius;
    if(randPos.x> getCenter().x - centerSpace && randPos.x < getCenter().x + centerSpace){
        if(randPos.y> getCenter().y - centerSpace && randPos.y < getCenter().y + centerSpace){
            //push it out of the center on the x
            if(randPos.x<getCenter().x){
                randPos.x -= centerSpace;
            }else{
                randPos.x += centerSpace;
            }
        }
    }
    return randPos;
}
void LightModeSpotlight::update(){
    //DS: only update as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    
    //also don't update if state is going out
    if(mCurState == STATE_OUT_START) return;
    
    spotLight.update();
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
            //wait for idle counter to pick a new location to go to
            if(mIdleCounter + mIdleDuration < ofGetElapsedTimeMillis()){
                mIdleCounter = ofGetElapsedTimeMillis();
                randomizePosition();
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
void LightModeSpotlight::randomizePosition(){
    float duration = 1;
    spotLight.mTargetRadius = ofRandom(mBaseRadius, mBaseRadius * 1.2);
    spotLight.animatePosition(getNonCenter(), duration);
    spotLight.animateRadius(spotLight.mTargetRadius, duration);
}
void LightModeSpotlight::visualizeAudio(){
    //called from update
    //scale the circle up and down with the snares
    spotLight.mTargetRadius = mBaseRadius * (1 + (mModel->mBeat.snare()*.5));
    spotLight.mGlowRadius = ofLerp(spotLight.mGlowRadius, spotLight.mRadius * 0.2, 0.15);
}

void LightModeSpotlight::draw(){
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    //draw spotlight
    spotLight.draw();
}

void LightModeSpotlight::setQuestionState()
{
    cout << "LightModeEt::setQuestionState" << endl;
    isWaitingForResponse = true;
    spotLight.animatePosition(getCenter(), .2, 0, EASE_OUT_QUAD);
    spotLight.setTargetRadiusEnabled(false);
    spotLight.animateRadius(mBaseRadius*1.5, 1, 0, EASE_OUT_ELASTIC);
}

void LightModeSpotlight::setResponseState()
{
    cout << "LightModeEt::setResponseState" << endl;
    //stay in middle.. or make sure in the middle
    spotLight.setTargetRadiusEnabled(true);
    isWaitingForResponse = false;
    spotLight.animatePosition(getCenter());
}

void LightModeSpotlight::setNormalState()
{
    cout << "LightModeEt::setNormalState" << endl;
    //push far offsceen
    spotLight.setTargetRadiusEnabled(false);
    randomizePosition();
    mIdleCounter = ofGetElapsedTimeMillis();
}

