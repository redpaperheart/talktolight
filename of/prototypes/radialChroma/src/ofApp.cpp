#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    radius = 0;
    center = ofGetWindowSize() * 0.5;
    num = 12;
    palette =
    {
        ofFloatColor::red,
        ofFloatColor::green,
        ofFloatColor::blue,
        ofFloatColor::yellow
    };
}

//--------------------------------------------------------------
void ofApp::update(){
    radius = ofGetMouseX() * 0.5;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofNoFill();
    ofSetCircleResolution(128);
    
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    
    ofPushMatrix();
    ofTranslate(center);
    
    this->drawRings( 500.0f );
    
    ofPopMatrix();
    
    ofEnableAlphaBlending();
}

void ofApp::drawRings( float _initSize )
{
    for( int i = 0; i < num; i++)
    {
        ofPushMatrix();
        ofRotate( 360 / num * i );
        
        int colorIndex = i % palette.size();
        ofSetColor(palette[colorIndex]);
        ofDrawEllipse( 0, 0 + radius, _initSize, _initSize);
        
        ofPopMatrix();
    }
    
    if (_initSize > 50) {
        drawRings(_initSize - 100);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
