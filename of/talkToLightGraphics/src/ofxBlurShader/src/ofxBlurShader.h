/*
 *  ofxBlurShader.h
 *
 *  Created by Rick Companje on 25-10-10.
 *  updated on 2/22/18 for of009 and raspPi by Shuvashis Das
 *  Based on the blurShader example by Theo
 *
 */

#pragma once 

#include "ofMain.h"
#include "../../LightMode.h" // to get the custom asset path for shader - SD


class ofxBlurShader : public LightMode{
public:
	
    ofxBlurShader(); 
    
	void setup(float w, float h);
	void begin(int amount=5, int iterations=2);
	void end();
	void draw(float x=0, float y=0, float w=0, float h=0);
    
    int iterations, amount;
    bool enabled;
    
    ofFbo fbo1,fbo2;
    ofShader hShader, vShader;

private:
    bool initialized;
};

