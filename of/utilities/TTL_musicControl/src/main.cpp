#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
//    ofSetupOpenGL(1200,1000,OF_WINDOW);            // <-------- setup the GL context
//    ofRunApp(new ofApp());

    ofGLFWWindowSettings settings;
    settings.multiMonitorFullScreen = true;
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
