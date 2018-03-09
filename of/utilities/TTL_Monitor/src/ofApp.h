#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define OSC_PORT              12345
#define OSC_QUESTION_STARTED  "/assistant/question/start"
#define OSC_RESPONSE_STARTED  "/assistant/response/start"
#define OSC_RESPONSE_ENDED    "/assistant/response/end"

#define OSC_MODE_LIGHT        "/mode/light"
#define OSC_MODE_DISCO        "/mode/disco"
#define OSC_MODE_CAT          "/mode/cat"

#define OSC_MODE_SPOTLIGHT      "/mode/spotlight"
#define OSC_MODE_RAINBOW        "/mode/rainbow"
#define OSC_MODE_AURORA         "/mode/aurora"
#define OSC_MODE_SPACESHIP      "/mode/spaceship"
#define OSC_MODE_ALIEN          "/mode/alien"
#define OSC_MODE_SCANNER        "/mode/scanner"
#define OSC_MODE_FIREFLY        "/mode/firefly"
#define OSC_MODE_CAR            "/mode/kitt"
#define OSC_MODE_GOODNIGHT      "/mode/goodnight"

#define OSC_STATUS_PORT         12346
#define OSC_STATUS_HEART        "/status/heart"
#define OSC_DEVICE_CHANGE       "/device/change"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        string currentMode;
        string currentState;
        int lastHeartBeat; // in ms
        int hearBeatTimeout= 3000; // in ms
        float avgVolume;
        string deviceName;
        void onHeartBeat();
        void parseOsc();
        ofxOscReceiver receiver;
    
    

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
		
};
