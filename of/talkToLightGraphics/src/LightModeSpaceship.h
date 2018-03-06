//
//  LightModeSpotlight
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/21/18.
//

#pragma once

#include "ofMain.h"
#include "LightRect.h"
#include "LightMode.h"
#include "GlowShapes.h"

class LightModeSpaceship : public LightMode
{
public:
    void setup( std::shared_ptr<LightCircle> assistantLightRef ) override;
    void update() override;
    void draw() override;
    
    //states
    void animateIn() override; // init, set circles sizes etc.
    void animateOut(float duration=1) override; // clean up, reset, e.g. remove additional circles, when switching modes.
    
    void setQuestionState() override;
    void setResponseState() override;
    void setNormalState() override;
    
	void reloadShaders();  //hitting spacebar reloads shaders, no need to recompile solution

	//functions for different states
	void onIdle();
	void onQuestion(int mQuestionBar);
	void onQuestionTween();
	void onResponse();

private:

	//std::vector<Box> mBoxes;
	std::vector<LightRect> mBoxes;
	const int mNumBox = 6*15;
	void setAlphaToZero(float* arg);
	bool mBorder;
	bool mQuestion = false;
	int mQuestionBar = 1;

	ofShader mBoxShader;

	bool alphaTrigger = false;
	bool mStateBegin = true;
	
	float mRectGlow = 0.50;
	float mPrevSeconds = 0.0;
	float intervalGap ;
	
	

};
