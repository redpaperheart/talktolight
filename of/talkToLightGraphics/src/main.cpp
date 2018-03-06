#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
	// had to add the following ifdef in order make the shader work
	// need to make sure it's not causing issues -- SD 
	
	#ifdef __linux__ 
		ofGLESWindowSettings settings;
		settings.glesVersion = 2;
		settings.width = W_WIDTH;
		settings.height = W_HEIGHT;
		ofCreateWindow(settings);
	#elif _WIN32 || _WIN64 || __APPLE__
		ofSetupOpenGL(W_WIDTH, W_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
	#endif
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
