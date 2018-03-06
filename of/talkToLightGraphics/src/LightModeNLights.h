//
//  LightNLightsMode.h
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/20/18.
//
//

#pragma once

#include "ofMain.h"
#include "LightMode.h"

#ifdef __linux__ 
#include "ofxTweenzor/src/ofxTweenzor.h"
#elif _WIN32 || _WIN64 || __APPLE__
#include "ofxTweenzor.h"
#endif


class LightModeNLights : public LightMode{

public:
	void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
	void update() override;
    void draw() override;
	void reloadShaders() override;

	//states
	void animateIn() override; // init, set circles sizes etc.
	void animateOut(float duration=1) override; // clean up, reset, e.g. remove additional circles, when switching modes.

	void setQuestionState() override;
	void setResponseState() override;
	void setNormalState() override;
	
	void onResponse(float* arg);
	void audioTweenUp_1();
	void audioTweenDown_1( float* arg );
	void audioTweenUp_2();
	void audioTweenDown_2( float* arg );
	void audioTweenUp_3();
	void audioTweenDown_3( float* arg );

protected:
	int mQuestionCount = 0;
	bool mIsSoundReactive = false; 
	
	int mNumBars = 20;
	vector<int> mBarHeight;
	vector<ofColor> mBarColor;

	ofShader mShaderNlights;
	ofImage mNoise1;
	ofImage mNoise2;
	ofImage mNoise3;
	ofImage mNoise4;
	ofImage mNoise5;

	float xOffset;
	float xStart;
	float yStart;
	float mPulseMult;
	float mPulseMultTar;
	float mTweenzorDuration;
	float mRespondAlpha;
	float mRespondAlphaTar;
	float mAlphaMult = 0.0;
	float mAlphaMultTar = 1.0;
	float mAudio_1 = 0.0;
	float mAudio_2 = 0.0;
	float mAudio_3 = 0.0;
	float mAudioTar_1 = 0.0;
	float mAudioTar_2 = 0.0;
	float mAudioTar_3 = 0.0;
	float mSin1=0;
	
	bool   mRespond;	
	
	ofVec3f mColor;
	ofVec3f mColorTar;
	ofVec3f mColorDefault;
	
	void alphaTweenOn();
	void alphaTweenOff();
	void colorQuestionTweenOn();
	void colorQuestionTweenOff();
	void colorResponseTweenOn();

};
