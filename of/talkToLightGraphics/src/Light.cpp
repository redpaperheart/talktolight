//
//  Light.cpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 5/15/17.
//
//

#include "Light.h"

void Light::setup()
{
    Tweenzor::init();
    
    mModel = Model::getInstance();
    
    mAssistantLightRef = std::make_shared<LightCircleAssistant>( ofGetWindowSize()*0.5f, 0 );
    
    // create idle/default mode
    mModeIdle = std::make_shared<LightMode>();
    mModeIdle->setup( mAssistantLightRef );
    mModes.push_back(mModeIdle);
    
    //laser mode
    mModeLaser = std::make_shared<LightModeLaser>();
    mModeLaser->setup( mAssistantLightRef );
    mModes.push_back(mModeLaser);
    
    // create disco mode
    mModeDisco = std::make_shared<LightModeDisco>();
    mModeDisco->setup( mAssistantLightRef );
    mModes.push_back(mModeDisco);
    
    // create light mode
    mModeLight = std::make_shared<LightModeLight>();
    mModeLight->setup( mAssistantLightRef );
    mModes.push_back(mModeLight);

	// create NorthernLights mode
	mModeNLights = std::make_shared<LightModeNLights>();
    mModeNLights->setup( mAssistantLightRef );
	mModes.push_back(mModeNLights);
    
    // create Robot mode
    mModeRobot = std::make_shared<LightModeRobot>();
    mModeRobot->setup( mAssistantLightRef );
    mModes.push_back(mModeRobot);

	// create Scanner mode
	mModeScanner = std::make_shared<LightModeScanner>();
	mModeScanner->setup( mAssistantLightRef );
	mModes.push_back(mModeScanner);
    
    // create Spotlight mode
    mModeSpotLight = std::make_shared<LightModeSpotlight>();
    mModeSpotLight->setup( mAssistantLightRef );
    mModes.push_back(mModeSpotLight);
    
    // create ET mode
    mModeET = std::make_shared<LightModeEt>();
    mModeET->setup( mAssistantLightRef );
    mModes.push_back(mModeET);
    
    // create Spaceship mode
    mModeSpaceship = std::make_shared<LightModeSpaceship>();
    mModeSpaceship->setup( mAssistantLightRef );
    mModes.push_back(mModeSpaceship);
    
    // create firefly mode
    mModeFireFly = std::make_shared<LightModeFirefly>();
    mModeFireFly->setup( mAssistantLightRef );
    mModes.push_back(mModeFireFly);
    
    // create Rainbow mode
    mModeRainbow = std::make_shared<LightModeRainbow>();
    mModeRainbow->setup( mAssistantLightRef );
    mModes.push_back(mModeRainbow);
    
    // create Good Night mode
    mModeGoodNight = std::make_shared<LightModeGoodNight>();
    mModeGoodNight->setup( mAssistantLightRef );
    mModes.push_back(mModeGoodNight);
    
    //start on a random mode
    if(startMode==Model::MODE_NONE){
        setMode(Model::MODE_SPOTLIGHT);
    }else{
        setMode(startMode);
    }
}


void Light::update()
{
    Tweenzor::update(ofGetElapsedTimeMillis());
    
    mAssistantLightRef->update();
   
    //DS: replace with loop:
    for( auto mode : mModes){
        mode->update();
    }
}

void Light::draw()
{
    ofPushMatrix();
    
    // assistant light reference gets drawn by the light classe, not mode classes
    mAssistantLightRef->draw();
    
    for( auto mode : mModes){
        mode->draw();
    }
    
    ofPopMatrix();
}

void Light::changeState(Model::State state){
    if(!mModeCurrent) return;
    mModel->resetIdleTimer();
    switch(state){
        case Model::STATE_NONE:
            break;
        case Model::STATE_NORMAL:
            mModeCurrent->setNormalState();
            break;
        case Model::STATE_QUESTION:
            mModeCurrent->setQuestionState();
            break;
        case Model::STATE_RESPONSE:
            mModeCurrent->setResponseState();
            break;
    }
    
    mModel->setCurState(state);
}
//void Light::changeToRandomMode(){
//    int r = ofRandom(0, 10);
//    switch(r){
//        case 0:
//            setMode(Model::MODE_LASER);
//            break;
//        case 1:
//            setMode(Model::MODE_DISCO);
//            break;
//        case 2:
//            setMode(Model::MODE_AURORA);
//            break;
//        case 3:
//            setMode(Model::MODE_ROBOT);
//            break;
//        case 4:
//            setMode(Model::MODE_SCANNER);
//            break;
//        case 5:
//            setMode(Model::MODE_SPOTLIGHT);
//            break;
//        case 6:
//            setMode(Model::MODE_RAINBOW);
//            break;
//        case 7:
//            setMode(Model::MODE_ALIEN);
//            break;
//        case 8:
//            setMode(Model::MODE_FIREFLY);
//            break;
//        case 9:
//            setMode(Model::MODE_SPACESHIP);
//            break;
//        default:
//            setMode(Model::MODE_RAINBOW);
//            break;
//    }
//}
void Light::setMode(Model::Mode mode)
{
    if (mode == mModel->mCurMode) return;
    if(mModeCurrent != NULL){
        mModeCurrent->animateOut();
    }
    mModel->resetIdleTimer();
    switch(mode){
        case Model::MODE_NONE:
            break;
        case Model::MODE_IDLE:
            mModeCurrent = mModeIdle;
            break;
        case Model::MODE_LASER:
            mModeCurrent = mModeLaser;
            break;
        case Model::MODE_DISCO:
            mModeCurrent = mModeDisco;
            break;
        case Model::MODE_LIGHTS:
            mModeCurrent = mModeLight;
            break;
		case Model::MODE_AURORA:
			mModeCurrent = mModeNLights;
			break;
        case Model::MODE_ROBOT:
            mModeCurrent = mModeRobot;
            break;
		case Model::MODE_SCANNER:
			mModeCurrent = mModeScanner;
			break;
        case Model::MODE_SPOTLIGHT:
            mModeCurrent = mModeSpotLight;
            break;
        case Model::MODE_RAINBOW:
            mModeCurrent = mModeRainbow;
            break;
        case Model::MODE_ALIEN:
            mModeCurrent = mModeET;
            break;
        case Model::MODE_FIREFLY:
            mModeCurrent = mModeFireFly;
            break;
        case Model::MODE_SPACESHIP:
            mModeCurrent = mModeSpaceship;
            break;
    }
//    mModeCurrent->animateIn();
    mModeCurrent->animateInWithDelay(1);
//    mModeCurrent->onDelayComplete(0);
//    changeState(Model::STATE_NORMAL);
    mModel->mCurMode = mode;
    
}
void Light::resize(int w, int h)
{
    for (auto mode : mModes) {
        if (mode->getState() != LightMode::STATE_OUT) {
            mode->resize(w, h);
        }
    }
}

void Light::reloadShaders()
{
    for (auto mode : mModes) {
        if (mode->getState() != LightMode::STATE_OUT) {
            cout << "Reloading shaders for mode " << endl;
            mode->reloadShaders();
        }
    }
}
