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
    
    drawTrail();
}

void LightCircleTrail::drawTrail()
{
    std::vector<ofPoint> history;
    history.push_back(mHistory[0]);
    ofRectangle rect(history[0].x, mHistory[0].y, 0, 0);
    
    int lastAddedIndex = 0;
    
    for( int i = 1; i < mHistory.size(); i++){
        if( mHistory[lastAddedIndex].distance(mHistory[i]) > 2.0){
            lastAddedIndex = i;
            history.push_back(mHistory[i]);
            rect.growToInclude(mHistory[i]);
        }
    }
    
    // only draw the trail if it's longer than 2 pixels
    if( history.size() < 2 ) return;
    
    ofPolyline poly = ofPolyline(history);
    ofPath trail;
    trail.setColor(mColor);
    trail.setStrokeWidth(4);

    trail.moveTo(history[0] + poly.getNormalAtIndex(0) * mRadius);
    
    for ( int i = 1; i < history.size(); i++ )
    {
        trail.lineTo(history[i] + poly.getNormalAtIndex(i) * mRadius);
    }
    
    for ( int i = history.size()-1; i > -1; i-- )
    {
        trail.lineTo(history[i] - poly.getNormalAtIndex(i) * mRadius);
    }
    
    trail.close();
    trail.simplify();
    trail.draw();
    
    ofDrawCircle( mHistory.back(), mRadius + 1);
    ofDrawCircle( mHistory.front(), mRadius + 1 );
    
    // Draw head
    // big red glow
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofVec2f ctr = rect.getCenter();
    float size = ofClamp(max(rect.getWidth(), rect.getHeight()) * 1.5f, 70.0f, 400.0f);
    float alpha = ofMap(size, 70.0f, 200.0f, 0.5, 0.25f, true);
    
    GlowShapes::setGlowColor(ofFloatColor(1.0, 0.25f, 0.0, alpha));
    GlowShapes::drawCirc(ctr.x, ctr.y, 0.0f, size);
    
    // small white glow when we're going fast
    if (size > 120.0f) {
        const auto &head = history.front();
        float headSize = ofMap(size, 120.0f, 200.0f, 10.0f, 50.0f, true);
        float headAlpha = ofMap(size, 120.0, 200.0f, 0.0f, 0.35f, true);
        GlowShapes::setGlowColor(ofFloatColor(1.0, 1.0, 1.0, headAlpha));
        GlowShapes::drawCirc(head.x, head.y, 0.0f, headSize);
    }
    ofDisableBlendMode();
    ofPopStyle();
}
