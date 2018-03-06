
#pragma once

#include "ofMain.h"

class Model {
public:
    static Model* getInstance();
    void setup();
    
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
        MODE_CAT
    };
    
    
//    static std::string stateToString(const State &key)
//    {
//        std::map<State, std::string> enumToString = {
//            { STATE_NONE, "State: None" },
//            { STATE_TITLE, "State 1: Title" },
//            { STATE_GAME, "State 2: Game" }
//        };
//        return enumToString[key];
//    }
    
    Mode   mCurMode = MODE_IDLE;
    
//    signals::Signal<void()> signal_curStateChanged;
    void setCurState(const State &stateId);
    State getCurState() const { return mCurState; }
    
//    signals::Signal<void()> signal_nextStateChanged;
    void setNextState(const State &stateId);
    State getNextState() const { return mNextState; }
    
    bool isAssistantActive(){
        return !(mCurState == STATE_NORMAL || mCurState == STATE_NONE);
    }
    
//    void toggleDebug(){ bDebug = !bDebug; }
    bool isTransitioningStates();
    
    
    // audio in
    int 	mInputConnected = false;
    int 	mBufferCounter;
    float   mSmoothedVol;
    float   mScaledVol;
    float   mCurVol;
	float	mThresholdVol;
    ofSoundDevice mCurDevice;

	// for ofxBeat
	float   mKick;
	float	mSnare;
	float	mHihat;
	float	mFFTAvg;
	float	mBassAvg;
    
    vector <float> mLeft;
    vector <float> mRight;
    vector <float> mVolHistory;

    
    
    float   mFps = 0;
    bool    bDebug = true;
    bool    bShowCursor = true;
    bool    bShowParams = true;
    bool    bDrawHelpers = true;
    
    float   mModeChangeTime = 0;
    
private:
    // Singleton
    Model() {}; // Private so that it can  not be called
    Model(Model const&){}; // copy constructor is private
    Model& operator=(Model const&) { return *m_pInstance; } // assignment operator is private
    static Model* m_pInstance;
    
    State mCurState;
    State mNextState;  
};
