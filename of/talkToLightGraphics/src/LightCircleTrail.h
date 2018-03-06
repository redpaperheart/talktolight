//
//  LightCircleTrail.hpp
//  talkToLightGraphics
//
//  Created by Andrew Cotter on 7/20/17.
//
//

#pragma once

#include "LightCircle.h"
#include "ofMain.h"
#include "Model.h"

class LightCircleTrail : public LightCircle {
public:
    LightCircleTrail(ofVec2f pos = ofVec2f(0), float rad = 0);
    ~LightCircleTrail();
    
    void update() override;
    void draw() override;
    void drawTrail();
    
protected:
//    ofPath mTrail;
    std::vector<ofPoint> mHistory;
     
};
