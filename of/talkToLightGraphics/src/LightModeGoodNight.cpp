//
//  LightModeGoodNight
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#include "LightModeGoodNight.h"


void LightModeGoodNight::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );
    mBaseRadius = 100;
//    mCenterBodyRadius = 400;
    
    for(int s=0; s<10; s++){
        
        Star star;
        star.pos = ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        star.sinOffset =ofRandom(100000);
        star.sinPhase =ofRandom(.01, .05);
        star.glow = ofMap(sin(star.sinOffset), -1, 1, 1, 8);
        stars.push_back(star);
    }
    mStarAlpha = 0;
    moonPos = getCenter();
    moon.mRadius = mBaseRadius;
//    moon.setTargetRadiusEnabled(true);
    moon.setFilled(true);
    moon.setColor(ofFloatColor::fromHex(0xffef68));
    moon.setGlowColor(ofFloatColor::fromHex(0xfce340, 0.5f));
    moon.mPosition = moonPos;
    
    shadow.mRadius = mBaseRadius*1.1;
    shadow.mGlowRadius = mBaseRadius * 0.05;
//    shadow.setTargetRadiusEnabled(true);
    shadow.setFilled(true);
    shadow.setColor(ofFloatColor::fromHex(0x000000));
    shadow.setGlowColor(ofFloatColor::fromHex(0x000000, 0.5f));
    shadow.mPosition = moonPos;
}

void LightModeGoodNight::reloadShaders()
{
    GlowShapes::reloadShaders();
}

void LightModeGoodNight::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeGoodNight::animateIn" << endl;
    mCurState = STATE_IN;
    mStarAlpha = 0;
    Tweenzor::add(&mStarAlpha, 0.0, 1.0, 0, 1, EASE_OUT_QUAD);
    
    moon.mPosition = moonPos;
    shadow.mPosition = moonPos;
    
//    shadow.animateRadius(mBaseRadius * 1.1);
    shadow.animatePosition(moonPos + ofVec2f(mBaseRadius*.7, 0));
}

void LightModeGoodNight::animateOut(float duration)
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeEt::animateOut" << endl;
    
    Tweenzor::add(&mStarAlpha, mStarAlpha, 0.0, 0, 1, EASE_OUT_QUAD);
//     moon.mPosition = getCenter();
//    shadow.animateRadius(mBaseRadius * 1.6);
    shadow.animatePosition(moonPos);
    LightMode::animateOut(duration);
}
void LightModeGoodNight::update(){
    //DS: only update as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    
    //also don't update if state is going out
    if(mCurState == STATE_OUT_START) return;
    
    moon.update();
    shadow.update();
    for(int s=0; s<10; s++){
        
//        Star star;
//        star.pos = ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        stars[s].sinOffset += stars[s].sinPhase;
        stars[s].glow = ofMap(sin(stars[s].sinOffset), -1, 1, 1, 7);
    }
    
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
            //wait for idle counter to pick a new location to go to
//            if(mIdleCounter + mIdleDuration < ofGetElapsedTimeMillis()){
//                mIdleCounter = ofGetElapsedTimeMillis();
//                randomizePosition();
//            }
            break;
        case Model::STATE_QUESTION:
            //nothing
            break;
        case Model::STATE_RESPONSE:
            //rings are audio reactive
//            visualizeAudio();
            break;
    }
}

//void LightModeGoodNight::visualizeAudio(){
//    //called from update
//    //scale the circle up and down with the snares
//    spotLight.mTargetRadius = mBaseRadius * (1 + (mModel->mBeat.snare()*.5));
//    spotLight.mGlowRadius = ofLerp(spotLight.mGlowRadius, spotLight.mRadius * 0.2, 0.15);
//}

void LightModeGoodNight::draw(){
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    //draw spotlight
    moon.draw();
    shadow.draw();
    
    ofSetColor(ofFloatColor::fromHex(0xffef68, mStarAlpha));
    GlowShapes::setGlowColor(ofFloatColor::fromHex(0xfce340, 0.5f*mStarAlpha));
    for(int s=0; s<10; s++){
        
        GlowShapes::drawCirc(stars[s].pos.x, stars[s].pos.y, 4, stars[s].glow);
            
    }
}

void LightModeGoodNight::setQuestionState()
{
    cout << "LightModeEt::setQuestionState" << endl;
    isWaitingForResponse = true;
//    spotLight.animatePosition(getCenter(), .2, 0, EASE_OUT_QUAD);
//    spotLight.setTargetRadiusEnabled(false);
//    spotLight.animateRadius(mBaseRadius*1.5, 1, 0, EASE_OUT_ELASTIC);
}

void LightModeGoodNight::setResponseState()
{
    cout << "LightModeEt::setResponseState" << endl;
    //stay in middle.. or make sure in the middle
//    spotLight.setTargetRadiusEnabled(true);
    isWaitingForResponse = false;
//    spotLight.animatePosition(getCenter());
}

void LightModeGoodNight::setNormalState()
{
    cout << "LightModeEt::setNormalState" << endl;
    //push far offsceen
    moon.setTargetRadiusEnabled(false);
    shadow.setTargetRadiusEnabled(false);
//    randomizePosition();
//    mIdleCounter = ofGetElapsedTimeMillis();
}

