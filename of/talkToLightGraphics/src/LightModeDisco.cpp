//
//  LightModeDisco.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightModeDisco.h"


void LightModeDisco::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    LightMode::setup( assistantLightRef );
    mRadiusOrig = 35;
}

void LightModeDisco::update()
{
    if(mCurState == STATE_OUT) return;
    
    LightMode::update();
    
    if ( mCurState == STATE_OUT_START ) return;
    
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            break;
        case Model::STATE_NORMAL:
            visualizeAudio();
            break;
        case Model::STATE_QUESTION:
            // something.. throw out some random dots?
            if(ofRandom(0.0, 1.0) >.9){
                addLightIdle();
            }
            break;
        case Model::STATE_RESPONSE:
            visualizeAudio();
            break;
    }
}

void LightModeDisco::visualizeAudio(){
    
    if(mCircles.size() < 30){
        if(mModel->mBeat.snare()>.9){
            addLightSnare();
        }
        if(mModel->mBeat.kick()>.9){
            addLightKick();
        }
        if(mModel->mBeat.hihat()>.9){
            addLightKick();
        }
    }
}

void LightModeDisco::draw()
{
    if(mCurState == STATE_OUT) return;
    
    ofPushMatrix();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    float rotation = 2.61803398875 * ofGetElapsedTimef();
    ofTranslate(ofGetWindowSize() * 0.5);
    ofRotate(rotation);
    ofTranslate(-ofGetWindowSize() * 0.5);
    
    for (auto circle : mCircles) {
        circle->mGlowRadius = circle->mRadius * 0.4f;
        circle->draw();
    }

    ofDisableBlendMode();
    ofPopStyle();
    ofPopMatrix();
}

void LightModeDisco::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeDisco::animateIn" << endl;
    mCurState = STATE_IN;
}

void LightModeDisco::animateOut(float duration)
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeDisco::animateOut" << endl;
    LightMode::animateOut(duration);
}

void LightModeDisco::setQuestionState()
{
    cout << "LightModeDisco::setQuestionState" << endl;
    float rad = mRadiusOrig * mScale;
    isWaitingForResponse = true;
}

void LightModeDisco::setResponseState()
{
    isWaitingForResponse = false;
}

void LightModeDisco::setNormalState()
{
    cout << "LightModeDisco::setNormalState" << endl;
}


void LightModeDisco::addLight()
{
    float angle = ofRandom(2 * M_PI);
    float dist = 0.5 * ofRandom(ofGetWindowHeight() - 50);
    ofVec2f pos = ofGetWindowSize() * 0.5 + dist * ofVec2f(cos(angle), sin(angle));
    auto c = std::make_shared<LightCircle>(pos);

    if (mUseColors) {
        c->setColor(mColorPalette.at((int)ofRandom(mColorPalette.size())));
    }
    c->setFilled(ofRandom(1.0f) > 0.25f);
    c->animateRadius(ofMap( mModel->mVolScaled, 0, 0.3, 10.0, 40.0) * mScale, 0.25);
    
    c->onAnimationComplete([c]{
        c->animateOut(1.5);
    });
    mCircles.push_back(c);
}

std::shared_ptr<LightCircle> LightModeDisco::addLightRadius(float radius, float dist, float inDur, float outDur, bool filled){
    float angle = ofRandom(2 * M_PI);
    ofVec2f pos = ofGetWindowSize() * 0.5 + dist * ofVec2f(cos(angle), sin(angle));
    auto c = std::make_shared<LightCircle>(pos);
    static const ofFloatColor blueGlow = ofFloatColor::fromHex(0xaff1ff);
    
    c->setColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    c->setGlowColor(ofFloatColor(blueGlow, ofRandom(0.15, 0.35)));
    
    c->setFilled(filled);
    c->animateRadius(radius, inDur);
    
    c->onAnimationComplete([c]{
        c->animateOut(1.5);
    });
    
    mCircles.push_back(c);
    return c;
}

void LightModeDisco::addLightIdle(){
    addLightRadius(ofRandom(6.0, 12.0), 0.2 * ofRandom(ofGetWindowHeight()), 2, 1);
}

void LightModeDisco::addLightSnare(){
    float r = ofMap( mModel->mVolCur * mModel->discoVolumeScaler * mModel->volumeScaler, 0, 0.3, 10.0, 40.0) * mScale;
    auto c = addLightRadius(r, 0.5 * ofRandom(ofGetWindowHeight() - 50), .1, .8, ofRandom(1.0f) > 0.25f);
}

void LightModeDisco::addLightKick(){
    float r = ofMap( mModel->mVolCur * mModel->discoVolumeScaler * mModel->volumeScaler, 0, 0.3, 40.0, 50.0) * mScale * 2;
    auto c = addLightRadius(r, 0.5 * ofRandom(ofGetWindowHeight() - 50), .1, .5, ofRandom(1.0f) > 0.25f);
}
void LightModeDisco::addLightHighhat(){
    float r = ofMap( mModel->mVolCur * mModel->discoVolumeScaler * mModel->volumeScaler, 0, 0.3, 5.0, 20.0) * mScale;
    auto c = addLightRadius(r, 150 + 0.5 * ofRandom(ofGetWindowHeight() - 350), .1, .3, ofRandom(1.0f) > 0.25f);
}

void LightModeDisco::removeLight()
{
    
}

void LightModeDisco::reloadShaders()
{
    GlowShapes::reloadShaders();
}

