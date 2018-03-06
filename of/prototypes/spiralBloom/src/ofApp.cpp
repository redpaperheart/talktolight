#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    num = 180;
    c = ofColor(255);
    center = ofVec2f( ofGetWindowSize() / 2 );
    debug = false;
    
    mGui.setup();
    mGui.add(spacing.setup("Spacing", 1.5, 0, 10));
    mGui.add(theta.setup("Theta", 137.5, 0, 360));
    mGui.add(numCirclesSlider.setup("Circles", 180, 0, 360));
}

//--------------------------------------------------------------
void ofApp::update()
{
    num = numCirclesSlider;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    
    ofBackground(0, 0, 0);
    
    ofPushMatrix();
    ofTranslate( center );
    ofRotate( ofGetElapsedTimef() );
    
    for (int i = 45; i < num; i++)
    {
        ofSetColor(c);
        ofRotate(theta);
        
        bloomScale = ofClamp(abs(sin( ofGetElapsedTimef() + i )),
                             0.2, 1) ;
        
        ofDrawCircle( 0, i*spacing,
                      sqrt(abs(i)) * bloomScale );
    }
    
    ofPopMatrix();
    
    if (debug) mGui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_TAB)
    {
        debug = !debug;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
