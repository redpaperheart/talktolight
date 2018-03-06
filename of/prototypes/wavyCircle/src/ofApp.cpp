#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup()
{
    mRad = 150;
    mThickness = 15;
    mFilled = true;
    mPos = ofVec2f( ofGetWidth()/2, ofGetHeight()/2 );
    mColor = ofColor(255);
    mDiameter = 200.f;
    n = 10;
    previousN = n;
    drawDebug = false;
    drawGui = false;
    
    
    mShape.setMode(ofPath::POLYLINES);
    
    setupAudio();
    
    //add points to each array
    for( int i = 0; i < n; i++)
    {
        //plot the outer points of the ring
        ofVec2f a = ofVec2f( mDiameter * sin(ofMap(i, 0, n, 0, TWO_PI)),
                             mDiameter * cos(ofMap(i, 0, n, 0, TWO_PI)));
        outerPoints.push_back(a);
        
        
        //plot the inner points of the ring
        float offsetX = mThickness * sin(ofMap(i, 0, n, 0, TWO_PI));
        float offsetY = mThickness * cos(ofMap(i, 0, n, 0, TWO_PI));
        
        ofVec2f b = ofVec2f( a.x - offsetX,
                             a.y - offsetY);
        innerPoints.push_back(b);
        
        
        //set up the array of offsets applied to the points
        ofVec2f c = ofVec2f();
        offsetPoints.push_back(c);
    }
    
    //set up gui
    mGui.setup("Circle");
    
    mButtons.setup();
    mButtons.add(mDebugButton.setup("Debug", false));
    mButtons.add(mListeningButton.setup("Listening", true));
    
    mRingParams.setup();
    mRingParams.add(mRingResolution.setup("Ring Res", 10, 3, 360));
    mRingParams.add(mRingSize.setup("Ring Size", 145, 10, 500));
    mRingParams.add(mStrokeSlider.setup("Stroke", 10, 1, 50));
    mRingParams.add(mOffsetLimits.setup("Offset Clamp", 5, 1, 6));
    mRingParams.add(mXenoSpeed.setup("Xeno Speed", 0.11, 0, 0.5));
    mRingParams.add(mResetSpeed.setup("Magnitude Reset Rate", 0.8, 0, 1));
    
    mSoundParams.setup();
    mSoundParams.add(mMagnitudeSlider.setup("Magnitude", 0, 0, 1000));
    mSoundParams.add(mMagnitudeCap.setup("Magnitude Cap", 200, 0, 1000));
    mSoundParams.add(mMinThreshold.setup("Min Sound Threshold", 0.05, 0, 1));
    mSoundParams.add(mSoudMultiplier.setup("Sound Multiplier", 125, 0, 1000));
    
    mGui.add(&mButtons);
    mGui.add(&mRingParams);
    mGui.add(&mSoundParams);
}

//--------------------------------------------------------------
void ofApp::update(){
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
    
    updateAudio();
    
    //if the volume is loud enough, turn listenig on
    if (scaledVol > mMinThreshold)
    {
        mListeningButton = true;
    }else
    {
        mListeningButton = false;
    }
    
    
    mListening = mListeningButton;
    
    //if listening is on, add to the magnitude based on the scaled volume
    if(mListening)
    {
        mMagnitudeSlider = mMagnitudeSlider + (scaledVol * (mSoudMultiplier + mRingSize));
        mMagnitudeSlider = mMagnitudeSlider * mResetSpeed;
    }else
    {
        mMagnitudeSlider = mMagnitudeSlider * mResetSpeed;
    }
    if (mMagnitudeSlider > mMagnitudeCap )
    {
        mMagnitudeSlider = mMagnitudeCap;
    }
    
    mMagnitude = mMagnitudeSlider;
    drawDebug = mDebugButton;
    
    mDiameter = mRingSize;
    
    n = mRingResolution;
    
    
    //if there are changes in the thickness, diameter, or resolution,
    //replot the points of the arrays
    if (mThickness != mStrokeSlider)
    {
        mThickness = mStrokeSlider;
        resizeRing(outerPoints, innerPoints);
    }
    
    if (mDiameter != mPreviousDiameter)
    {
        resizeRing(outerPoints, innerPoints);
        mPreviousDiameter = mDiameter;
    }
    if(n != previousN)
    {
        resizeRingRes(outerPoints, innerPoints);
        previousN = n;
    }
    
    updateOffsets(offsetPoints, mMagnitude);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    drawRing(outerPoints, innerPoints);
    
    if(drawGui) mGui.draw();
}

//--------------------------------------------------------------
void ofApp::drawRing(vector<ofVec2f> outside, vector<ofVec2f> inside)
{
    //clear the previous points in the ofPath, if any
    mShape.clear();
    
    if( drawDebug )
    {
        mShape.setStrokeColor(mColor);
        mShape.setStrokeWidth(2);
        mShape.setFilled(false);
    } else
    {
        mShape.setFillColor(ofColor(mColor));
        mShape.setFilled(true);
    }
    
    //draw the outside of the ring
    plotCurves(outerPoints, offsetPoints);
    mShape.close();
    mShape.newSubPath();
    
    //draw the inside of the ring
    plotCurves(innerPoints, offsetPoints);
    mShape.close();
    
    //draw the ring around the desired center
    ofPushMatrix();
    ofTranslate( mPos );
    mShape.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::plotCurves(vector<ofVec2f> points, vector<ofVec2f> offsets)
{
    
    for(int i = 0; i < points.size(); i ++)
    {
        ofVec2f point;
        if( i == 0 )
        {
            //start the path from [points.size()-1] first (this makes it smoother)
            mShape.curveTo(points[points.size()-1] + (offsets[points.size()-1]));
        }
        
        //curve to the current point
        mShape.curveTo(points[i] + offsets[i]);
        
        
        if( i == points.size()-1 )
        {
            //curve to index 0 and 1 again to make the path smooth
            mShape.curveTo(points[0] + (offsets[0]));
            mShape.curveTo(points[1] + (offsets[1]));
        }
        
        if (drawDebug)
        {
            ofPushMatrix();
            ofTranslate( mPos );
            ofNoFill();
            ofSetColor(255, 255, 0);
            
            //unfilled circles the size of the offset clamp
            ofDrawCircle(points[i],
                         TWO_PI * (mDiameter/mOffsetLimits) / mRingResolution);
            
            //circles where all the original points are
            ofFill();
            float temphue = float(i)/float(n) * 255;
            ofSetColor(255-temphue, 0, temphue, 128);
            ofDrawCircle(points[i], 10);
            
            //circles where all the offsetpoints are
            ofSetColor(0, 255, 0, 128);
            ofDrawCircle(point, 5);
            
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateOffsets(vector<ofVec2f> offsets, int _scale)
{
    for( int i = 0; i < offsets.size(); i++)
    {
        if (volHistory.size() > i)
        {
            //re-map each value in volhistory
            ofVec2f delta = ofVec2f( sin( ofMap(volHistory[i], 0, 1, 0, TWO_PI)
                                         * _scale ),
                                     cos( ofMap(volHistory[i], 0, 1, 0, TWO_PI)
                                         * _scale ));
            delta.normalize();
            delta.x *= _scale;
            delta.y *= _scale;
            
            //Xeno formula:
            //newPosition = (1-pct) * targetPositon + pct * currentPositon
            offsetPoints[i].x = mXenoSpeed * delta.x + (1-mXenoSpeed) * offsetPoints[i].x;
            offsetPoints[i].y = mXenoSpeed * delta.y + (1-mXenoSpeed) * offsetPoints[i].y;
            
            //constrain the offsets based ont the mOffsetLimits
            offsetPoints[i].x = ofClamp(offsetPoints[i].x,
                                      -1 * TWO_PI * (mDiameter/mOffsetLimits) / mRingResolution,
                                      TWO_PI * (mDiameter/mOffsetLimits) / mRingResolution);
            offsetPoints[i].y = ofClamp(offsetPoints[i].y,
                                        -1 * TWO_PI * (mDiameter/mOffsetLimits) / mRingResolution,
                                        TWO_PI * (mDiameter/mOffsetLimits) / mRingResolution);
        }
    }
}

//--------------------------------------------------------------
void ofApp::resetOffsets(vector<ofVec2f> offsets)
{
    //resets every offset to (0,0)
    for( int i = 0; i < offsets.size(); i++)
    {
        offsetPoints[i] = ofVec2f();
    }
}

//--------------------------------------------------------------
void ofApp::resizeRing(vector<ofVec2f> outer, vector<ofVec2f> inner)
{
    
    outerPoints.clear();
    innerPoints.clear();
    
    for (int i = 0; i < n; i++) {
        //plot the outer points of the ring
        ofVec2f a = ofVec2f( mDiameter * sin(ofMap(i, 0, n, 0, TWO_PI)),
                             mDiameter * cos(ofMap(i, 0, n, 0, TWO_PI)));
        outerPoints.push_back(a);
        
        //plot the inner points of the ring
        float strokeX = mThickness * sin(ofMap(i, 0, n, 0, TWO_PI));
        float strokeY = mThickness * cos(ofMap(i, 0, n, 0, TWO_PI));
        
        ofVec2f b = ofVec2f( a.x - strokeX,
                             a.y - strokeY);
        innerPoints.push_back(b);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::resizeRingRes(vector<ofVec2f> outer, vector<ofVec2f> inner)
{
    
    outerPoints.clear();
    innerPoints.clear();
    offsetPoints.clear();
    
    resizeRing(outer, inner);
    
    for (int i = 0; i < n; i++)
    {
        //set up the array of offsets applied to the points
        //this prevents out-of-index errors when the resolution changes
        ofVec2f c = ofVec2f();
        offsetPoints.push_back(c);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::setupAudio()
{
//    soundStream.printDeviceList();
    
    int bufferSize = 256;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(n, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
}

//--------------------------------------------------------------
void ofApp::updateAudio()
{
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value

    if( volHistory.size() >= n ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    
    // DS: why do you multiply it with -1 here?
    for (int i = 0; i < volHistory.size(); i++)
    {
        volHistory[i] *= -1;
    }

}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++)
    {
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.3;
    smoothedVol += 0.7 * curVol;
    
    bufferCounter++;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_TAB)
    {
        //bring up the GUI when tab is pressed
        drawGui  = !drawGui;
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
