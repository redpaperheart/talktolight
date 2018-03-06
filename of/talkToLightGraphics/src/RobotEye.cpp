//
//  RobotEye
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/26/2018.
//
//

#include "RobotEye.h"
#include "GlowShapes.h"

void RobotEye::setup(ofVec2f position, float innerPercentage, float outerRadius){
    mPosition = position; //center of the ring
    mColor = ofFloatColor(1, 0, 0); // red
    mInnerBlurColor = ofFloatColor(0, 0, 0); // black
    mOuterBlurColor = ofFloatColor(1, .2, .2, .25); // transparent red
    mInnerPercentage = innerPercentage;
    mOuterRadius = outerRadius;
}

void RobotEye::update(){
    mBreathSin += mBreathSinSpeed;
    mInnerPercentage += (mTargInnerSize-mInnerPercentage) * mLerpSpeed;
    if(isBreathing){
        mTargInnerSize = ofMap(sin(mBreathSin), -1, 1, .2f, .8);
        //also lerp the outter blur
    }
    if(mLerpRadiusEnabled){
        mOuterRadius += (mTargetOuterRadius-mOuterRadius) * mLerpSpeed;
    }
    //rotate the speed
//    mRotation += mRotationSpeed*mRotationScale;
    
//    if(lerpEnabled){
//        mShapeScale += (targetShapeScale-mShapeScale) * lerpSpeed;
//    }
}

void RobotEye::setLerpEnabled(bool _val){
    mLerpRadiusEnabled = _val;
    mTargetOuterRadius = mOuterRadius;
    //stop any tweens?
    Tweenzor::removeTween(&mOuterRadius);
}

void RobotEye::draw(){
    if(mOuterRadius < 5) return;
    
    ofFill();
    ofPushMatrix();
    ofTranslate(mPosition);
    ofSetColor(mColor);
    
    GlowShapes::setGlowColor(mOuterBlurColor);
    GlowShapes::drawCirc(0, 0, mOuterRadius, mOuterRadius * 0.25f);
    GlowShapes::setGlowColor(mInnerBlurColor);
    GlowShapes::drawCirc(0, 0, 0, mOuterRadius);
    
    ofPopMatrix();
}
void RobotEye::animateRadiusTo(float radius, float duration, float delay, int easing){
    if(mLerpRadiusEnabled) return; //can't tween while lerping
    Tweenzor::add(&mOuterRadius, mOuterRadius, radius, delay, duration, easing);
//    Tweenzor::addCompleteListener( Tweenzor::getTween(&mShapeScale), this, &ETRing::onAnimationShapeScaleComplete);
}

//void ETRing::pulse(float scaleA, float scaleB, float durationA, float durationB, float delay){
//
//    onAnimationComplete( [this, scaleB, durationB](){
//        animateScaleTo(scaleB, durationB, 0, EASE_IN_QUAD);
//    } );
//    animateScaleTo(scaleA, durationA, delay, EASE_OUT_QUAD);
//}
//void ETRing::onAnimationShapeScaleComplete(float* arg){
//    if (mCallback) {
//        mCallback();
//        mCallback = nullptr;
//    }
//}
//
//void ETRing::onAnimationComplete(std::function<void()> callback)
//{
//    mCallback = callback;
//}

