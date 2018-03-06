//
//  LightMode.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightMode.h"


void LightMode::setup( std::shared_ptr<LightCircle> assistantLightRef  )
{
    mColorPalette = { ofFloatColor::red, ofFloatColor::blue, ofFloatColor::yellow };
    mAssistantLightRef = assistantLightRef;
    mModel = Model::getInstance();
}

void LightMode::update()
{
    if(mCurState == STATE_OUT) return;
    
    // remove dead circles
    for (auto i = mCircles.begin(); i != mCircles.end();) {
        if (!(*i)->isAlive()) {
            i = mCircles.erase(i);
        }
        else {
            (*i)->update();
            ++i;
        }
    }
    /*
    if(mCurState == STATE_OUT_START && mCircles.size() < 1){
        mCurState = STATE_OUT;
    }
     */
}

void LightMode::draw()
{
    // DS: assistant light is drawn in Light class
    if(mCurState == STATE_OUT) return;
    
    ofPushMatrix();
    for (auto circle : mCircles) {
        circle->draw();
    }
    ofPopMatrix();
}

ofVec2f LightMode::getCenter()
{
    return ofGetWindowSize() * 0.5f;
}

ofVec2f LightMode::getRandomPointInScreen()
{
    return ofVec2f(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
}

//void LightMode::setAudioLevel(float level)
//{
//    mPrevAudioLevel = mAudioLevel;
//    mAudioLevel = level;
//}
void LightMode::animateInWithDelay(float delay){
    Tweenzor::add(&mTimer, 0.0, 1.0, 0.0, delay, EASE_LINEAR);
    Tweenzor::addCompleteListener(Tweenzor::getTween(&mTimer), this, &LightMode::onDelayComplete);
}
void LightMode::onDelayComplete(float *arg){
    animateIn();
    mModel->resetIdleTimer();
    mModel->setCurState(Model::STATE_NORMAL);
    setNormalState();
}
void LightMode::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightMode::animateIn" << endl;
    mCurState = STATE_IN;
}

void LightMode::animateOut(float duration)
{
    // reset, clean up anything when switching of this mode.
    cout << "LightMode::animateOut" << endl;
    mCurState = STATE_OUT_START;
    //start a timer to set the state to OUT
	Tweenzor::add(&mTimer, 0.0f, 1.0f, 0.0f, duration, EASE_LINEAR);
    Tweenzor::addCompleteListener(Tweenzor::getTween(&mTimer), this, &LightMode::onAnimateOutComplete);
}

void LightMode::onAnimateOutComplete(float* arg){
    cout << "LightMode:: mode out complete: "<< endl;
    Tweenzor::removeCompleteListener(Tweenzor::getTween(&mTimer));
    mCurState = STATE_OUT;
}

void LightMode::setQuestionState()
{
    cout << "LightMode::setQuestionState" << endl;
    float rad = (mAssistantLightRef->mRadius < 1)? mRadiusOrig : mAssistantLightRef->mRadius;
    
    mAssistantLightRef->onAnimationComplete( [this, rad](){
        mAssistantLightRef->animateRadius(rad, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
    } );
    mAssistantLightRef->animateRadius(rad * 1.2, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
    isWaitingForResponse = true;
}

void LightMode::setResponseState()
{
    mAssistantLightRef->setSoundReactiveness(true);
    isWaitingForResponse = false;
}

void LightMode::setNormalState()
{
    cout << "LightMode::setNormalState" << endl;
    mAssistantLightRef->setSoundReactiveness(false);
    mAssistantLightRef->animateRadius(0, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}

void LightMode::addColors()
{
    mUseColors = true;
    
    ofFloatColor *newColor = &mColorPalette.at((int)ofRandom(mColorPalette.size()));
    ofFloatColor *lastColor = newColor;
    
    mAssistantLightRef->animateColor(ofFloatColor(*lastColor));
    
    for (auto circle : mCircles) {
        // randomize without repeating last color
        do {
            newColor = &mColorPalette.at((int)ofRandom(mColorPalette.size()));
        }
        while (lastColor == newColor);
        lastColor = newColor;
        circle->animateColor(ofFloatColor(*newColor));
    }
}

void LightMode::removeColors()
{
    mUseColors = false;
    mAssistantLightRef->animateColor(ofFloatColor::white);
    for (auto circle : mCircles) {
        circle->animateColor(ofFloatColor::white);
    }
}

void LightMode::bigger()
{
    updateSize(mScale*1.5f);
}

void LightMode::smaller()
{
    updateSize(mScale/1.5f);
}

void LightMode::updateSize(float scale)
{
    mScale = scale;
    mAssistantLightRef->animateRadius(mRadiusOrig * mScale);
    for (auto circle : mCircles) {
        if (!circle->isDying()) {
            circle->animateRadius(mRadiusOrig * mScale);
        }
    }
}

