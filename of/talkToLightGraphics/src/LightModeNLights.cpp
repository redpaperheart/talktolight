//
//  LightNLightsMode.cpp
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/20/18.
//
//

#include "Model.h"
#include "LightModeNLights.h"

//--------------------------------------------------------------
void LightModeNLights::setup( std::shared_ptr<LightCircle> assistantLightRef){
    
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );
    mNoise1.load(mModel->assetPath + "/nLights/noise1.jpeg");
    mNoise2.load(mModel->assetPath + "/nLights/noise2.jpeg");
    mNoise3.load(mModel->assetPath + "/nLights/noise3.jpeg");
    mNoise4.load(mModel->assetPath + "/nLights/noise4.jpeg");
    mNoise5.load(mModel->assetPath + "/nLights/noise5.jpeg");
    
    xOffset = ofGetWindowWidth()/40;
    xStart = ofGetWindowWidth()/4;
    yStart = ofGetWindowHeight()/2;
    
    mPulseMult = -0.4;
    mPulseMultTar = 0.0;
    mTweenzorDuration = 0.35;
    
    mColor = ofVec3f( 0.5, 1.0, 0.8 );
    mColorDefault = mColor;
    mRespond = false;
    
    mRespondAlpha = 0.0;
    mRespondAlphaTar = 1.0;
    
    reloadShaders();
}

void LightModeNLights::reloadShaders()
{
#ifdef TARGET_OPENGLES
    mShaderNlights.load(mModel->assetPath + "/nLights/shaders_gles/nLights.vert", mModel->assetPath + "/nLights/shaders_gles/nLights.frag");
#else
    mShaderNlights.load(mModel->assetPath + "/nLights/shaders/nLights.vert", mModel->assetPath + "/nLights/shaders/nLights.frag");
#endif
//    mShaderNlights.bindDefaults();
}

void LightModeNLights::onResponse(float* arg)
{    
    Tweenzor::add(&mPulseMult, mPulseMult, mPulseMultTar, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
}

//--------------------------------------------------------------
void LightModeNLights::update(){
       //DS: only update as long as this mode is still active
    if(mCurState == STATE_OUT) return;
     
     mSin1+=.01;
     mAudio_1 = abs(sin(mSin1) * .15);
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
            break;
        case Model::STATE_QUESTION:	
			break;
        case Model::STATE_RESPONSE:
			colorResponseTweenOn();
			//mModel->mVolScaled / mModel->mVolTop > 0.5
			//mAudioTar_2 = ofMap( (mModel->mVolScaled / mModel->mVolTop), 0, .5, -0.15, .15);
			//mAudio_2 += (mAudioTar_2 - mAudio_2) *.2;
			
			
			if( mModel->mBeat.snare() > 0.7){
				audioTweenUp_1();
				colorResponseTweenOn(); 
			}
			if( mModel->mBeat.snare() > 0.5){
				audioTweenUp_2(); 
				colorResponseTweenOn();
			}
			if( mModel->mBeat.snare() > 0.9){
				audioTweenUp_3(); 
				colorResponseTweenOn();
			}
			//cout << mAudio << endl;
            break;
    }
}

//--------------------------------------------------------------

void LightModeNLights::draw(){
	
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;
        
    mShaderNlights.begin();
    ofFill();
    ofPushMatrix();
    ofPushStyle();
    mShaderNlights.setUniform1f( "uTime", ofGetElapsedTimef() );
    mShaderNlights.setUniform2f( "uRes", ofVec2f(W_WIDTH, W_HEIGHT ));
    mShaderNlights.setUniformTexture( "uNoise1", mNoise1, 1 );
    mShaderNlights.setUniformTexture( "uNoise2", mNoise2, 2 );
    mShaderNlights.setUniformTexture( "uNoise3", mNoise3, 3 );
    mShaderNlights.setUniformTexture( "uNoise4", mNoise4, 4 );
    mShaderNlights.setUniformTexture( "uNoise5", mNoise5, 5 );
    mShaderNlights.setUniform1f( "uPulseMult", mPulseMult );
    mShaderNlights.setUniform3f( "uColor", mColor );
    mShaderNlights.setUniform1i( "uRespond", mRespond );
    mShaderNlights.setUniform1i( "uRespondAlpha", mRespondAlpha );
    mShaderNlights.setUniform1i( "uRespondAlphaTar", mRespondAlphaTar );
    mShaderNlights.setUniform1f( "uAlphaMult", mAlphaMult );
    mShaderNlights.setUniform1f( "uAudio_1", mAudio_1 );
    mShaderNlights.setUniform1f( "uAudio_2", mAudio_2 );
    mShaderNlights.setUniform1f( "uAudio_3", mAudio_3 );
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    ofPopMatrix();
    mShaderNlights.end();
    
}

void LightModeNLights::colorResponseTweenOn(){
	
	if(Tweenzor::getTween(&mColor) == NULL){
		mColorTar = ofVec3f( ofRandom(0.6, 0.99), ofRandom(0.3, 0.99), ofRandom(0.3, .7) );
		Tweenzor::add(&mColor, mColor, mColorTar, 0.f, 0.50, EASE_IN_OUT_QUAD);
	}

}

void LightModeNLights::colorQuestionTweenOn(){
	
	mColorTar = ofVec3f( ofRandom(0.6, 0.99), ofRandom(0.3, 0.99), ofRandom(0.3, .7) );
	Tweenzor::add(&mColor, mColor, mColorTar, 0.f, 1.0, EASE_IN_OUT_QUAD);
}

void LightModeNLights::colorQuestionTweenOff(){
	Tweenzor::add(&mColor, mColor, mColorDefault, 0.f, 1.0, EASE_IN_OUT_QUAD);
}

void LightModeNLights::alphaTweenOn(){
	Tweenzor::add(&mAlphaMult, mAlphaMult, 1.0, 0.f, 3.0, EASE_IN_OUT_QUAD);
}

void LightModeNLights::alphaTweenOff(){
	Tweenzor::add(&mAlphaMult, mAlphaMult, 0.0, 0.f, 2.5, EASE_OUT_QUAD);
}

void LightModeNLights::audioTweenUp_1(){
	if(Tweenzor::getTween(&mAudio_1) == NULL){
		Tweenzor::add(&mAudio_1, mAudio_1, ofRandom(0, 0.15), 0.f, ofRandom(0.15, 0.35), EASE_IN_OUT_QUAD);
	}
}


void LightModeNLights::audioTweenUp_2(){
	if(Tweenzor::getTween(&mAudio_2) == NULL){
		Tweenzor::add(&mAudio_2, mAudio_2, ofRandom(0, 0.1), 0.0f, ofRandom(0.25, 0.5), EASE_IN_OUT_QUAD);
	}
}

void LightModeNLights::audioTweenUp_3(){
	if(Tweenzor::getTween(&mAudio_3) == NULL){
		Tweenzor::add(&mAudio_3, mAudio_3, ofRandom(0, 0.15), 0.0f, ofRandom(0.25, 0.5), EASE_IN_OUT_QUAD);
	}
}


void LightModeNLights::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeNLights::animateIn" << endl;
    
    mCurState = STATE_IN;
    //alphaTweenOn();
    mAlphaMult = 0.0;
    Tweenzor::add(&mAlphaMult, mAlphaMult, 1.0, 0.f, 2.0, EASE_IN_OUT_QUAD);
}

void LightModeNLights::animateOut(float duration)
{
	cout << "NLights::AnimateOut" << endl;
	//alphaTweenOff();
	//mAlphaMult = 0.0;
	Tweenzor::add(&mAlphaMult, mAlphaMult, 0.0, 0.f, 2.0, EASE_OUT_QUAD);
	//colorQuestionTweenOff();
    LightMode::animateOut(1);
}

void LightModeNLights::setQuestionState()
{
    cout << "LightMode::setQuestionState" << endl;
    Tweenzor::add(&mPulseMult, mPulseMult, 0.90, 0.f, mTweenzorDuration);
    colorQuestionTweenOn();
    //audioTweenDown();
}

void LightModeNLights::setResponseState()
{
    //DS: stop loading animation
    isWaitingForResponse = false;
    
    //Tweenzor::add(&mRespondAlpha, mRespondAlpha, 1.0, 0.f, mTweenzorDuration);
    if(mRespondAlpha > 0.90)
    {
        mRespond = true;
    }
    //audioTweenUp(); 
    //mAudio = 10.0;  
}

void LightModeNLights::setNormalState()
{
    //DS: go back to idle animation
    
    //DS: remove colors
    LightMode::removeColors();
    Tweenzor::add(&mPulseMult, mPulseMult, -0.4, 0.f, mTweenzorDuration);
    //Tweenzor::add(&mRespondAlpha, mRespondAlpha, 0.0, 0.f, mTweenzorDuration);
    mRespond = false;
    colorQuestionTweenOff();
    //mPulseMult = 0.25;
    
    //DS: stop sound reactiveness
    //mIsSoundReactive = false;
}
