//
//  Light.cpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 5/15/17.
//
//

#include "LightCircle.h"

int LightCircle::sIdCounter = 0;

LightCircle::LightCircle(ofVec2f pos, float rad)
{
    mPosition = pos;
    mRadius = rad;
    mId = sIdCounter ++;
    mOffset = ofVec2f(0);
    mModel = Model::getInstance();
}

LightCircle::~LightCircle()
{
    Tweenzor::removeTween(&mRadius);
    auto pos = Tweenzor::getTween(&mCatTarget);
    if (pos) pos->remove();
    auto target = Tweenzor::getTween(&mCatTarget);
    if (target) target->remove();
}

void LightCircle::update()
{
    if(mTargetRadiusEnabled){
        mRadius += (mTargetRadius-mRadius) * mRadiusSpeed;
    }
}

void LightCircle::draw()
{
    if (mRadius <= 0.0f) return;
    
    ofPushStyle();
    ofPushMatrix();
    ofFill();
    ofSetColor(mColor);
    GlowShapes::setGlowColor(mGlowColor);
    
    if (mFilled || mRadius - mThickness <= 0) {
//    if (mFilled) {
        GlowShapes::drawCirc(mPosition.x, mPosition.y, mRadius, mGlowRadius);
    }
    else {
        GlowShapes::drawRing(mPosition.x, mPosition.y, mRadius - mThickness, mRadius, mGlowRadius);
    }
    
    ofPopMatrix();
    ofPopStyle();    
}


void LightCircle::animateOut(float duration, float delay)
{
    if( mIsDying ) return;
    mIsDying = true;
    animateRadius(0, duration, delay);
    Tweenzor::addCompleteListener( Tweenzor::getTween(&mRadius), this, &LightCircle::onAnimateOutComplete);
}

void LightCircle::animateRadius(float targetRadius, float duration, float delay, int easing)
{
    Tweenzor::add(&mRadius, mRadius, targetRadius, delay, duration, easing);
    Tweenzor::addCompleteListener( Tweenzor::getTween(&mRadius), this, &LightCircle::onAnimateRadiusComplete);
}

void LightCircle::animatePosition( ofVec2f pos, float duration, float delay, int easing){
    Tweenzor::add(&mPosition, mPosition, pos, delay, duration, easing);
}

void LightCircle::animateColor(ofFloatColor color, float duration, float delay, int easing)
{
    Tweenzor::add(&mColor, mColor, color, delay, duration, easing);
}


void LightCircle::onAnimateOutComplete(float* arg)
{
    mIsDying = true;
    mIsDead = true;
}

void LightCircle::onAnimateRadiusComplete(float* arg)
{
    if (mCallback) {
        mCallback();
        mCallback = nullptr;
    }
}

void LightCircle::onAnimationComplete(std::function<void()> callback)
{
    mCallback = callback;
}
