//
//  LightModeRobot.cpp
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/21/18.
//

#include "LightModeRobot.h"


void LightModeRobot::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );
    mRadiusOrig = 300;
    
    bigEye.setup(getCenter(), .1, 500);
    littleEye.setup(getCenter(), .1, 4);
    littleEye.mOuterBlurColor = ofFloatColor(0.0, 0.15);
}

void LightModeRobot::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeLight::animateIn" << endl;
//LightMode::animateIn();
    mCurState = STATE_IN;
    bigEye.setLerpEnabled(false);
    littleEye.setLerpEnabled(false);
    bigEye.animateRadiusTo(mRadiusOrig);
}

void LightModeRobot::animateOut(float duration)
{
    // reset, clean up anything when switching off this mode.
    cout << "LightModeLight::animateOut" << endl;
    bigEye.setLerpEnabled(false);
    littleEye.setLerpEnabled(false);
    bigEye.animateRadiusTo(0);
    littleEye.animateRadiusTo(0);
    LightMode::animateOut(duration);
}

void LightModeRobot::update(){
    //DS: only update as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    
    bigEye.update();
    littleEye.update();
    
    //also don't update if state is going out
    if(mCurState == STATE_OUT_START) return;
    
    switch(mModel->getCurState()){
        case Model::STATE_NORMAL:
            //nothing
            break;
        case Model::STATE_QUESTION:
            //nothing
            break;
        case Model::STATE_RESPONSE:
            //rings are audio reactive
            visualizeAudio();
            break;
        default:
            break;
    }
}
void LightModeRobot::visualizeAudio(){
    //called from update
    bigEye.mTargInnerSize = .8;
    littleEye.mTargInnerSize = .2;
    
    float vol = mModel->mVolCur;
    float volScaled = ofMap(vol, 0.0, 0.02 * mModel->volumeScaler, 0.0, 1.0, true);
//    cout << "vol:"<< vol << " volScaled:"<< volScaled << endl;
    
    if(volScaled > 0.3){
//        float tRadius = clamp();
        bigEye.mTargetOuterRadius = ofMap(volScaled, .3, 1, mRadiusOrig, ofGetHeight()*.5, true);
    }
    if(volScaled > 0.5){
        littleEye.mTargetOuterRadius = ofMap(volScaled, .5, 1, bigEye.mTargetOuterRadius*.4, bigEye.mTargetOuterRadius*.8, true);
    }else if(volScaled < 0.2){
        littleEye.mTargetOuterRadius = 4;
    }else{
        //nothing
    }
}

void LightModeRobot::draw(){
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    //draw eye
    bigEye.draw();
    littleEye.draw();
}

void LightModeRobot::setQuestionState()
{
    cout << "LightModeLight::setQuestionState" << endl;

    bigEye.setLerpEnabled(false);
    littleEye.setLerpEnabled(false);
    bigEye.animateRadiusTo(ofGetHeight()*.5);
    littleEye.animateRadiusTo(0);
    isWaitingForResponse = true;
}

void LightModeRobot::setResponseState()
{
    cout << "LightModeLight::setResponseState" << endl;
    bigEye.isBreathing = false;
    littleEye.isBreathing = false;
    bigEye.setLerpEnabled(true);
    littleEye.setLerpEnabled(true);
    
    isWaitingForResponse = false;
}

void LightModeRobot::setNormalState()
{
    cout << "LightModeLight::setNormalState" << endl;
    bigEye.isBreathing = true;
    littleEye.isBreathing = true;
    bigEye.setLerpEnabled(false);
    littleEye.setLerpEnabled(false);
    bigEye.animateRadiusTo(mRadiusOrig);
    littleEye.animateRadiusTo(0);
}

void LightModeRobot::reloadShaders()
{
    GlowShapes::reloadShaders();
}

