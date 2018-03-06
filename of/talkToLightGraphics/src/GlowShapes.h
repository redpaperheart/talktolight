//
//  GlowShapes.h
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 3/2/18.
//
//

#pragma once

#include "ofMain.h"

class GlowShapes {
  public:
    static void reloadShaders();
    static void drawCirc(float x, float y, float rad, float glow);
    static void drawRing(float x, float y, float innerRad, float outerRad, float glowRad);
    static void drawRect(float x, float y, float w, float h, float glow);
    static void setGlowColor(const ofFloatColor &color);
    
  private:
    static bool sDebug;
    static ofShader sGlowCircShader;
    static ofShader sGlowRingShader;
    static ofShader sGlowRectShader;
    static ofShader sStripeShader;
    static ofPlanePrimitive sPlane;
    static ofFloatColor sGlowColor;
};
