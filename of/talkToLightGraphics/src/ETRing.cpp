//
//  ETRing.cpp
//  talkToLightGraphics
//
//  Created by Greg Schomburg on 2/26/2018.
//
//

#include "ETRing.h"

//int LightCircle::sIdCounter = 0;

ETRing::ETRing()
{

}

ETRing::~ETRing()
{

}

void ETRing::setup(int shapeCount, float radius, float rotationSpeed, ofVec2f position){
    mShapeCount = shapeCount; //number of shapes to draw in the ring
    mRotationSpeed = rotationSpeed; // degrees per frame to rotate
    mRadius = radius;
    mPosition = position; //center of the ring
}

void ETRing::update(){
    //rotate the speed
    mRotation += mRotationSpeed*mRotationScale;
    
    if(lerpEnabled){
        mShapeScale += (targetShapeScale-mShapeScale) * lerpSpeed;
    }
}

void ETRing::setLerpEnabled(bool _val){
    lerpEnabled = _val;
    targetShapeScale = mShapeScale;
    //stop any tweens?
    Tweenzor::removeTween(&mShapeScale);
}

void ETRing::draw(){
    ofFill();
    ofSetColor(ofFloatColor(1.0, 0.58, 0.08, 1.0));
    GlowShapes::setGlowColor(ofFloatColor(0.45, 0.0, 0.0, 0.5));
    ofPushMatrix();
    ofRotateZ(mRotation);
    
    //draw ring shapes
    for(int m=0; m<mShapeCount; m++){
         //draw a dot then rotate the
        ofPushMatrix();
        ofTranslate(0, mRadius, 0);
//        mShapeScale = 0.30f;
        
        if (mShapeScale > 0.0f) {
            if(mShapeId==0){
                GlowShapes::drawCirc(0, 0, mThickness * mShapeScale, mCircGlow * mShapeScale);
            }
            else {
                float w = mThickness * 5;
                float h = mThickness * 2 * mShapeScale;
                float glow = mRectGlow * mShapeScale;
//                GlowShapes::drawRect(w * -.5, h * -.5, w, h, glow);
                GlowShapes::drawRect(w * -.5, h * -.5, w, h, glow);
            }
        }
        ofPopMatrix();
        ofRotateZ(360/mShapeCount); //rotate to next shape
    }
    ofPopMatrix();
}

void ETRing::animateScaleTo(float scale, float duration, float delay, int easing){
    if(lerpEnabled) return; //can't tween while lerping
    Tweenzor::add(&mShapeScale, mShapeScale, scale, delay, duration, easing);
    Tweenzor::addCompleteListener( Tweenzor::getTween(&mShapeScale), this, &ETRing::onAnimationShapeScaleComplete);
}

void ETRing::pulse(float scaleA, float scaleB, float durationA, float durationB, float delay){
    
    onAnimationComplete( [this, scaleB, durationB](){
        animateScaleTo(scaleB, durationB, 0, EASE_IN_QUAD);
    } );
    animateScaleTo(scaleA, durationA, delay, EASE_OUT_QUAD);
}

void ETRing::onAnimationShapeScaleComplete(float* arg){
    if (mCallback) {
        mCallback();
        mCallback = nullptr;
    }
}

void ETRing::onAnimationComplete(std::function<void()> callback)
{
    mCallback = callback;
}
