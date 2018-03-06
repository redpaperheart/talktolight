//
//  LightModeCat.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/23/17.
//
//

#include "LightModeDisco.h"
#include "Model.h"


void LightModeDisco::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    //Tweenzor::init();
    //mColorPalette = { ofFloatColor::red, ofFloatColor::blue, ofFloatColor::yellow };
    LightMode::setup( assistantLightRef );
    mRadiusOrig = 35;
}

void LightModeDisco::update()
{

    LightMode::update();

	if(Model::getInstance()->mFFTAvg > 0.15){

		if (Model::getInstance()->mKick > 0.5 ) {
			mScale = 2.0; 
			addLight();
		}
		if (Model::getInstance()->mSnare > 0.5 ) {
			mScale = 1.0;
			addLight();
		}
		if (Model::getInstance()->mHihat > 0.5 ) {
			mScale = 1.0;
			addLight();
		}

	}
	
	
    
	/*   
	if (mAudioLevel - mPrevAudioLevel > ( Model::getInstance()->mThresholdVol ) ) {
        addLight();
    }
	*/
    
}

void LightModeDisco::draw(){
    
    ofPushMatrix();
    
    float rotation = 2 * ofGetElapsedTimef();
    ofTranslate(ofGetWindowSize() * 0.5);
    ofRotate(rotation);
    ofTranslate(-ofGetWindowSize() * 0.5);
    
    for (auto circle : mCircles) {
        circle->draw();
    }

    ofPopMatrix();
}

void LightModeDisco::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeDisco::animateIn" << endl;
    //LightMode::animateIn();
    removeColors();
    mAssistantLightRef->animateRadius(0);
}

void LightModeDisco::animateOut()
{
    // reset, clean up anything when switching of this mode.
    cout << "LightModeDisco::animateOut" << endl;
    LightMode::animateOut();
}

void LightModeDisco::setQuestionState()
{
    cout << "LightModeDisco::setQuestionState" << endl;
    //float rad = (mAssistantLightRef->mRadius < mRadiusOrig*mScale)? mRadiusOrig*mScale : mAssistantLightRef->mRadius;
    float rad = mRadiusOrig * mScale;
    
    mAssistantLightRef->onAnimationComplete( [this, rad](){
        mAssistantLightRef->animateRadius(rad, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
    } );
    mAssistantLightRef->animateRadius(rad * 1.2, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}

void LightModeDisco::setResponseState()
{
    mAssistantLightRef->setSoundReactiveness(true);
}

void LightModeDisco::setNormalState()
{
    cout << "LightModeDisco::setNormalState" << endl;
    mAssistantLightRef->setSoundReactiveness(false);
    mAssistantLightRef->animateRadius(0, 0.5f, 0.0f, EASE_IN_OUT_QUAD);
}


void LightModeDisco::addLight()
{
    //float angle = ofRandom(2 * M_PI);
	float angle = ofRandom(2 * 3.1416);
    float dist = 0.5 * ofRandom(min(ofGetWindowWidth(), ofGetWindowHeight()));
    ofVec2f pos = ofGetWindowSize() * 0.5 + dist * ofVec2f(cos(angle), sin(angle));
    auto c = std::make_shared<LightCircle>(pos);

    if (mUseColors) {
        c->setColor(mColorPalette.at((int)ofRandom(mColorPalette.size())));
    }
    c->setFilled(ofRandom(1.0f) > 0.25f);
    c->animateRadius(ofRandom(mRadiusOrig*0.5, mRadiusOrig) * mScale, 0.25);
    c->onAnimationComplete([c]{
        c->animateOut(1.5);
    });
    mCircles.push_back(c);
}

void LightModeDisco::removeLight()
{
    
}

