
#pragma once

#ifdef _WIN32 || _WIN64 
	#define M_PI 3.1416
#endif

#define W_WIDTH  1920
#define W_HEIGHT 1200


#include "ofMain.h"

#ifdef __linux__
#include "ofxBeat/src/ofxBeat.h"
#elif _WIN32 || _WIN64 || __APPLE__
#include "ofxBeat.h"
#endif

class Model {
public:
    static Model* getInstance();
    void setup();
    
    //loaded settings from settings.xml
    string assetPath = "";
    
    // State Management
    enum State {
        STATE_NONE,
        STATE_NORMAL,
        STATE_QUESTION,
        STATE_RESPONSE
    };
    
    enum Mode{
        MODE_NONE,
        MODE_IDLE,
        MODE_LIGHTS,
        MODE_DISCO,
        MODE_LASER,
        MODE_ROBOT,
		MODE_SCANNER,
        MODE_SPOTLIGHT,
        MODE_RAINBOW, 
        MODE_FIREFLY,
        MODE_AURORA,
        MODE_ALIEN,
        MODE_SPACESHIP,
        MODE_GOODNIGHT
    };
    
    Mode   mCurMode = MODE_IDLE;
    
    void setCurState(const State &stateId);
    State getCurState() const { return mCurState; }
    
    void setNextState(const State &stateId);
    State getNextState() const { return mNextState; }
    
    bool isAssistantActive(){
        return !(mCurState == STATE_NORMAL || mCurState == STATE_NONE);
    }
    
    bool isTransitioningStates();
    float volumeScaler = 1; //used for adjusting volume sensitivity
    
    int idleTimeout = 60000; //millis
    int idleTimer = 10000;
    void resetIdleTimer(){
        idleTimer = ofGetElapsedTimeMillis() + idleTimeout;
    }
    //OSC control
    string musicControlIP = "192.168.0.61";
    string menuControlIP = "192.168.0.60";
    
    
    // audio in
    ofSoundDevice mCurDevice;
    int 	mInputConnected = false;
    int 	mBufferCounter;
    float   mVolSmoothed;
    float   mVolScaled;
    float   mVolCur;
    float   mVolTop;
    
    // for ofxBeat
    ofxBeat mBeat;
	float	mFFTAvg;
	float	mBassAvg;
    
    vector <float> mLeft;
    vector <float> mRight;
    vector <float> mVolHistory;
    vector <float> mVolScaledHistory;

	//triggreing shaders for testing
	bool mTrigger = false;

    
    // debug
    float   mFps = 0;
    bool    bDebug = true;
    bool    bShowCursor = true;
    bool    bShowParams = true;
    bool    bDrawHelpers = true;
	bool	bShowGrid = false;
    
//    float   mModeChangeTime = 0;
    
    //float discoSensitivity = 0.0175;
    
private:
    // Singleton
    Model() {}; // Private so that it can  not be called
    Model(Model const&){}; // copy constructor is private
    Model& operator=(Model const&) { return *m_pInstance; } // assignment operator is private
    static Model* m_pInstance;
    
    State mCurState;
    State mNextState;
    
    
};
