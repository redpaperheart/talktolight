//
//  LightNLightsMode.cpp
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/20/18.
//
//

#include "Model.h"
#include "LightModeScanner.h"

//--------------------------------------------------------------
void LightModeScanner::setup( std::shared_ptr<LightCircle> assistantLightRef){

	// DS: pass the assistantLightRef to the super's setup function
	LightMode::setup( assistantLightRef );

	mBarWidth_1 = 20.0f / ofGetWidth();
	mBarWidth_2 = mBarWidth_1;
	mBarWidthTar_1 = mBarWidth_1;
	mBarWidthTar_2 = mBarWidth_2;
	mRespBarWidth = 100.0f / ofGetWidth();
	mBarDistance = 50.0f / ofGetWidth();
	mRectPosX_1 = 0.0f;	
	mRectPosX_2 = mRectPosX_1 + mBarDistance;
	mRectPosXTar_1 = mRectPosX_1;
	mRectPosXTar_2 = mRectPosX_2;
	mRespBarPosX = -(2.0*ofGetWidth() ) / ofGetWidth(); // setting to -2 to push it outside the left side
	mRespBarPosXTar = mRespBarPosX;
	mBarOffset = 100.0 / ofGetWidth();
	mTweenzorDuration = .3;
	mMoveUV =  true;
	mDrawResp = true;
	
    reloadShaders();
    
    setNormalState(); // important to do this or white bar doesn't appear
}

void LightModeScanner::reloadShaders()
{
#ifdef TARGET_OPENGLES
    mShaderScanner.load(mModel->assetPath + "/scanner/shaders_gles/scanner.vert",mModel->assetPath + "/scanner/shaders_gles/scanner.frag");
#else
    mShaderScanner.load(mModel->assetPath + "/scanner/shaders/scanner.vert", mModel->assetPath + "/scanner/shaders/scanner.frag");
#endif
 
}

// this function is called on when the tween is complete //
/*
void LightModeScanner::onComplete(float* arg) {
	
	Tweenzor::add(&mRectPosX_1, mRectPosX_1, mRectPosXTar_1, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	Tweenzor::getTween( &mRectPosX_1 )->setRepeat( 1, true );
	Tweenzor::addCompleteListener( Tweenzor::getTween(&mRectPosX_1), this, &LightModeScanner::onComplete);
	Tweenzor::add(&mRectPosX_2, mRectPosX_2, mRectPosXTar_2, 0.0f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	Tweenzor::getTween( &mRectPosX_2 )->setRepeat( 1, true );
	Tweenzor::addCompleteListener( Tweenzor::getTween(&mRectPosX_2), this, &LightModeScanner::onComplete);

	Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.f, mTweenzorDuration, EASE_IN_OUT_BOUNCE);
	Tweenzor::getTween( &mRespBarPosX )->setRepeat( 1, true );
	Tweenzor::addCompleteListener( Tweenzor::getTween(&mRespBarPosX), this, &LightModeScanner::onComplete);

	Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	//Tweenzor::getTween( &mBarWidth_1 )->setRepeat( 1, true );
	//Tweenzor::addCompleteListener( Tweenzor::getTween(&mBarWidth_1), this, &LightModeScanner::onComplete);

	Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	//Tweenzor::getTween( &mBarWidth_2 )->setRepeat( 1, true );
	//Tweenzor::addCompleteListener( Tweenzor::getTween(&mBarWidth_2), this, &LightModeScanner::onComplete);
}
*/

void LightModeScanner::onQuestion(){
	
	mRectPosXTar_1 = mBarOffset;
	mRectPosXTar_2 = -mBarOffset;
	
	//mRespBarPosX =  ofRandom(-2,2) * ( mRectPosX_2 + ((mRectPosX_2 - mRectPosX_1)/2) ) ;//to put the response bar around the other two bars
	mRespBarPosXTar =  mRectPosXTar_2 + ((mRectPosXTar_1 + mRectPosXTar_2)/2) + mRespBarWidth/2 + mBarWidth_1/2 ;//to put the response bar in the middle of the other two bars
		
	//mBarWidthTar_1 = 20.0/ofGetWidth();
	//mBarWidthTar_2 = 20.0/ofGetWidth();
	
	Tweenzor::add(&mRectPosX_1, mRectPosX_1, mRectPosXTar_1, 0.f, 1.0, EASE_OUT_EXPO);
	Tweenzor::add(&mRectPosX_2, mRectPosX_2, mRectPosXTar_2, 0.0f,1.0, EASE_OUT_EXPO);
	//Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.f, mTweenzorDuration, EASE_IN_OUT_BOUNCE);
	
}
void LightModeScanner::onNormal(){
	mRectPosXTar_1 = 0.0;
	mRectPosXTar_2 = mRectPosXTar_1 + mBarDistance;
	
	mRespBarPosX = -(2.0*ofGetWidth() ) / ofGetWidth(); // setting to -2 to push it outside the left side;
	mRespBarPosXTar = mRespBarPosX;
	
	mBarWidthTar_1 = 20.0/ofGetWidth();
	mBarWidthTar_2 = 20.0/ofGetWidth();
	mRespBarWidthTar = 20.0/ofGetWidth();
	
	Tweenzor::add(&mRectPosX_1, mRectPosX_1, mRectPosXTar_1, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	Tweenzor::add(&mRectPosX_2, mRectPosX_2, mRectPosXTar_2, 0.0f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	
	Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0.f, 2.0, EASE_IN_OUT_QUAD);
	Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0.f, 2.0, EASE_IN_OUT_QUAD);
	Tweenzor::add(&mRespBarWidth, mRespBarWidth, mRespBarWidthTar, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	
	Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.f, mTweenzorDuration, EASE_IN_OUT_BOUNCE);
	
}

void LightModeScanner::onColorBarComplete(float* arg){
	
	mRectPosXPre_1 = mRectPosX_1;
	mRectPosXPre_2 = mRectPosX_2;
	
	float dur = ofRandom(mGlitchDurationMin, mGlitchDurationMax);
	if(arg == &mRectPosX_1){
		mRectPosXTar_1 = ofRandom(-2,2)*mBarOffset;
		//mBarWidthTar_1 = ( ofRandom(5, 25) + 20.0 )/ofGetWidth();
		
		Tweenzor::add(&mRectPosX_1, mRectPosX_1, mRectPosXTar_1, 0.f, dur, EASE_IN_OUT_ELASTIC);
		//Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0.f, dur, EASE_IN_OUT_ELASTIC);
		Tweenzor::addCompleteListener( Tweenzor::getTween(&mRectPosX_1), this, &LightModeScanner::onColorBarComplete);
	}else if(arg == &mRectPosX_2){
		mRectPosXTar_2 = ofRandom(2,-2)*mBarOffset;
		//mBarWidthTar_2 = ( ofRandom(5, 25) + 20.0 )/ofGetWidth();
		Tweenzor::add(&mRectPosX_2, mRectPosX_2, mRectPosXTar_2, 0.0f, dur, EASE_IN_OUT_ELASTIC);
		//Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0.f, dur, EASE_IN_OUT_ELASTIC);
		Tweenzor::addCompleteListener( Tweenzor::getTween(&mRectPosX_2), this, &LightModeScanner::onColorBarComplete);
	}else{
		//nothing
	}
}
void LightModeScanner::startColorBarGlitch(){
		
		
		mRectPosXPre_1 = mRectPosX_1;
		mRectPosXPre_2 = mRectPosX_2;
			
		float dur = ofRandom(mGlitchDurationMin, mGlitchDurationMax);
		mRectPosXTar_1 = ofRandom(-2,2)*mBarOffset;
		//mBarWidthTar_1 = ( ofRandom(5, 25) + 20.0 )/ofGetWidth();
		Tweenzor::add(&mRectPosX_1, mRectPosX_1, mRectPosXTar_1, 0.f, dur, EASE_IN_OUT_ELASTIC);
		//Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0.f, dur, EASE_IN_OUT_ELASTIC);
		
		Tweenzor::addCompleteListener( Tweenzor::getTween(&mRectPosX_1), this, &LightModeScanner::onColorBarComplete);
		
		dur = ofRandom(mGlitchDurationMin, mGlitchDurationMax);
		
		mRectPosXTar_2 = ofRandom(2,-2)*mBarOffset;
		//mBarWidthTar_2 = ( ofRandom(5, 25) + 20.0 )/ofGetWidth();
		Tweenzor::add(&mRectPosX_2, mRectPosX_2, mRectPosXTar_2, 0.0f, dur, EASE_IN_OUT_ELASTIC);
		//Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0.f, dur, EASE_IN_OUT_ELASTIC);
		Tweenzor::addCompleteListener( Tweenzor::getTween(&mRectPosX_2), this, &LightModeScanner::onColorBarComplete);

}
void LightModeScanner::stopColorBarGlitch(){
	Tweenzor::removeCompleteListener( Tweenzor::getTween(&mRectPosX_1));
	Tweenzor::removeCompleteListener( Tweenzor::getTween(&mRectPosX_2));
}
void LightModeScanner::onRespBarComplete(float *arg){
	//mRespBarPosX =  ofRandom(-2,2) * ( mRectPosX_2 + ((mRectPosX_2 - mRectPosX_1)/2) ) ;//to put the response bar around the other two bars
	mRespBarPosXTar =  mRectPosX_2 + ((mRectPosX_2 - mRectPosX_1)/2)  ;//to put the response bar in the middle of the other two bars
	Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.0f, mTweenzorDuration, EASE_IN_OUT_BOUNCE);
}
	
void LightModeScanner::onResponse(float* arg) {
		
	mRectPosXTar_1 = ofRandom(-2,2)*mBarOffset;
	mRectPosXTar_2 = ofRandom(2,-2)*mBarOffset;
	
	float randTime = ofRandom(1,100);
	
	mRespBarPosX =  ofRandom(-2,2) * ( mRectPosX_2 + ((mRectPosX_2 - mRectPosX_1)/2) ) ;//to put the response bar around the other two bars
	mRespBarPosXTar =  mRectPosX_2 + ((mRectPosX_2 - mRectPosX_1)/2)  ;//to put the response bar in the middle of the other two bars
	mBarWidthTar_1 = ( ofRandom(5, 25) + 20.0 )/ofGetWidth();
	mBarWidthTar_2 = ( ofRandom(5, 25) + 20.0 )/ofGetWidth();
	
	Tweenzor::add(&mRectPosX_1, mRectPosX_1, mRectPosXTar_1, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	Tweenzor::add(&mRectPosX_2, mRectPosX_2, mRectPosXTar_2, 0.0f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	
	Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0.f, mTweenzorDuration, EASE_IN_OUT_ELASTIC);
	
	Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.0f, mTweenzorDuration, EASE_IN_OUT_BOUNCE);

}

//--------------------------------------------------------------
void LightModeScanner::update(){
	//DS: only update as long as this mode is still active
	if(mCurState == STATE_OUT) return;

	//if(mModel->getInstance()->mTrigger)
	//{
		//onGlitch(0);
	//}
	if(mCurState == STATE_OUT_START) return;
	
	switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
            //nothing
            break;
        case Model::STATE_QUESTION:
            //nothing
            break;
        case Model::STATE_RESPONSE:
        
			// adding barwidth so that they expand while positions are tweening
			mBarWidth_1 = mBarWidthTar_1 *  (1+mRectVel_1);

			if(Tweenzor::getTween(&mRectPosX_1) != NULL)
			{
				mRectVel_1 += 8 * std::abs(mRectPosX_1 - mRectPosXPre_1 );
				mRectVel_1 = ofClamp(mRectVel_1, 0, 10);
			}
			mRectVel_1 *= 0.8;
			
			
			mBarWidth_2 = mBarWidthTar_2 *  (1+mRectVel_2);

			if(Tweenzor::getTween(&mRectPosX_2) != NULL)
			{
				mRectVel_2 += 8 * std::abs(mRectPosX_2 - mRectPosXPre_2 );
				mRectVel_2 = ofClamp(mRectVel_2, 0, 10);
			}
			mRectVel_2 *= 0.8;
			

            mRespBarPosXDelta = mRespBarPosX - mRespBarPosXPre;
            mRespBarPosXPre = mRespBarPosX;
            
//			if(Tweenzor::getTween(&mRespBarPosX) != NULL)
//			{
//				mRespVel += 8 * std::abs(mRespBarPosXDelta);
//				mRespVel = ofClamp(mRespVel, 0, 10);
//			}
//			mRespVel *= 0.8;
//          mRespBarWidth = mRespBarWidthTar * (1 + mRespVel);
            
            // smooth width a bit
            float newVel = mRespBarWidthTar * (1 + std::abs(mRespBarPosXDelta) * 200.0f);
            newVel = ofClamp(newVel, 0.0f, 2.0f);
            mRespVel = ofLerp(mRespVel, newVel, 0.15f);
            mRespBarWidth = mRespVel;
            
			//showing the white response bar only when snare is above threshold
            if(Tweenzor::getTween(&mRespBarPosX) == NULL){
				if( mModel->mBeat.snare() > 0.5)
				{
					mRespBarPosXTar =  ofRandom(-2,2) * ( mRectPosX_2 + ((mRectPosX_2 - mRectPosX_1)/2) );
                    Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.0f, mTweenzorDuration * 3.0f, EASE_IN_OUT_BOUNCE);
                }
                else if (mRespBarPosXTar > 0.0) {
                    // only tween to offscreen if we're on screen
					mRespBarPosXTar = -(1.0 * ofGetWidth() ) / ofGetWidth();
                    Tweenzor::add(&mRespBarPosX, mRespBarPosX, mRespBarPosXTar, 0.0f, mTweenzorDuration * 1.0f, EASE_IN_OUT_BOUNCE);
				}
				
			}
            break;
    }
    
}

//--------------------------------------------------------------

void LightModeScanner::draw(){
	if(mCurState == STATE_OUT) return;

    ofFill();
    ofPushMatrix();
    ofPushStyle();
    
    mShaderScanner.begin();
    mShaderScanner.setUniform1f("uTime", ofGetElapsedTimef() );
    mShaderScanner.setUniform2f("uRes", ofVec2f(ofGetWidth(), ofGetHeight()));
    mShaderScanner.setUniform1f("uBarWidth_1", mBarWidth_1);
    mShaderScanner.setUniform1f("uBarWidth_2", mBarWidth_2);
    mShaderScanner.setUniform1i("uTrigger", mModel->getInstance()->mTrigger);
    mShaderScanner.setUniform1i("uDrawResp", mDrawResp);
    mShaderScanner.setUniform1i("uMoveUV", mMoveUV);
    mShaderScanner.setUniform1f("uRespBarWidth", mRespBarWidth);
    mShaderScanner.setUniform1f("uBarDistance", mBarDistance);
    mShaderScanner.setUniform1f("uRectPosX_1", mRectPosX_1);
    mShaderScanner.setUniform1f("uRectPosX_2", mRectPosX_2);
    mShaderScanner.setUniform1f("uRespBarPosX", mRespBarPosX);
    mShaderScanner.setUniform1f("uRespBarPosXDelta", mRespBarPosXDelta);
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    ofPopMatrix();
    
    mShaderScanner.end();
}

void LightModeScanner::animateInBar(){
	
	mBarWidth_1 = 0.0;
	mBarWidth_2 = 0.0;
	mBarWidthTar_1 = 20.0/ofGetWidth();
	mBarWidthTar_2 = 20.0/ofGetWidth();
	
	Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0, 1.0, EASE_IN_OUT_QUAD);
	Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0, 1.0, EASE_IN_OUT_QUAD);
	
	//cout << mBarWidth_1 << " " << mBarWidth_2 << endl;
}

void LightModeScanner::animateIn()
{
	// initialize anything mode specific here.
	mCurState = STATE_IN;
	mBarWidth_1 = 0.0;
	mBarWidth_2 = 0.0;
}

void LightModeScanner::animateOutBar(){
		
	mBarWidth_1 = 20.0/ofGetWidth();
	mBarWidth_2 = 20.0/ofGetWidth();
	mBarWidthTar_1 = 0.0;
	mBarWidthTar_2 = 0.0;
	
	Tweenzor::add(&mBarWidth_1, mBarWidth_1, mBarWidthTar_1, 0, 1.0, EASE_IN_OUT_QUAD);
	Tweenzor::add(&mBarWidth_2, mBarWidth_2, mBarWidthTar_2, 0, 1.0, EASE_IN_OUT_QUAD);
}


void LightModeScanner::animateOut(float duration)
{
	animateOutBar();
    LightMode::animateOut(duration);
	cout << "Scanner::AnimateOut" << endl;
}

void LightModeScanner::setQuestionState()
{
	cout << "LightMode::setQuestionState" << endl;
	mDrawResp = true;
	onQuestion();
}

void LightModeScanner::setResponseState()
{
	isWaitingForResponse = false;
	startColorBarGlitch();
}

void LightModeScanner::setNormalState()
{
	stopColorBarGlitch();
	onNormal();
	//DS: go back to idle animation

	//DS: remove colors 
	//LightMode::removeColors();

	//DS: stop sound reactiveness
	//mIsSoundReactive = false;

}
