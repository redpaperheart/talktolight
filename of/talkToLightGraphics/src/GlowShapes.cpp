//
//  GlowShapes.cpp
//  talkToLightGraphics
//
//  Created by Adrià Navarro López on 3/2/18.
//
//

#include "GlowShapes.h"
#include "Model.h"

bool GlowShapes::sDebug = false;
ofShader GlowShapes::sGlowCircShader;
ofShader GlowShapes::sGlowRingShader;
ofShader GlowShapes::sGlowRectShader;
ofShader GlowShapes::sStripeShader;
ofPlanePrimitive GlowShapes::sPlane;
ofFloatColor GlowShapes::sGlowColor;

void GlowShapes::reloadShaders()
{
    auto assetPath = Model::getInstance()->assetPath;
    
#ifdef TARGET_OPENGLES
    sGlowCircShader.load(assetPath + "/common/shaders_es2/passThrough.vert", assetPath + "/common/shaders_es2/glowCirc.frag");
    sGlowRingShader.load(assetPath + "/common/shaders_es2/passThrough.vert", assetPath + "/common/shaders_es2/glowRing.frag");
    sGlowRectShader.load(assetPath + "/common/shaders_es2/passThrough.vert", assetPath + "/common/shaders_es2/glowRect.frag");
#else
    sGlowCircShader.load(assetPath + "/common/shaders/passThrough.vert", assetPath + "/common/shaders/glowCirc.frag");
    sGlowRingShader.load(assetPath + "/common/shaders/passThrough.vert", assetPath + "/common/shaders/glowRing.frag");
    sGlowRectShader.load(assetPath + "/common/shaders/passThrough.vert", assetPath + "/common/shaders/glowRect.frag");
#endif
    sGlowCircShader.bindDefaults();
    sGlowRingShader.bindDefaults();
    sGlowRectShader.bindDefaults();
    
    sPlane.set(1, 1, 2, 2);
}

void GlowShapes::setGlowColor(const ofFloatColor &color)
{
    sGlowColor = color;
}

void GlowShapes::drawCirc(float x, float y, float r, float glow)
{
    float totalRad = r + glow;
    float circRad = r / totalRad;
    float rectPixelSize = totalRad * 2.0f;
    
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(rectPixelSize, rectPixelSize);
 
    sGlowCircShader.begin();
    sGlowCircShader.setUniform1f("circRad", circRad);
    sGlowCircShader.setUniform1f("rectPixelSize", rectPixelSize);
    sGlowCircShader.setUniform4f("glowColor", sGlowColor);
    sPlane.draw();
    sGlowCircShader.end();
    
    ofPopMatrix();
    
    if (sDebug) {
        ofPushStyle();
        ofSetColor(255, 50);
        ofNoFill();
        ofDrawCircle(x, y, r);
        ofDrawRectangle(x - totalRad, y - totalRad, rectPixelSize, rectPixelSize);
        ofPopStyle();
    }
    
}

void GlowShapes::drawRing(float x, float y, float innerRad, float outerRad, float glowRad)
{
    float totalRad = outerRad + glowRad;
    float normOuterRad = outerRad / totalRad;
    float normInnerRad = innerRad / totalRad;
    float rectPixelSize = totalRad * 2.0f;
    
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(rectPixelSize, rectPixelSize);
    
    sGlowRingShader.begin();
    sGlowRingShader.setUniform1f("circRad", normOuterRad);
    sGlowRingShader.setUniform1f("inRad", normInnerRad);
    sGlowRingShader.setUniform1f("rectPixelSize", rectPixelSize);
    sGlowRingShader.setUniform4f("glowColor", sGlowColor);
    sPlane.draw();
    sGlowRingShader.end();
    
    ofPopMatrix();
    
    if (sDebug) {
        ofPushStyle();
        ofSetColor(255, 50);
        ofNoFill();
        ofDrawCircle(x, y, innerRad);
        ofDrawCircle(x, y, outerRad);
        ofDrawRectangle(x - totalRad, y - totalRad, rectPixelSize, rectPixelSize);
        ofPopStyle();
    }
    
}

void GlowShapes::drawRect(float x, float y, float w, float h, float glow)
{
    ofVec2f totalSize(w + glow, h + glow);
    ofVec2f rectSize = ofVec2f(w, h) / totalSize;
    
    ofPushMatrix();
    ofTranslate(x + w * 0.5, y + h * 0.5);
    ofScale(totalSize);
    
    sGlowRectShader.begin();
    sGlowRectShader.setUniform2f("totalPixelSize", totalSize);
    sGlowRectShader.setUniform2f("rectPixelSize", ofVec2f(w, h));
    sGlowRectShader.setUniform2f("rectSize", rectSize);
    sGlowRectShader.setUniform2f("glowSize", ofVec2f(1.0) - rectSize);
    sGlowRectShader.setUniform4f("glowColor", sGlowColor);
    sPlane.draw();
    sGlowRectShader.end();
    
    ofPopMatrix();
    
    if (sDebug) {
        ofPushStyle();
        ofSetColor(255, 50);
        ofNoFill();
        ofDrawRectangle(x, y, w, h);
        ofDrawRectangle(x - 0.5 * glow, y - 0.5 * glow, w + glow, h + glow);
        ofPopStyle();
    }
}
