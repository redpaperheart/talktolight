//
//  LightRect.h
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/20/18.
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

class LightRect{
	public:
		ofPlanePrimitive mPlane;
		float   mHeight = 80.0;
		float	mWidth = 320.0;
		float gap = 2.50;						// to keep distance between each box
		bool state;
		bool mStayOn = false;
		float alphaMult  = 0.0 ;
		float alphaMultTar  = 0.0 ;
		float mDuration = 1.0;
		float mDurationResponse = ofRandom( 0.1, 1.0 );
		ofColor color ;

		void alphaTweenOn(float delay){
			if(Tweenzor::getTween(&alphaMult) == NULL){
				Tweenzor::add(&alphaMult, 0, 1.0, delay, .3, EASE_IN_OUT_QUAD);
				Tweenzor::addCompleteListener(Tweenzor::getTween(&alphaMult), this, &LightRect::alphaTweenOff);
			}
		}

		void alphaTweenOff(float* arg){
			if(mStayOn) return;
			Tweenzor::add(&alphaMult, alphaMult, 0.0, 0.f, 1.0, EASE_IN_OUT_QUAD);
			//state = false;
		}

		void alphaTweenOnResponse(float delay){
			if(Tweenzor::getTween(&alphaMult) == NULL){
				Tweenzor::add(&alphaMult, 0.0, 1.0, delay, 0.05, EASE_IN_OUT_QUAD);
				Tweenzor::addCompleteListener(Tweenzor::getTween(&alphaMult), this, &LightRect::alphaTweenOffResponse);
			}
		}

		void alphaTweenOffResponse(float* arg){
			if(mStayOn) return;
			Tweenzor::add(&alphaMult, alphaMult, 0.0, 0.f, 0.5, EASE_IN_OUT_QUAD);
		}
	
		void alphaTweenQuestionOn(float duration, float delay){
			Tweenzor::add(&alphaMult, alphaMult, 1.0, delay, duration, EASE_IN_OUT_QUAD);
		}
		
		void alphaTweenAnimateIn(){
			Tweenzor::add(&alphaMult, alphaMult, 1.0, 0.f, mDuration, EASE_IN_OUT_QUAD);
		}
		

		void alphaTweenAnimateOut(){
			Tweenzor::add(&alphaMult, alphaMult, 0.0, 0.f, mDuration, EASE_IN_OUT_QUAD);
		}
};
