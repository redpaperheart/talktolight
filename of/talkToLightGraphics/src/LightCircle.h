//
//  Light.hpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 5/15/17.
//
//

#pragma once

#include "ofMain.h"


#ifdef __linux__ 
	#include "ofxTweenzor/src/ofxTweenzor.h"
#elif _WIN32 || _WIN64 || __APPLE__
	#include "ofxTweenzor.h"
#endif

#include "Model.h"
#include "GlowShapes.h"

class LightCircle {
  public:
	LightCircle(ofVec2f pos = ofVec2f(0), float rad = 0);
	~LightCircle();
	
	virtual void update(); 
	virtual void draw();
	
	virtual void animateOut(float duration = 1.0, float delay = 0);
	
	virtual void animateRadius(float radius, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
	virtual void animatePosition( ofVec2f pos, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
	virtual void animateColor(ofFloatColor color, float duration = 0.5, float delay = 0, int easing = EASE_IN_OUT_QUAD);
	
	virtual void onAnimationComplete(std::function<void()> callback);
	
	virtual void setFilled(bool filled)         { mFilled = filled; }
    virtual void setColor(ofFloatColor color)   { mColor = color; }
	virtual void setGlowColor(ofFloatColor col) { mGlowColor = col; }
	virtual bool isAlive()                      { return !mIsDead; }
	virtual bool isDying()                      { return mIsDying; }
	virtual bool isDead()                       { return mIsDead; }
	virtual bool isRadiusTweening()             { return Tweenzor::getTween(&mRadius) != NULL; }
	virtual bool isPositionTweening()           { return Tweenzor::getTween(&mPosition) != NULL; }
	
	virtual void setSoundReactiveness(bool val){ mIsSoundReactive = val; }
  
    float   mRadius = 0;
	float   mGlowRadius = 0;
	ofVec2f mPosition;
	ofVec2f mOffset;
    
    //overrides tweenzor animations and uses target+lerp in update() instead
    float mTargetRadius = 0;
    float mRadiusSpeed = .2;
    bool mTargetRadiusEnabled = false;
    void setTargetRadiusEnabled(bool _val){
        mTargetRadiusEnabled = _val;
        mTargetRadius = mRadius;
        //stop any tweens
        Tweenzor::removeTween(&mRadius);
    }
	
	// bad design, cat mode values should not be here:
	float   mCatNextUpdate = 0;
	ofVec2f mCatTarget;
	
	ofFloatColor getColor() { return this->mColor; };
	
  protected:
	virtual void onAnimateOutComplete(float* arg);
	//virtual void onAnimateInComplete(float* arg);
	virtual void onAnimateRadiusComplete(float* arg);
	
	static int  sIdCounter; // is that in use?
	
	bool        mIsDying = false;
	bool        mIsDead = false;
	
	bool        mFilled = false;
	int         mId = 0;
	float       mThickness = 10;
   
    ofFloatColor mColor = ofFloatColor::white;
	ofFloatColor mGlowColor = ofFloatColor::white;
	
	std::function<void()> mCallback = nullptr;
	
	Model       *mModel;
	
	bool  mIsSoundReactive = false;
};
