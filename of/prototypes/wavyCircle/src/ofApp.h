#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTweenzor.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //ring functions
    void drawRing(vector<ofVec2f> outside, vector<ofVec2f> inside);
    
    void updateOffsets( vector<ofVec2f> offsets, int _scale);
    void resetOffsets(vector<ofVec2f> offsets);
    
    void plotCurves( vector<ofVec2f> points, vector<ofVec2f> offsets );
    
    void resizeRing(vector<ofVec2f> outer,
                    vector<ofVec2f> inner);
    void resizeRingRes(vector<ofVec2f> outer,
                    vector<ofVec2f> inner);
		
    
    //sound input
    void setupAudio();
    void updateAudio();
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    bool mListening;
    
    ofSoundStream soundStream;
    
    
    //circle params
    float mRad;
    float mThickness;
    bool mFilled;
    ofVec2f mPos;
    ofColor mColor;
    float mDiameter;
    float mPreviousDiameter;
    
    //array of all the points
    vector<ofVec2f> outerPoints;
    vector<ofVec2f> innerPoints;
    vector<ofVec2f> offsetPoints;
    
    //path based on the arrays
    ofPath mShape;
    
    int n; // the number of points in the circle
    int previousN;
    
    bool drawDebug;
    bool drawGui;
    float mMagnitude;
    
    //gui sliders
    ofxPanel mGui;
    
    ofxGuiGroup mButtons;
    ofxToggle mDebugButton;
    ofxToggle mListeningButton;
    
    ofxGuiGroup mRingParams;
    ofxIntSlider mRingResolution;
    ofxFloatSlider mRingSize;
    ofxFloatSlider mStrokeSlider;
    ofxFloatSlider mOffsetLimits;
    ofxFloatSlider mXenoSpeed;
    ofxFloatSlider mResetSpeed;
    
    ofxGuiGroup mSoundParams;
    ofxFloatSlider mMagnitudeSlider;
    ofxIntSlider mMagnitudeCap;
    ofxFloatSlider mMinThreshold;
    ofxFloatSlider mSoudMultiplier;
    ofxFloatSlider mSoundDampener;
    
    
};
