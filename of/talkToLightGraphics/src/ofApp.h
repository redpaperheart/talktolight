#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "Light.h"
#include "Model.h"


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

#define OSC_STATUS_HEART        "/status/heart"


class ofApp : public ofBaseApp{
    
  public:
    
    void setup();
    void loadSettings();
    ofxXmlSettings settings;
    
    void setupOsc();
    void setupAudio(unsigned int deviceId);
    int currentDeviceId=-1;
    void update();
    
    void draw();
    void drawDebug();
	void drawOfxBeatBars();
	void drawOfxBeatBand();

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
    void exit();
    
    void parseOsc();
    void gotMessage(ofMessage msg);
    
    void audioIn(float * input, int bufferSize, int nChannels);
    
    ofSoundStream mSoundStream;
    int audioCheckCounter;
    
    ofxOscReceiver receiver;
    ofxOscSender menuSender;
    ofxOscSender musicSender;
    ofxOscSender statusSender;
    
    void sendStatus(string msg);
    
    void sendMessage(string msg);
    void setMode(string osc);
    void changeToRandomMode();
    
    Model   *mModel;
    
    bool    mDebug = true;
    void setDebug(bool val=false);
    bool mAppMin = false;
    void setAppMinimized(bool val=false);
    string  mControlsString;
    Light   mLight;

};
