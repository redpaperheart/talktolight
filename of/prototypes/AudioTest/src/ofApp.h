/*
	Prototype to test audio reativity for the Google Assistant Project 
	- Shuvashis Das
*/

#pragma once

#include "ofMain.h"
#include "Light.h"
#include "Model.h"
#include "ofxBeat.h"

//#define M_PI 3.1416

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

		// Model
		Model   *mModel;

		ofSoundStream mSoundStream;
		string  mControlsString;
		Light   mLight;

		ofxBeat mBeat;

		void audioIn(float * input, int bufferSize, int nChannels);
		void setupAudio(unsigned int deviceId);

		// audio in
		int 	mInputConnected = false;
		int 	mBufferCounter;
		float   mSmoothedVol;
		float   mScaledVol;
		float   mCurVol;
		ofSoundDevice mCurDevice;

		vector <float> mLeft;
		vector <float> mRight;
		vector <float> mVolHistory;
		void drawDebug();
		void drawOfxBeatBars();
		void drawBeatBand();
		
};
