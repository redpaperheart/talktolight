//
//  LightModeSpotlight
//  talkToLightGraphics
//
//  Created by Shuvashis Das on 2/21/18.
//

#include "LightModeSpaceship.h"


void LightModeSpaceship::setup( std::shared_ptr<LightCircle> assistantLightRef )
{
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );

	mBorder = true;
    
	reloadShaders();
	float heightTemp = 0.0;
	float widthTemp = 0.0;
	float t = 0.0;

	for (int i = 0; i < mNumBox ; i++) {
		LightRect b;
		b.mPlane.set( b.mWidth, b.mHeight);   ///dimensions for width and height in pixels
		widthTemp = b.mWidth;
		// this if statement makes the box relocate to the left most side and chages height to the next row 
		if((i > 0) && i%6 == 0){
			heightTemp += b.mHeight;
			t = 0.0;
			//cout << "heightTemp: " << heightTemp <<endl;
		}
		b.mPlane.setPosition( ( t * b.mWidth) , heightTemp, 0); /// position in x y z
		b.mPlane.setResolution(2, 2); /// this resolution (as columns and rows) is enough
		b.color = ofColor(0.0,0.0,255,255);
		mBoxes.push_back(b);
		t++;
	}
	
}

void LightModeSpaceship::reloadShaders()
{ 
    GlowShapes::reloadShaders();
}

void LightModeSpaceship::animateIn()
{
    // initialize anything mode specific here.
    cout << "LightModeSpotlight::animateIn" << endl;
    mCurState = STATE_IN;
    /*
	for(int i = 0; i < mBoxes.size(); i++){
		mBoxes[i].alphaTweenAnimateIn ();
	}
	*/
	onIdle();
}

void LightModeSpaceship::animateOut(float duration)
{
	mCurState = STATE_OUT_START;
    // reset, clean up anything when switching of this mode.
    cout << "LightModeEt::animateOut" << endl;
	for(int i = 0; i < mBoxes.size(); i++){
		mBoxes[i].alphaTweenAnimateOut();
	}
    LightMode::animateOut(1.0);
}
 
void LightModeSpaceship::update(){
    //DS: only update as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    
    //also don't update if state is going out
    if(mCurState == STATE_OUT_START) return;

	float interval;
	
	intervalGap =  1.50;//ofRandom(2.0, 3.0);
	
    switch(mModel->getCurState()){
        case Model::STATE_NONE:
            //nothing
            break;
        case Model::STATE_NORMAL:
			interval = ofGetElapsedTimef() - mPrevSeconds;  
			if(interval > intervalGap){
				mPrevSeconds = ofGetElapsedTimef();
				onIdle();
			}
            break;
        case Model::STATE_QUESTION:

			break;
        case Model::STATE_RESPONSE:
			interval = ofGetElapsedTimef() - mPrevSeconds;  
			if(interval > 1.0){
				mPrevSeconds = ofGetElapsedTimef();
				onResponse();
			}
            break;
    }
} 


void LightModeSpaceship::draw(){
    //DS: only draw as long as this mode is still active
    if(mCurState == STATE_OUT) return;

    ofPushMatrix();
    ofTranslate(mBoxes[0].mWidth/2, mBoxes[0].mHeight/2);

    //drawing a solid box first
    for (int i = 0; i < mBoxes.size(); i++) {
        auto &box = mBoxes[i];
        
        if(box.alphaMult>0){
            ofVec2f pos(box.mPlane.getPosition());
            ofVec2f size(box.mWidth, box.mHeight);
            ofSetColor( 0.0, 0.0, 255.0, 255.0f * box.alphaMult);
            GlowShapes::setGlowColor(ofFloatColor(0.0f, 0.0f, 1.0f, 0.25f * box.alphaMult));
            GlowShapes::drawRect(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, size.x, size.y, size.y * 2.0f);
        }
    }
		
		
	ofPopMatrix();
}

void LightModeSpaceship::onIdle(){

	for(int i = 0; i < mBoxes.size(); i++){
		if(Tweenzor::getTween(&mBoxes[i].alphaMult) == NULL){
			mBoxes[i].alphaMult = 0.0;
		}
	}
		
	//choosing to draw only upto 5 boxes
	int boxIdNum = ofRandom(1, 4);
	
	for(int k = 0; k < boxIdNum; k++){
		int boxId = ofRandom( 0, 15*6 );
		mBoxes[boxId].alphaTweenOn( float(k)/boxIdNum );
	}
}


void LightModeSpaceship::onQuestion(int mQuestionBar){	
	 
	// there are 15 rows, we pick one randomly to draw accross
	int rowId = 7;
	int columnId = 1;

	// offestting the column so that it does not start at the left most corner
	int count = 0;
	float interval;
	mPrevSeconds = 0.0;

	//picking boxes to draw
	mPrevSeconds = ofGetElapsedTimef();
	mBoxes[rowId + mQuestionBar +(5*rowId)].alphaMult = 1.0;
	if(mQuestionBar == 4)
		mQuestion = true;
}

void LightModeSpaceship::onResponse(){
	
	for(int i = 0; i < mBoxes.size(); i++){
		if(Tweenzor::getTween(&mBoxes[i].alphaMult) == NULL){
			mBoxes[i].alphaMult = 0.0;
		}
	}

	//choosing to draw only upto 5 boxes
	int boxIdNum = ofRandom(4, 8);
 
	mPrevSeconds = 0.0;
	int rowId;
	int columnId; 
	int count = 0.0;
	
	for(int k = 0; k < boxIdNum; k++){
		rowId = ofRandom( 5, 10 );
		columnId = ofRandom( 1, 5 );
		if( mModel->mBeat.snare() > 0.9){
			mBoxes[ rowId + columnId +(5*rowId) ].alphaTweenOnResponse(float(k)/boxIdNum);
		}
	}
}

void LightModeSpaceship::setQuestionState()
{
    cout << "LightModeEt::setQuestionState" << endl;
	mBorder = false;
	mPrevSeconds = 0.0;
	//initially drawing nothing boxes for question mode
	
	for(int i = 0; i < mBoxes.size(); i++){
		mBoxes[i].alphaMult = 0.0;
	}
	for(int b=0; b<4; b++){
		int offset=43;
		mBoxes[b+offset].mStayOn = true;
		mBoxes[b+offset].alphaTweenQuestionOn(1, b*.1);
	}
	
	mQuestion = false;
	mQuestionBar = 1;
}

void LightModeSpaceship::setResponseState()
{
    cout << "LightModeEt::setResponseState" << endl;
	for(int i = 0; i < mBoxes.size(); i++){
		mBoxes[i].mStayOn = false;
	}
	mPrevSeconds = 0.0;
}

void LightModeSpaceship::setNormalState()
{
    cout << "LightModeEt::setNormalState" << endl;
	mPrevSeconds = 0.0;
	for(int i = 0; i < mBoxes.size(); i++){
		mBoxes[i].mStayOn = false;
	}
	onIdle();
}

