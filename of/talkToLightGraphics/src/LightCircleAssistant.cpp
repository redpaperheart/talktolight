//
//  LightCircleAssistant.cpp
//  talkToLightGraphics
//
//  Created by Daniel Scheibel on 5/31/17.
//
//

#include "LightCircleAssistant.h"

LightCircleAssistant::LightCircleAssistant(ofVec2f pos, float rad): LightCircle(pos, rad)
{
    
    mShape.setMode(ofPath::POLYLINES);
    
    //add points to each array
    for( int i = 0; i < mRingResolution; i++)
    {
        ofVec2f a = ofVec2f( sin(ofMap(i, 0, mRingResolution, 0, TWO_PI)),
                             cos(ofMap(i, 0, mRingResolution, 0, TWO_PI)));
        outerPoints.push_back(a);
        
        //set up the array of offsets applied to the points
        ofVec2f c = ofVec2f();
        offsetPoints.push_back(c);
    }
}

LightCircleAssistant::~LightCircleAssistant()
{
    Tweenzor::removeTween(&mRadius);
    auto pos = Tweenzor::getTween(&mCatTarget);
    if (pos) pos->remove();
    auto target = Tweenzor::getTween(&mCatTarget);
    if (target) target->remove();
}

void LightCircleAssistant::update()
{
    
    if (! mIsSoundReactive ) return;
    //if listening is on, add to the magnitude based on the scaled volume
    if( mModel->mVolScaled > mMinThreshold )
    {
        mMagnitude = mMagnitude + (mModel->mVolScaled * (mSoundMultiplier + mRadius));
        mMagnitude = mMagnitude * mResetSpeed;
    }else
    {
        mMagnitude = mMagnitude * mResetSpeed;
    }
    
    updateOffsets(offsetPoints, mMagnitude);
}

void LightCircleAssistant::draw()
{
    if (mRadius <= 0.0f) return;
    
    ofPushStyle();
    ofPushMatrix();
    
    if (! mIsSoundReactive ){
        if (mFilled || mRadius - mThickness <= 0) {
            ofFill();
            ofSetColor(mColor);
            ofDrawCircle(mPosition, mRadius);
        }
        else {
            ofPath circle;
            circle.setColor(mColor);
            circle.setCircleResolution(180);
            circle.arc(mPosition, mRadius - mThickness, mRadius - mThickness, 0, 360);
            circle.close();
            circle.arc(mPosition, mRadius, mRadius, 0, 360);
            circle.draw();
        }
    }else{
        if(mFilled){
            drawFilledWarp(outerPoints);
        }else{
            drawRing(outerPoints);
        }
    }
    
    ofPopMatrix();
    ofPopStyle();    
}


//draw a warped filled circle, same as ring, but without the inner path
void LightCircleAssistant::drawFilledWarp(vector<ofVec2f> outside){
    mShape.clear();
    if( mDrawDebug )
    {
        mShape.setStrokeColor(mColor);
        mShape.setStrokeWidth(2);
        mShape.setFilled(false);
    } else
    {
        mShape.setFillColor(ofColor(mColor));
        mShape.setFilled(true);
    }
    plotCurves(outerPoints, mRadius, offsetPoints);
    mShape.close();
    ofPushMatrix();
    ofTranslate( mPosition );
    mShape.draw();
    ofPopMatrix();
}
//--------------------------------------------------------------
void LightCircleAssistant::drawRing(vector<ofVec2f> points)
{
    //clear the previous points in the ofPath, if any
    mShape.clear();
    
    if( mDrawDebug )
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
    plotCurves(outerPoints, mRadius, offsetPoints);
    mShape.close();
    mShape.newSubPath();
    
    //draw the inside of the ring
    plotCurves(outerPoints, mRadius-mThickness, offsetPoints);
    mShape.close();
    
    //draw the ring around the desired center
    ofPushMatrix();
    ofTranslate( mPosition );
    mShape.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void LightCircleAssistant::plotCurves(vector<ofVec2f> points, float radius, vector<ofVec2f> offsets)
{
    
    for(int i = 0; i < points.size(); i ++)
    {
        ofVec2f point;
        if( i == 0 )
        {
            //start the path from [points.size()-1] first (this makes it smoother)
            mShape.curveTo( radius * points[points.size()-1] + (offsets[points.size()-1]));
        }
        
        //curve to the current point
        mShape.curveTo( radius * points[i] + offsets[i]);
        
        if( i == points.size()-1 )
        {
            //curve to index 0 and 1 again to make the path smooth
            mShape.curveTo(radius * points[0] + (offsets[0]));
            mShape.curveTo(radius * points[1] + (offsets[1]));
        }
        
        if (mDrawDebug)
        {
            ofPushMatrix();
            ofTranslate( mPosition );
            ofNoFill();
            ofSetColor(255, 255, 0);
            
            //unfilled circles the size of the offset clamp
            ofDrawCircle(radius * points[i], TWO_PI * (mRadius/mOffsetLimits) / mRingResolution);
            
            //circles where all the original points are
            ofFill();
            float temphue = float(i)/float(mRingResolution) * 255;
            ofSetColor(255-temphue, 0, temphue, 128);
            ofDrawCircle(radius * points[i], 10);
            
            //circles where all the offsetpoints are
            ofSetColor(0, 255, 0, 128);
            ofDrawCircle((offsets[i]), 5);
            
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void LightCircleAssistant::updateOffsets(vector<ofVec2f> offsets, int _scale)
{
    for( int i = 0; i < offsets.size(); i++)
    {
        if (mModel->mVolHistory.size() > i)
        {
            //re-map each value in volhistory
            ofVec2f delta = ofVec2f( sin( ofMap(mModel->mVolHistory[i], 0, 1, 0, TWO_PI) * _scale ),
                                     cos( ofMap(mModel->mVolHistory[i], 0, 1, 0, TWO_PI) * _scale ));
            delta.normalize();
            delta.x *= _scale;
            delta.y *= _scale;
            
            //Xeno formula:
            //newPosition = (1-pct) * targetPositon + pct * currentPositon
            offsetPoints[i].x = mXenoSpeed * delta.x + (1-mXenoSpeed) * offsetPoints[i].x;
            offsetPoints[i].y = mXenoSpeed * delta.y + (1-mXenoSpeed) * offsetPoints[i].y;
            
            //constrain the offsets based ont the mOffsetLimits
            offsetPoints[i].x = ofClamp(offsetPoints[i].x,
                                        -1 * TWO_PI * (mRadius/mOffsetLimits) / mRingResolution,
                                        TWO_PI * (mRadius/mOffsetLimits) / mRingResolution);
            offsetPoints[i].y = ofClamp(offsetPoints[i].y,
                                        -1 * TWO_PI * (mRadius/mOffsetLimits) / mRingResolution,
                                        TWO_PI * (mRadius/mOffsetLimits) / mRingResolution);
        }
    }
}

//--------------------------------------------------------------
//void LightCircleAssistant::resizeRing(vector<ofVec2f> outer, vector<ofVec2f> inner)
//{
//    outerPoints.clear();
//    innerPoints.clear();
//    
//    for (int i = 0; i < mRingResolution; i++) {
//        //plot the outer points of the ring
//        ofVec2f a = ofVec2f( mRadius * sin(ofMap(i, 0, mRingResolution, 0, TWO_PI)),
//                            mRadius * cos(ofMap(i, 0, mRingResolution, 0, TWO_PI)));
//        outerPoints.push_back(a);
//        
//        //plot the inner points of the ring
//        float strokeX = mThickness * sin(ofMap(i, 0, mRingResolution, 0, TWO_PI));
//        float strokeY = mThickness * cos(ofMap(i, 0, mRingResolution, 0, TWO_PI));
//        
//        ofVec2f b = ofVec2f( a.x - strokeX,
//                            a.y - strokeY);
//        innerPoints.push_back(b);
//    }
//}
