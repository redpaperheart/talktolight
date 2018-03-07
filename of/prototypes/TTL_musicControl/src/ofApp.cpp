#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    loadSettings();
    Tweenzor::init();
    setupOsc();
    setupAMPM();
    

    if(!musicControl){
        setupViews();
    }else{
        targetVolume = currentVolume;
        setVolume(currentVolume);
        //start the playlist playing
        startPlaylist();
    }
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofBackground(0, 0, 0);
    
//    ofSetWindowShape(1080*2, 1920);
//    ofSetWindowPosition(0, 0);
    mControlsString =
    "CONTROLS\n\n"
    "d - toggle debug \n"
    "c - toggle cursor \n"
    "f - toggle full screen \n\n"
    
    "q - mode Disco \n"
    "w - mode Laser \n"
    "e - mode Aurora \n"
    "r - mode Robot \n"
    "t - mode Spotlight \n"
    "y - mode Alien \n"
    "u - mode Spaceship \n"
    "i - mode FireFly \n"
    "o - mode Scanner \n"
    "p - mode Rainbow \n\n"
    "h - mode Goodnight \n\n"
    
    "k - Question Started \n"
    "l - Response Ended \n";
}

void ofApp::loadSettings(){
    if(!ofDirectory::doesDirectoryExist("./")){
        cout << "no data directory, creating:" << endl;
        string path = "../data";
        ofDirectory dir(path);
        if(!dir.exists()){
            dir.create(true);
        }
    }
    if(settings.loadFile("settings.xml")){
        //use them
    }else{
        //create them
        ofxXmlSettings settings;
        settings.setValue("settings:maxVolumeLevel", 7);
        settings.setValue("settings:lowVolumeLevel", 2);
        settings.setValue("settings:assetPath", "../../assets/");
        settings.setValue("settings:playListName", "DiscoBall");
        settings.setValue("settings:playDuration", 3000);
        settings.setValue("settings:musicControl", false);
        settings.setValue("settings:debug", false);
        settings.setValue("settings:font", "NotoSans-Regular.ttf");
        settings.saveFile("settings.xml");
    }
    
    //then use the settings
    maxVolume = settings.getValue("settings:maxVolumeLevel", 7);
    lowVolume = settings.getValue("settings:lowVolumeLevel", 2);
    playListName = settings.getValue("settings:playListName", "DiscoBall");
    playDuration = settings.getValue("settings:playDuration", 3000);
    assetPath = settings.getValue("settings:assetPath", "../../assets/");
    setDebug(settings.getValue("settings:debug", false));
    musicControl = settings.getValue("settings:musicControl", false);
    font = settings.getValue("settings:font", "NotoSans-Regular.ttf");
    if(menuText.loadFile(assetPath + "menuText.xml")){
        
    }
}
void ofApp::setupViews(){
    ofTrueTypeFont::setGlobalDpi(96);
    
    notoRegular = std::make_shared<ofTrueTypeFont>();
    notoRegular->load(assetPath + "NotoSans-Regular.ttf", 50, true, true);
    notoRegular->setLineHeight(60);
    notoRegular->setLetterSpacing(1.037);
    
    lightMenu = std::make_shared<MenuView>();
    lightMenu->font = notoRegular;
    lightMenu->setupLight();
    
    //get the menu text
    menuText.pushTag("menuText");
    menuText.pushTag("lightMenu");
    int modeCount = menuText.getNumTags("mode");
    for(int i = 0; i < modeCount; i++){
        menuText.pushTag("mode", i);
        cout << "text:"<< menuText.getValue("text", "default") << endl;
        cout << "id:"<< menuText.getValue("id", "none") << endl;
        lightMenu->addItem(menuText.getValue("text", "default"), menuText.getValue("id", "none"), lightMenu->mLineHeight);
        menuText.popTag();
    }
    menuText.popTag();
    menuText.popTag();
    lightMenu->show();
    
    
    talkMenu = std::make_shared<MenuView>();
    talkMenu->font = notoRegular;
    talkMenu->setupTalk();
    talkMenu->mPosition = ofVec2f(1080, 0);
    
    //get the menu text
    menuText.pushTag("menuText");
    menuText.pushTag("talkMenu");
    modeCount = menuText.getNumTags("mode");
    for(int i = 0; i < modeCount; i++){
        
        menuText.pushTag("mode", i);
        string oscId = menuText.getValue("id", "none");
        std::shared_ptr<talkList> list = talkMenu->addList(oscId);
        
        menuText.pushTag("options");
        int optionCount = menuText.getNumTags("option");
        cout << "optionCount:"<< optionCount << endl;
        for(int o = 0; o < optionCount; o++){
            menuText.pushTag("option", o);
            talkMenu->addListItem(list, menuText.getValue("text", "default"), oscId, lightMenu->mLineHeight * menuText.getAttribute("text", "lines", 2));
            menuText.popTag();
        }
        menuText.popTag();
        menuText.popTag();
       
    }
    menuText.popTag();
    menuText.popTag();
    talkMenu->show();
    
}
void ofApp::setMode(string oscId){
    currentMode = oscId;
    if(musicControl){
        if(currentMode==OSC_MODE_DISCO){
            musicPlay();
        }else{
            musicStop();
        }
    }else{
        lightMenu->activateMode(oscId);
        talkMenu->activateMode(oscId);
    }
}
void ofApp::onQuestionStarted(){
    if(currentMode==OSC_MODE_DISCO){
        fadeVolume(lowVolume);
    }
}
void ofApp::onResponseComplete(){
    if(currentMode==OSC_MODE_DISCO){
        fadeVolume(maxVolume);
    }
}
void ofApp::setDebug(bool val){
    debug = val;
    if(debug){
        ofShowCursor();
    }else{
        //hide the cursor
        ofHideCursor();
        ofSetFullscreen(true);
    }
    if(musicControl){
    }else{
        if(lightMenu!=NULL){
            lightMenu->debug= debug;
            talkMenu->debug= debug;
        }
    }
    
}
void ofApp::toggleScale(){
    if(musicControl){
        
    }else{
        if(lightMenu->mScale == 1){
            lightMenu->mScale = .5;
            talkMenu->mScale = .5;
        }else{
            lightMenu->mScale = 1;
            talkMenu->mScale = 1;
        }
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    //
    Tweenzor::update( ofGetElapsedTimeMillis() );
    mAMPM->update();
    if(currentVolume != targetVolume){
        if(abs(targetVolume-currentVolume) < 0.2f){
            currentVolume = targetVolume;
        }
        currentVolume += (targetVolume-currentVolume)*.2f;
        setVolume(currentVolume);
    }
    //check if the music should timeout
    
    parseOsc();
    
    lightMenu->update();
    talkMenu->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(musicControl){
        ofSetColor(225);
        ofDrawBitmapString(mControlsString + "\n Mode: " + currentMode, 50, 50);
    }else{
        lightMenu->draw();
        talkMenu->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'd':
            setDebug(!debug);
        case 's':
            toggleScale();
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
        case 'o':
            setMode(OSC_MODE_SCANNER);
            break;
        case 'p':
            setMode(OSC_MODE_RAINBOW);
            break;
        case 'h':
            setMode(OSC_MODE_GOODNIGHT);
            break;
        case 'a':
            setMode("default");
            break;
        case 'k':
            onQuestionStarted();
            break;
        case 'l':
            onResponseComplete();
            break;
        case 'm':
            musicStop();
            break;
        case 'b':
            musicChange();
            break;
    }
}

// APPLESCRIPT + MUSIC METHODS
//--------------------------------------------------------------
void ofApp::toggleVolume(){
    if(targetVolume!=0){
        fadeVolume(0);
    }else{
        fadeVolume(maxVolume);
    }
}
void ofApp::fadeVolume(float val){
    targetVolume = val;
}
void ofApp::setVolume(float val){
    appleScriptCmd("set Volume " + ofToString(val));
}
void ofApp::nextTrack(){
    appleScriptCmd("tell app \"iTunes\" to play next track");
}
void ofApp::prevTrack(){
    appleScriptCmd("tell app \"iTunes\" to play previous track");
}
void ofApp::startPlaylist(){
    appleScriptCmd("tell application \"iTunes\" to play playlist \""+playListName+"\"");
}
void ofApp::appleScriptCmd(string cmd){
    //wrap the command up and send it
    string commandString = "osascript -e '" + cmd + "'";
    system(commandString.c_str());
}

// OSC HANDLERS
//--------------------------------------------------------------
void ofApp::musicPlay(){
    musicStartTime = ofGetElapsedTimeMillis();
    fadeVolume(maxVolume);
    isPlaying=true;
}
void ofApp::musicChange(){
    musicStartTime = ofGetElapsedTimeMillis();
    nextTrack();
    isPlaying=true;
}
void ofApp::musicStop(){
    musicStartTime = 0;
    isPlaying=false;
    fadeVolume(0);
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
}

void ofApp::parseOsc()
{
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        auto addr = m.getAddress();
        cout << "OSC:  " << addr << endl;
        
        if (addr == OSC_MUSIC_PLAY) {
            musicPlay();
        }
        else if(addr == OSC_MUSIC_STOP){
            musicStop();
        }
        else if(addr == OSC_MUSIC_CHANGE){
            musicChange();
        }
        else if(addr == OSC_QUESTION_STARTED){
            //question
            onQuestionStarted();
        }
        else if(addr == OSC_RESPONSE_STARTED){
            //response started
        }
        else if(addr == OSC_RESPONSE_ENDED){
            //response end
            onResponseComplete();
        }
        else if(addr == OSC_MODE_DISCO){
            setMode(OSC_MODE_DISCO);
        }
        else if(addr == OSC_MODE_CAT){
            setMode(OSC_MODE_CAT);
        }
        else if(addr == OSC_MODE_AURORA){
            setMode(OSC_MODE_AURORA);
        }
        else if(addr == OSC_MODE_ROBOT){
            setMode(OSC_MODE_ROBOT);
        }
        else if(addr == OSC_MODE_SPOTLIGHT){
            setMode(OSC_MODE_SPOTLIGHT);
        }
        else if(addr == OSC_MODE_ALIEN){
            setMode(OSC_MODE_ALIEN);
        }
        else if(addr == OSC_MODE_SPACESHIP){
            setMode(OSC_MODE_SPACESHIP);
        }
        else if(addr == OSC_MODE_FIREFLY){
            setMode(OSC_MODE_FIREFLY);
        }
        else if(addr == OSC_MODE_RAINBOW){
            setMode(OSC_MODE_RAINBOW);
        }
        else if(addr == OSC_MODE_SCANNER){
            setMode(OSC_MODE_SCANNER);
        }
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
