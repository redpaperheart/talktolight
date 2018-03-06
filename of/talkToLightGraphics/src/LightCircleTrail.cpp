//
//  LightCircleTrail.cpp
//  talkToLightGraphics
//
//  Created by Andrew Cotter on 7/20/17.
//
//

#include "LightCircleTrail.h"

LightCircleTrail::LightCircleTrail(ofVec2f pos, float rad)
{
    mPosition = pos;
    mRadius = rad;
    mOffset = ofVec2f(0);
    mModel = Model::getInstance();
}

LightCircleTrail::~LightCircleTrail()
{
    mHistory.clear();
}

void LightCircleTrail::update()
{
    mHistory.insert(mHistory.begin(), mPosition);
    while (mHistory.size() > 10) {
        mHistory.pop_back();
    }
}

void LightCircleTrail::draw()
{
    if (mHistory.empty()) return;
    
    ofFill();
    ofSetColor(mColor);
    ofDrawCircle( mHistory.front(), mRadius );
    
    drawTrail();
    
    ofDrawCircle( mHistory.back(), mRadius);
}

void LightCircleTrail::drawTrail()
{
    
    // only draw the trail if it's longer than 2 pixels
    std::vector<ofPoint> history;
    history.push_back(mHistory[0]);
    int lastAddedIndex = 0;
    for( int i = 1; i < mHistory.size(); i++){
        if( mHistory[lastAddedIndex].distance(mHistory[i]) > 2.0){
            lastAddedIndex = i;
            history.push_back( mHistory[i] );
        }
    }
    if( history.size() < 2 ) return;
    
    ofPolyline poly = ofPolyline(history);
    
    ofPath trail;
    //trail.setCurveResolution(128);
    //trail.setColor(mColor);
    trail.setColor(ofColor(255,0,0));
    trail.setStrokeWidth(4);

    
    for ( int i = 0; i < history.size(); i++ )
    {
        if (i == 0)
        {
            trail.moveTo(history[i] + poly.getNormalAtIndex(i) * mRadius);
        }
        else
        {
            trail.lineTo(history[i] + poly.getNormalAtIndex(i) * mRadius);
        }
        
    }
    
    for ( int i = history.size()-1; i > -1; i-- )
    {
        if ( i == history.size()-1 )
        {
            trail.lineTo(history[i] - poly.getNormalAtIndex(i) * mRadius);
        }
        else
        {
            trail.lineTo(history[i] - poly.getNormalAtIndex(i) * mRadius);
        }
    }
    
    trail.close();
    trail.simplify();
    trail.draw();
    
}
