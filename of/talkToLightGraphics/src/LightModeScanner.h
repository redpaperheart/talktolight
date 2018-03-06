//
//  LightModeScanner.h
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/23/18.
//
//

#pragma once

#include "ofMain.h"
#include "LightMode.h"
#include "Model.h"

#ifdef __linux__ 
#include "ofxTweenzor/src/ofxTweenzor.h"
#elif _WIN32 || _WIN64 || __APPLE__
#include "ofxTweenzor.h"
#endif

class LightModeScanner : public LightMode{

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

	//for tweenzor animation
	void onComplete(float* arg);
	void animateInBar();
	void animateOutBar();
	void onResponse(float* arg);
	void onColorBarComplete(float* arg);
	void startColorBarGlitch();
	void stopColorBarGlitch();
	void onRespBarComplete(float *arg);
	
	void onNormal();
	void onQuestion();
//    void onAnimateOutComplete(float* arg) override;

protected:
	int mQuestionCount = 0;
	bool mIsSoundReactive = false; 

	int mNumBars = 2;

	ofShader mShaderScanner;

	ofImage mNoise1;
	ofImage mNoise2;

	//uniforms for shader
	float mBarWidth_1 ;
	float mBarWidth_2 ;
	float mBarWidthTar_1;
	float mBarWidthTar_2;
	float mRespBarWidth;
	float mRespBarWidthTar;
	float mBarDistance;
	float mRectPosX_1;
	float mRectPosXPre_1;
	float mRectPosXTar_1;
	float mRectPosX_2;
	float mRectPosXPre_2;
	float mRectPosXTar_2;
	float mRespBarPosX;
    float mRespBarPosXDelta;
    float mRespBarPosXPre;
	float mRespBarPosXTar;
	float mBarOffset;
	float mUVSpeed;
	bool   mMoveUV;
	bool   mDrawResp;
	float mTweenzorDuration;
	float mGlitchDurationMin=.1;
	float mGlitchDurationMax = .8;
	float mRectVel_1 = 0;
	float mRectVel_2 = 0;
	float mRespVel = 0;

	
};
