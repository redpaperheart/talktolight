#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "listening for osc messages on port " << OSC_STATUS_PORT << endl;
    receiver.setup(OSC_STATUS_PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    parseOsc();
}

//--------------------------------------------------------------
void ofApp::draw(){
    bool noHeartBeat = lastHeartBeat + hearBeatTimeout< ofGetElapsedTimeMillis();
    if(noHeartBeat){
        ofBackground(255, 0, 0);
    }else{
        ofBackground(0, 0, 0);
    }
    
    ofSetColor(255, 255, 255);
    
    string statusString =
    string("\nSTATUS\n\n") +
    "\nMODE: " + currentMode +
    "\nSTATE: " + currentState +
    "\nLAST HEART BEAT: " + ofToString(lastHeartBeat) +
    "\nTIME DEAD: " + ofToString(ofGetElapsedTimeMillis()-lastHeartBeat) +
    "\nAverage Volume: ?" + ofToString(0);
    
    ofDrawBitmapString(statusString, 50, 50);
}

void ofApp::onHeartBeat(){
    lastHeartBeat = ofGetElapsedTimeMillis();
}
void ofApp::parseOsc()
{
    cout << "parseOsc " << endl;
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        auto addr = m.getAddress();
        
        //check if it's a mode change
        string::size_type isMode = addr.find( "/mode/", 0 );
        if( isMode != string::npos ) {
            currentMode = addr;
        }
        if (addr == OSC_STATUS_HEART) {
            onHeartBeat();
        }
        if (addr == OSC_QUESTION_STARTED) {
            currentState = OSC_QUESTION_STARTED;
        }
        else if (addr == OSC_RESPONSE_STARTED) {
            currentState = OSC_RESPONSE_STARTED;
        }
        else if (addr == OSC_RESPONSE_ENDED) {
             currentState = OSC_RESPONSE_ENDED;
        }
        else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
//            msg_string += ": ";
//            for(int i = 0; i < m.getNumArgs(); i++){
//                // get the argument type
//                msg_string += m.getArgTypeName(i);
//                msg_string += ":";
//                // display the argument - make sure we get the right type
//                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
//                    msg_string += ofToString(m.getArgAsInt32(i));
//                }
//                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
//                    msg_string += ofToString(m.getArgAsFloat(i));
//                }
//                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
//                    msg_string += m.getArgAsString(i);
//                }
//                else{
//                    msg_string += "unknown";
//                }
//            }
            // debug
            cout << "received OSC: " << msg_string << endl;
        }
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
