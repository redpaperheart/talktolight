#include "ofApp.h"
#include "Model.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mModel = Model::getInstance();
	mModel->mThresholdVol = 0.05;
	
	mControlsString = "\n\nINPUT DEVICES\n\n";

	auto devices = mSoundStream.getDeviceList();
	for (auto &device : devices) {
		if (device.inputChannels > 0){
			mControlsString += ofToString(device.deviceID) + " - " + device.name + "\n";
		}
	}

	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(0, 0, 0);

	//setupAudio(2); //calling it manually by pressing device id number  
	mLight.setup();
	mLight.setMode(Model::MODE_DISCO);

	//ofSoundStreamSetup(0, 1, this, 44100, mBeat.getBufferSize(), 4);
	mBeat.enableBeatDetect();
}

//--------------------------------------------------------------
void ofApp::update(){

	mBeat.update(ofGetElapsedTimeMillis());
	mModel->mKick = mBeat.kick();
	mModel->mSnare = mBeat.snare();
	mModel->mHihat = mBeat.hihat();
	//cout << mBeat.kick() << "," << mBeat.snare() << "," << mBeat.hihat() << endl;

	////lets scale the vol up to a 0-1 range 
	//mScaledVol = ofMap(mSmoothedVol, 0.0, 0.17, 0.0, 1.0, true);
	mModel->mScaledVol = ofMap(mModel->mCurVol, 0.0, 0.17, 0.0, 1.0, true);

	////lets record the volume into an array
	mModel->mVolHistory.push_back( mModel->mScaledVol );

	////if we are bigger the the size we want to record - lets drop the oldest value
	if( mModel->mVolHistory.size() >= 400 ){
		mModel->mVolHistory.erase(mModel->mVolHistory.begin(), mModel->mVolHistory.begin()+1);
	}
	
	mLight.setAudioLevel(mModel->mCurVol);
	mLight.update();

}



// Audio
//--------------------------------------------------------------

void ofApp::setupAudio(unsigned int deviceId)
{
	if (mModel->mInputConnected && mModel->mCurDevice.deviceID == deviceId) return;

	// 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	mSoundStream.stop();
	mSoundStream.close();
	mSoundStream.printDeviceList();
	mSoundStream.setDeviceID(deviceId);

	try {
		auto device = mModel->mCurDevice = mSoundStream.getDeviceList().at(deviceId);
		cout << "Trying to setup device " << deviceId << endl;
		cout << "   name: " << device.name << endl;
		cout << "   in channels: " << device.inputChannels << endl;
		cout << "   rates: ";

		for (auto rate : device.sampleRates) {
			cout << rate << " ";
		}
		cout << endl;
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
	}

	int bufferSize = 1024;

	mModel->mLeft.assign(bufferSize, 0.0);
	mModel->mRight.assign(bufferSize, 0.0);
	mModel->mVolHistory.assign(400, 0.0);

	mModel->mBufferCounter	= 0;
	mModel->mSmoothedVol    = 0.0;
	mModel->mScaledVol		= 0.0;
	mModel->mCurVol         = 0.0;

	mModel->mInputConnected = mSoundStream.setup(this, 0, min(2, (int)mModel->mCurDevice.inputChannels), 48000, bufferSize, 4);
	//mModel->mInputConnected = mSoundStream.setup(this, 0, 1, 48000, bufferSize, 4);
	mSoundStream.start();
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
	mBeat.audioReceived(input, bufferSize, nChannels);

	mModel->mCurVol = 0.0;

	if (mModel->mCurDevice.inputChannels == 0) return;

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	if (mModel->mCurDevice.inputChannels >= 2) {
		for (int i = 0; i < bufferSize; i++){
			mModel->mLeft[i]	= input[i*2] * 0.5;
			mModel->mRight[i]	= input[i*2+1] * 0.5;
			mModel->mCurVol     += mModel->mLeft[i] * mModel->mLeft[i];
			mModel->mCurVol     += mModel->mRight[i] * mModel->mRight[i];
			numCounted  += 2;
		}
	}
	else if (mModel->mCurDevice.inputChannels == 1) {
		for (int i = 0; i < bufferSize; i++){
			mModel->mLeft[i]	    = input[i] ;
			mModel->mCurVol     += mModel->mLeft[i] * mModel->mLeft[i];
			numCounted  ++;
		}
	}

	//this is how we get the mean of rms :)
	mModel->mCurVol /= (float)numCounted;

	// this is how we get the root of rms :)
	mModel->mCurVol = sqrt( mModel->mCurVol );

	mModel->mSmoothedVol *= 0.93;
	mModel->mSmoothedVol += 0.07 * mModel->mCurVol;

	mModel->mBufferCounter++;

	//cout << "input channels: " << mCurDevice.inputChannels << " cur vol: " << mCurVol << " smooth vol: " << mSmoothedVol << " num counted: " << numCounted << endl; 
}

void ofApp::drawOfxBeatBars()
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() - 450, 50, 0);

	ofSetColor(225);
	ofDrawBitmapString("ofxBeat Bars", 4, 18);

	ofSetLineWidth(1);
	ofDrawRectangle(0, 0, 400, 225); //container box

	ofSetColor(225, 0, 0, 100);
	ofFill();  

	ofTranslate(25,0,0); //padding from left

	ofDrawRectangle(0, 200 , 100, -150 * mBeat.kick() );
	ofDrawRectangle(125, 200, 100, -150 * mBeat.snare() );
	ofDrawRectangle(250, 200, 100, -150 * mBeat.hihat() );

	ofSetColor(225);
	ofTranslate(25,195,0); 
	ofDrawBitmapString("kick", 4, 18);
	ofTranslate(125,0,0); 
	ofDrawBitmapString("snare", 4, 18);
	ofTranslate(125,0,0); 
	ofDrawBitmapString("hihat", 4, 18);

	ofPopMatrix();
	ofPopStyle();

}


void ofApp::drawBeatBand(){
	
		//cout << mBeat.getBand(i) << " " <<endl;
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth() - 450, 300, 0);

		ofSetColor(225);
		ofDrawBitmapString("ofxBeat FFT band ",4, 18);
		ofDrawRectangle(0, 0, 400, 400);

		ofSetColor(225, 0, 0, 100);
		ofFill(); 

		ofTranslate(25,0,0); //padding from left

		float avgBass = 0.0f;
		float avg = 0.0f;
		for(int i = 0; i < 32; i++)
		{
			//ofDrawRectangle( i*10, 400, 5, -100.0f * ofClamp( mBeat.getBand(i), 0, 4.0f )); // clamping because this is only to get a visual idea
			ofDrawRectangle( i*10, 375, 5, -100.0f * mBeat.getBand(i) );
			avg += mBeat.getBand(i);

			// averaging only the bass bands i.e. first 5 bands
			if( i < 5){
				avgBass += mBeat.getBand(i); 
			} 
		}

		// drawing avg bar
		avg /= 32; //average value of 32 bands
		mModel->getInstance()->mFFTAvg = avg;
		ofSetColor(255, 255, 0, 100);
		ofDrawRectangle( 340, 375, 5, -100.0f * avg );

		// drawing avgBass bar
		/*
		avgBass /= 5; //average value of fisrt 5 bands
		mModel->getInstance()->mBassAvg = avgBass;
		ofSetColor(255, 0, 0, 50);
		ofDrawRectangle( 0, 375, 5*10, -100.0f * avgBass );
		*/

		ofPopMatrix();
		ofPopStyle();
}



void ofApp::drawDebug(){
	ofSetColor(225);
	ofDrawBitmapString(mControlsString, 50, 50);

	ofNoFill();

	// drawing bars from ofxBeat addon
	drawOfxBeatBars();

	// draw band from ofxBeat
	drawBeatBand();

	ofSetColor(225);

	string reportString =
		string("\nSTATUS\n\n") +
		"\ninput connected: " + ofToString(mModel->mInputConnected) +
		"\nbuffers received: " + ofToString(mModel->mBufferCounter) +
		"\nticks: " + ofToString(mSoundStream.getTickCount()) +
		"\nfps: " + ofToString((int) ofGetFrameRate())  ;

	ofDrawBitmapString(reportString, 50, ofGetWindowHeight() - 150);
}

//--------------------------------------------------------------
void ofApp::draw(){
	mLight.draw();
	drawDebug();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case '<':
		mModel->mThresholdVol -= 0.001;
		break;
		case '>':
		mModel->mThresholdVol += 0.001;
		break;
	}
	if (key >= '0' && key <= '9') {
		int deviceId = key - '0';
		setupAudio(deviceId);
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
