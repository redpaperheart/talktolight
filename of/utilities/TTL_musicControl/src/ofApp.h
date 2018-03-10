#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "MenuView.h"
#include "ofxTweenzor.h"
//#include "ofxAMPMClient.h"

#define W_WIDTH  1920
#define W_HEIGHT 1080
#define OSC_PORT 12345

//Music
#define OSC_MUSIC_PLAY  "/music/play"
#define OSC_MUSIC_STOP  "/music/stop"
#define OSC_MUSIC_CHANGE  "/music/change"

//OSC States
#define OSC_QUESTION_STARTED  "/assistant/question/start"
#define OSC_RESPONSE_STARTED  "/assistant/response/start"
#define OSC_RESPONSE_ENDED    "/assistant/response/end"

//OSC Modes
#define OSC_MODE_LIGHT          "/mode/light"
#define OSC_MODE_DISCO          "/mode/disco"
#define OSC_MODE_CAT            "/mode/cat"
#define OSC_MODE_SPOTLIGHT      "/mode/spotlight"
#define OSC_MODE_ROBOT          "/mode/robot"
#define OSC_MODE_RAINBOW        "/mode/rainbow"
#define OSC_MODE_AURORA         "/mode/aurora"
#define OSC_MODE_SPACESHIP      "/mode/spaceship"
#define OSC_MODE_ALIEN          "/mode/alien"
#define OSC_MODE_SCANNER        "/mode/scanner"
#define OSC_MODE_FIREFLY        "/mode/firefly"
#define OSC_MODE_GOODNIGHT      "/mode/goodnight"

class ofApp : public ofBaseApp{

    public:
    void setup();
    void update();
    void draw();
    void loadSettings();
    ofxXmlSettings settings;
    ofxXmlSettings menuText;
    
    ofSoundPlayer sfx1;
    
    void setupViews();

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
    
    bool debug = false;
    string assetPath = "";
    string font = "";
    //Menu Views
    std::shared_ptr<MenuView>     lightMenu;
    std::shared_ptr<MenuView>     talkMenu;
    
    //OSC
    ofxOscReceiver receiver;
    void setupOsc();
    void parseOsc();
    void setMode(string oscId);
    
//    ofx::AMPMClientRef mAMPM;
//    void setupAMPM();
    
    //OSC handlers
    void musicPlay();
    void musicChange();
    void musicStop();
    bool musicControl = false;
    string currentMode;
    string mControlsString;
    
    void onQuestionStarted();
    void onResponseComplete();
    
    //MUSIC CTRL
    string playListName = "DiscoBall"; //name of the iTunes playlist
    float musicStartTime = 0; //when the music started
    float playDuration = 30000; //in ms
    int maxVolume = 3; //in 7 is highest on mac
    int lowVolume = 2;
    bool isPlaying = false;
    void setItunesVolume(float vol);
    
    std::shared_ptr<ofTrueTypeFont> notoRegular;
    
    float targetVolume=0;
    float currentVolume=0;
    void toggleVolume();
    void fadeVolume(float val);
    void setVolume(float val);
    void nextTrack();
    void prevTrack();
    void startPlaylist();
    void appleScriptCmd(string cmd);
    void setDebug(bool val);
    void toggleScale();
};
