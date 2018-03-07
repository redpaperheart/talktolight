#include "ofApp.h"
#include "LightModeScanner.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    cout << "Launching Talk To Light with OpenGL version " << glGetString(GL_VERSION) << endl;
    
    //    ofToggleFullscreen();
    mModel = Model::getInstance();
    loadSettings();
    mControlsString =
    "CONTROLS\n\n"
    "d - toggle debug \n"
    "g - toggle grid \n"
    "j - minimize app \n"
    "f - toggle full screen \n\n"
    
    "b - normal\n"
    "n - question \n"
    "m - response\n\n"
    
    "q - mode Disco \n"
    "w - mode Laser \n"
	"e - mode Aurora \n"
    "r - mode Robot \n"
    "t - mode Spotlight \n"
    "y - mode Alien \n"
    "u - mode Spaceship \n"
    "i - mode FireFly \n"
    "o - mode Scanner \n"
    "p - mode Rainbow \n"
    "g - mode Good Night \n"
    
    "\n"
    "<- / -> - disco sensitivity "
    "\n"
    
    "\n\nINPUT DEVICES\n\n";
    
    auto devices = mSoundStream.getDeviceList();
    
    int audioChannelDefault = currentDeviceId; //-1;
    for (auto &device : devices) {
        if (device.inputChannels > 0){
            mControlsString += ofToString(device.deviceID) + " - " + device.name + "\n";
            if (audioChannelDefault < 0) audioChannelDefault = device.deviceID;
        }
    }
    
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(0, 0, 0);
	
    setupOsc();
    setupAMPM();

    setupAudio(audioChannelDefault);
    
    mLight.setup();
    changeToRandomMode();
	
}
void ofApp::loadSettings(){
    //create the data folder if it doesn't exist
    if(!ofDirectory::doesDirectoryExist("./")){
        cout << "no data directory, creating:" << endl;
        string path = "../data";
        ofDirectory dir(path);
        if(!dir.exists()){
            dir.create(true);
        }
    }
    if(settings.loadFile("settings.xml")){
    }else{
        //assets path is different on windows
        #if  defined(__linux__) || defined(__APPLE__)
            settings.setValue("settings:assetPath", "../../assets");
        #elif defined(_WIN32) || defined(_WIN64)
            settings.setValue("settings:assetPath", "../../../assets");
        #endif

        settings.setValue("settings:launchFullscreen", false);
        settings.setValue("settings:launchDebug", false);
        settings.setValue("settings:audioChannelDefault", -1); //default device id
        settings.setValue("settings:musicControlIP", "192.168.0.61");
        settings.setValue("settings:menuControlIP", "192.168.0.60");
        settings.setValue("settings:idleTimeout", 60000);
        settings.setValue("settings:volumeScaler", 1.0);
        settings.saveFile("settings.xml");
    }
    
    //then use the settings
    mModel->assetPath = settings.getValue("settings:assetPath", "../../assets");
    currentDeviceId = settings.getValue("settings:audioChannelDefault", -1);
    setDebug(settings.getValue("settings:launchDebug", false));
    mModel->musicControlIP = settings.getValue("settings:musicControlIP", "192.168.0.61");
    mModel->menuControlIP = settings.getValue("settings:menuControlIP", "192.168.0.60");
    mModel->volumeScaler = settings.getValue("settings:volumeScaler", 1.0);
    mModel->bDebug = mDebug;
    mModel->idleTimeout = settings.getValue("settings:idleTimeout", 60000);
    if(settings.getValue("settings:launchFullscreen", false)){
        ofSetFullscreen(true);
    }
    
}
void ofApp::update(){
	
	mModel->mBeat.update(ofGetElapsedTimeMillis());
    mAMPM->update();
	//lets scale the vol up to a 0-1 range 
	//mScaledVol = ofMap(mSmoothedVol, 0.0, 0.17, 0.0, 1.0, true);
	mModel->mVolScaled = ofMap(mModel->mVolCur *mModel->volumeScaler, 0.0, 0.17, 0.0, 1.0, true);

    //lets record the volume into an array
    mModel->mVolHistory.push_back( mModel->mVolCur );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( mModel->mVolHistory.size() >= 400 ){
        mModel->mVolHistory.erase(mModel->mVolHistory.begin(), mModel->mVolHistory.begin()+1);
    }
    if(mModel->mVolScaled > mModel->mVolTop){
        mModel->mVolTop = mModel->mVolScaled;
    }else{
        mModel->mVolTop *= 0.98;
    }
    
	parseOsc();
    
    mLight.update();
    
    if(ofGetElapsedTimeMillis() > mModel->idleTimer){
        changeToRandomMode();
    }
}


void ofApp::draw()
{
	//ofEnableAlphaBlending();
	mLight.draw();
    drawDebug();
}

void ofApp::setMode(string osc){
    if (osc == OSC_MODE_LIGHT) {
        mLight.setMode(Model::MODE_LIGHTS);
    }
    else if (osc == OSC_MODE_AURORA) {
        mLight.setMode(Model::MODE_AURORA);
    }
    else if (osc == OSC_MODE_SPACESHIP) {
        mLight.setMode(Model::MODE_SPACESHIP);
    }
    else if (osc == OSC_MODE_ALIEN) {
        mLight.setMode(Model::MODE_ALIEN);
    }
    else if (osc == OSC_MODE_RAINBOW) {
        mLight.setMode(Model::MODE_RAINBOW);
    }
    else if (osc == OSC_MODE_CAT) {
        mLight.setMode(Model::MODE_LASER);
    }
    else if (osc == OSC_MODE_DISCO) {
        mLight.setMode(Model::MODE_DISCO);
    }
    else if (osc == OSC_MODE_SCANNER) {
        mLight.setMode(Model::MODE_SCANNER);
    }
    else if (osc == OSC_MODE_ROBOT) {
        mLight.setMode(Model::MODE_ROBOT);
    }
    else if (osc == OSC_MODE_SPOTLIGHT) {
        mLight.setMode(Model::MODE_SPOTLIGHT);
    }
    else if (osc == OSC_MODE_FIREFLY) {
        mLight.setMode(Model::MODE_FIREFLY);
    }
    else if (osc == OSC_MODE_GOODNIGHT) {
        mLight.setMode(Model::MODE_GOODNIGHT);
    }
    //relay the mode back out over osc
    sendMessage(osc);
}
void ofApp::changeToRandomMode(){
    cout << "changing to random mode" << endl;
    
    int r = ofRandom(0, 10);
    switch(r){
        case 0:
            setMode(OSC_MODE_AURORA);
            break;
        case 1:
            setMode(OSC_MODE_SPACESHIP);
            break;
        case 2:
            setMode(OSC_MODE_ALIEN);
            break;
        case 3:
            setMode(OSC_MODE_RAINBOW);
            break;
        case 4:
            setMode(OSC_MODE_CAT);
            break;
        case 5:
            setMode(OSC_MODE_DISCO);
            break;
        case 6:
            setMode(OSC_MODE_SCANNER);
            break;
        case 7:
            setMode(OSC_MODE_ROBOT);
            break;
        case 8:
            setMode(OSC_MODE_SPOTLIGHT);
            break;
        case 9:
            setMode(OSC_MODE_FIREFLY);
            break;
        default:
            setMode(OSC_MODE_RAINBOW);
            break;
    }
}
// Audio
//--------------------------------------------------------------

void ofApp::setupAudio(unsigned int deviceId)
{
	if (mModel->mInputConnected && mModel->mCurDevice.deviceID == deviceId) return;
    
    cout << "SETUPAUDIO" << endl;
    cout << "mBeatBuffer size:" << mModel->mBeat.getBufferSize()<< endl;
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
        currentDeviceId = deviceId;
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
    mModel->mVolSmoothed    = 0.0;
    mModel->mVolScaled		= 0.0;
    mModel->mVolCur         = 0.0;
    
    mModel->mInputConnected = mSoundStream.setup(this, 0, min(2, (int)mModel->mCurDevice.inputChannels), 48000, bufferSize, 4);
    mSoundStream.start();
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
    mModel->mBeat.audioReceived(input, bufferSize, nChannels);
    
    
    //TODO is this old audio sampling? should get replaced with ofxBeat?
    mModel->mVolCur = 0.0;
    
    if (mModel->mCurDevice.inputChannels == 0) return;

    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    if (mModel->mCurDevice.inputChannels >= 2) {
        for (int i = 0; i < bufferSize; i++){
                mModel->mLeft[i]	= input[i*2] * 0.5;
                mModel->mRight[i]	= input[i*2+1] * 0.5;
                mModel->mVolCur     += mModel->mLeft[i] * mModel->mLeft[i];
                mModel->mVolCur     += mModel->mRight[i] * mModel->mRight[i];
                numCounted  += 2;
            }
    }
    else if (mModel->mCurDevice.inputChannels == 1) {
        for (int i = 0; i < bufferSize; i++){
            mModel->mLeft[i]	    = input[i] ;
            mModel->mVolCur     += mModel->mLeft[i] * mModel->mLeft[i];
            numCounted  ++;
        }
    }
    
    //this is how we get the mean of rms :)
    mModel->mVolCur /= (float)numCounted;
    
    // this is how we get the root of rms :)
    mModel->mVolCur = sqrt( mModel->mVolCur );
    
    mModel->mVolSmoothed *= 0.93;
    mModel->mVolSmoothed += 0.07 * mModel->mVolCur;
    
    mModel->mBufferCounter++;

    //cout << " cur vol: " << mModel->mVolCur << " smooth vol: " << mModel->mVolSmoothed << endl;
}


// OSC
//--------------------------------------------------------------
void ofApp::setupAMPM(){
    mAMPM = ofx::AMPMClient::create( 3002, 3003 );
}
void ofApp::setupOsc()
{
    // listen to osc on the given port
    cout << "listening for osc messages on port " << OSC_PORT << endl;
    receiver.setup(OSC_PORT);
    
    //menu sender
    menuSender.setup(mModel->menuControlIP, OSC_PORT);
    musicSender.setup(mModel->musicControlIP, OSC_PORT);
}

void ofApp::sendMessage(string msg){
    ofxOscMessage m;
    m.setAddress( msg );
    
    menuSender.sendMessage(m);
    musicSender.sendMessage(m);
}

void ofApp::parseOsc()
{
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);
        
        auto addr = m.getAddress();
//        string str1( "Alpha Beta Gamma Delta" );
        
        //check if it's a mode change
        string::size_type isMode = addr.find( "/mode/", 0 );
        if( isMode != string::npos ) {
            cout << "Mode Change " << isMode << endl;
            setMode(addr);
        }
		
		if (addr == OSC_QUESTION_STARTED) {
            //mLight.setMode(Light::QUESTION);
            mLight.changeState(Model::STATE_QUESTION);
            sendMessage(addr);
		}
        else if (addr == OSC_RESPONSE_STARTED) {
            //mLight.setMode(Light::RESPONSE);
            mLight.changeState(Model::STATE_RESPONSE);
            sendMessage(addr);
        }
        else if (addr == OSC_RESPONSE_ENDED) {
            //mLight.setPreviousMode();
            mLight.changeState(Model::STATE_NORMAL);
            sendMessage(addr);
        }
        else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// debug
			cout << "received OSC: " << msg_string << endl;
		}
	}
}

// Graphics
//--------------------------------------------------------------

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

	ofDrawRectangle(0, 200 , 100, -150 * mModel->mBeat.kick() );
	ofDrawRectangle(125, 200, 100, -150 * mModel->mBeat.snare() );
	ofDrawRectangle(250, 200, 100, -150 * mModel->mBeat.hihat() );

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

void ofApp::drawOfxBeatBand()
{
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
        ofDrawRectangle( i*10, 375, 5, -100.0f * mModel->mBeat.getBand(i) );
        avg += mModel->mBeat.getBand(i);

        // averaging only the bass bands i.e. first 5 bands
        if( i < 5){
            avgBass += mModel->mBeat.getBand(i);
        } 
    }

    // drawing avg bar
    avg /= 32; //average value of 32 bands
    mModel->mFFTAvg = avg;
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

void ofApp::setDebug(bool val){
    mDebug = val;
    mModel->bDebug = mDebug;
    if(mDebug){
        ofShowCursor();
    }else{
        ofHideCursor();
    }
}
void ofApp::drawDebug()
{
    if (!mDebug) return;
    
    ofSetColor(225);
    ofDrawBitmapString(mControlsString, 50, 50);
    
    ofNoFill();
    
    // drawing bars from ofxBeat addon
    drawOfxBeatBars();

    // draw band from ofxBeat
    drawOfxBeatBand();
    

	if (mModel->bShowGrid) {
		//draw grid lines
		float gCount = 5;
		ofSetColor(210, 210, 210);
		{
			ofPolyline line;
			ofPoint pt;
			ofPoint pt2;
			pt.set(0, 0);
			line.addVertex(pt);

			pt2.set(0, ofGetWindowHeight());
			line.addVertex(pt2);
			ofPushMatrix();
			ofSetLineWidth(1);
			for (int c = 0; c < gCount; c++) {
				ofTranslate(ofGetWindowWidth() / gCount, 0, 0);
				line.draw();
			}
			ofPopMatrix();
		}
		{
			ofPolyline line;
			ofPoint pt;
			ofPoint pt2;
			pt.set(0, 0);
			line.addVertex(pt);
			pt2.set(ofGetWindowWidth(), 0);
			line.addVertex(pt2);

			ofPushMatrix();
			ofSetLineWidth(1);
			for (int r = 0; r < gCount; r++) {
				ofTranslate(0, ofGetWindowHeight() / gCount, 0);
				line.draw();
			}
			ofPopMatrix();
		}
        
		//draw border
		ofSetColor(255, 255, 255);
		ofSetLineWidth(3);
		ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	}
    
    ofSetColor(225);
    
    string reportString =
        string("\nSTATUS\n\n") +
        "\nmode: " + ofToString(mModel->mCurMode) +
        "\nstate: " + ofToString(mModel->getCurState()) +
        "\nVolume Scaler: " + ofToString(mModel->volumeScaler) +
        "\ninput connected: " + ofToString(mModel->mInputConnected) +
        "\nbuffers received: " + ofToString(mModel->mBufferCounter) +
        "\nticks: " + ofToString(mSoundStream.getTickCount()) +
        "\nfps: " + ofToString((int) ofGetFrameRate()) +
        "\nres: " + ofToString((int) ofGetWindowWidth()) + "x" + ofToString((int) ofGetWindowHeight());

    
    ofDrawBitmapString(reportString, 50, ofGetWindowHeight() - 170);
}

void ofApp::setAppMinimized(bool val){
	mAppMin =val;
	if(mAppMin){
		ofSetWindowShape(200, 200);
		ofSetWindowPosition(10, 10);
	}else{
		ofSetWindowShape(W_WIDTH, W_HEIGHT);
    ofSetWindowPosition(0, 0);
	}
}

// Events
//--------------------------------------------------------------

void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'd':
            setDebug(!mDebug);
            break;
        case 'f':
            ofToggleFullscreen();
            break;
		case 'g':
			mModel->bShowGrid = !mModel->bShowGrid;
			break;
		case 'j':
			setAppMinimized(!mAppMin);
			break;
        //State Changes
        case 'b':
            mLight.changeState(Model::STATE_NORMAL);
            sendMessage(OSC_RESPONSE_ENDED);
            break;
        case 'n':
            mLight.changeState(Model::STATE_QUESTION);
            sendMessage(OSC_QUESTION_STARTED);
            break;
        case 'm':
            mLight.changeState(Model::STATE_RESPONSE);
            sendMessage(OSC_RESPONSE_STARTED);
            break;
        
        case 'q':
            setMode(OSC_MODE_DISCO);
            break;
        case 'w':
            setMode(OSC_MODE_CAT);
            break;
        case 'e':
            setMode(OSC_MODE_AURORA);
            break;
        case 'r':
            setMode(OSC_MODE_ROBOT);
            break;
        case 't':
            setMode(OSC_MODE_SPOTLIGHT);
            break;
		case 'y':
			setMode(OSC_MODE_ALIEN);
			break;
        case 'u':
            setMode(OSC_MODE_SPACESHIP);
            break;
		case 'i':
			setMode(OSC_MODE_FIREFLY);
			break;
        case 'I':
            setMode(OSC_MODE_SPOTLIGHT);
            break;
        case 'o':
            setMode(OSC_MODE_SCANNER);
            break;
        case 'p':
        case 'P':
            setMode(OSC_MODE_RAINBOW);
            break;
        case 'h':
        case 'H':
            setMode(OSC_MODE_GOODNIGHT);
            break;
        case 's':
            changeToRandomMode();
            break;
		case 'x':
			mModel->getInstance()->mTrigger = !(mModel->getInstance()->mTrigger);
			break;
        case ' ':
            mLight.reloadShaders();
            break;
        case '.':
            cout << ofGetFrameRate() << " fps" << endl;
            break;
        case OF_KEY_RIGHT :{
            float s = .1;
            mModel->volumeScaler = ofClamp(mModel->volumeScaler+s, .1, 100);
            break;}
        case OF_KEY_LEFT :{
            float a = .1;
            mModel->volumeScaler = ofClamp(mModel->volumeScaler-a, .1, 100);
            break;}
//        case OF_KEY_LEFT :
//                float r = .1;
//                mModel->volumeScaler = ofClamp(mModel->volumeScaler+s, .1, 100);
//            break;
    }
    
    if (key >= '0' && key <= '9') {
        int deviceId = key - '0';
        setupAudio(deviceId);
    }
}

//-
void ofApp::exit(){
    cout << "exiting" << endl;
    //save the device id
    settings.setValue("settings:audioChannelDefault", currentDeviceId); //default device id
    settings.saveFile("settings.xml");
    
    //clean up sound stream?
//    mSoundStream.stop();
//    mSoundStream.close();
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
    mLight.resize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
