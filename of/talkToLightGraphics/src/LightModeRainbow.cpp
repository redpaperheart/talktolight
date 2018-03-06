
#include "Model.h"
#include "LightModeRainbow.h"

//--------------------------------------------------------------
void LightModeRainbow::setup( std::shared_ptr<LightCircle> assistantLightRef)
{
    // DS: pass the assistantLightRef to the super's setup function
    LightMode::setup( assistantLightRef );
    
    mPlane.set(1, 1, 2, 2);
    mSpotlights.resize(mMaxSpotLights);
    
    reloadShaders();
    resize(ofGetWindowWidth(), ofGetWindowHeight());
}

void LightModeRainbow::reloadShaders()
{

#ifdef TARGET_OPENGLES
    mCircleShader.load(mModel->assetPath + "/rainbow/shaders_es2/passThrough.vert", mModel->assetPath + "/rainbow/shaders_es2/softCircle.frag");
    mStripeShader.load(mModel->assetPath + "/rainbow/shaders_es2/passThrough.vert", mModel->assetPath + "/rainbow/shaders_es2/stripe.frag");
#else
    mCircleShader.load(mModel->assetPath + "/rainbow/shaders/passThrough.vert", mModel->assetPath + "/rainbow/shaders/softCircle.frag");
    mStripeShader.load(mModel->assetPath + "/rainbow/shaders/passThrough.vert", mModel->assetPath + "/rainbow/shaders/stripe.frag");
#endif
    mCircleShader.bindDefaults();
    mStripeShader.bindDefaults();
}

void LightModeRainbow::resize(int w, int h)
{
    // recreate fbo
    ofFbo::Settings settings;
    settings.width = w * mFboScale;
    settings.height = h * mFboScale;
    settings.useDepth = false;
    settings.useStencil = false;
    settings.internalformat = GL_RGB;
    settings.textureTarget = GL_TEXTURE_2D;
    mFbo.allocate(settings);
    
    cout << "Resizing and creating FBO with dimensions " << settings.width << "x" << settings.height << endl;
}

//--------------------------------------------------------------
void LightModeRainbow::update()
{
    //DS: only update as long as this mode is still active
    if (mCurState == STATE_OUT) return;

    LightMode::update();

    // save smoothed audio history
    float vol = mIsResponse ? mModel->mVolCur : 0.0f;
    float volScaled = ofMap(vol, 0.0, 0.02, 0.0, 1.0, true);
    float prev = mSoundHistory.empty() ? 0 : mSoundHistory.front();
    float smth = ofLerp(prev, volScaled, (volScaled - prev) > 0.0 ? 0.5 : 0.1);
    
    mSoundHistory.push_front(smth);
    
    if (mSoundHistory.size() > 400) {
        mSoundHistory.pop_back();
    }
    
    // update spotlights
    float now = ofGetElapsedTimef();
    float centerOut = volScaled;
    int maxSpotLights = ofMap(centerOut, 0.0f, 1.0f, float(mMaxSpotLights) * 0.4f, float(mMaxSpotLights), true);

    int numSpotlights = 0;
    int spotLightsAdded = 0;
    
    for (const auto &spot : mSpotlights) {
        numSpotlights += (int)spot.alive;
    }
    
    for (auto &spot : mSpotlights) {
        if (!spot.alive) {
            
            if (numSpotlights < maxSpotLights && spotLightsAdded < 1) {
                numSpotlights ++;
                spotLightsAdded ++;
                
                spot.alive = true;
                spot.t = 0.0f;
                spot.rad = ofRandom(1400, 1800);
                spot.saturation = ofRandom(128.0f, 255.0f);
                spot.startTime = now;
                
                if (centerOut > 0.25 && ofRandom(1.0f) < 0.8f ) {
                    spot.life = ofRandom(1.0f, 1.5f);
                    spot.pos = ofRandom(0.45f, 0.55f);
                    spot.vel = ofRandom(0.005f, 0.02f) * ofSign(spot.pos - 0.5f);
                }
                else {
                    spot.life = ofRandom(3.0f, 4.0f);
                    spot.pos = ofRandom(1.0f);
                    spot.vel = ofRandom(-0.005f, 0.005f);
                }
            }
        }
        else {
            spot.t = (now - spot.startTime) / spot.life;
            spot.pos += spot.vel;
            spot.rad -= 1.0f;
            spot.alive = spot.t < 1.0f;
            
            float screenRad = spot.rad / ofGetWindowWidth();
            spot.alive &= (spot.pos + screenRad > 0.0f) && (spot.pos - screenRad < 1.0f);
        }
    }
}

//--------------------------------------------------------------

void LightModeRainbow::draw()
{
    if(mCurState == STATE_OUT) return;
    
    // Draw smooth circles on FBO at half scale
    ofPushStyle();
    ofFill();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    mFbo.begin();
    ofPushView();
    ofViewport(0, 0, mFbo.getWidth(), mFbo.getHeight(), false);
    ofPushMatrix();
	ofScale(mFboScale, mFboScale);

    ofClear(0, 0, 0, 255);
    mCircleShader.begin();
    
    float y = ofGetWindowHeight() * 0.5f;
    float hueOffset = ofGetElapsedTimef() * 20.0f;
    
    for (const auto &spot : mSpotlights) {
        if (spot.alive) {
            float x = ofGetWindowWidth() * spot.pos;
            float alpha = 255.0f * 5.0f * spot.t * exp(1.0f - 5.0f * spot.t);
            float hue = fmodf(255.0f * spot.pos + hueOffset, 255.0f);
            float vol = mSoundHistory.empty() ? 0 : mSoundHistory.front();
            float rad = ofLerp(spot.rad, spot.rad * 0.6f, vol);
            
            ofSetColor(ofColor::fromHsb(hue, spot.saturation, 255.0f, alpha * mGlobalFade));
            drawPlane(x, y, rad, rad);
        }
    }
    
    ofPopMatrix();
    ofPopView();
    mCircleShader.end();
    mFbo.end();
    
    // Draw stripes using fbo as multiply texture
    float lineWidth = 50.0f;
    float padding = 20.0f;
    float paddedLine = lineWidth + padding;
    int numLines = std::ceil(ofGetWindowWidth() / paddedLine);
    float centerX = ofGetWindowWidth() * 0.5f;
    float centerY = ofGetWindowHeight() * 0.5f;
    float centerOffset = std::abs(float(numLines) * paddedLine * 0.5f - centerX) * 0.5f;
    float fadeWidth = mGlobalFade * mGlobalFade;
    
    ofEnableAlphaBlending();
    ofSetColor(255.0f);
    mStripeShader.begin();
    mStripeShader.setUniform1f("questionHighlight", mQuestionHighlight);
    mStripeShader.setUniform2f("windowSize", ofVec2f(ofGetWindowSize()));
    mStripeShader.setUniformTexture("tex0", mFbo.getTexture(), 0);
    
    for (int i = 0; i < numLines; i ++) {
        int volIdx = std::abs(numLines/2 - i) * 3;
        float vol = volIdx < mSoundHistory.size() ? mSoundHistory.at(volIdx) : 1.0f;
        float width = mGlobalFade * lineWidth * (1.0f - vol) * fadeWidth;
        float x = float(i) * paddedLine - centerOffset + 0.5f * lineWidth;
     
        drawPlane(x, centerY, width, ofGetWindowHeight());
    }
    
    mStripeShader.end();
    
    // debug
    if (mModel->bDebug) {
        mFbo.draw(0, 0, mFbo.getWidth() * 0.25f, mFbo.getHeight() * 0.25f);
        ofNoFill();
        ofDrawRectangle(0, 0, mFbo.getWidth() * 0.25f, mFbo.getHeight() * 0.25f);
    }
    ofPopStyle();
}

void LightModeRainbow::drawPlane(float x, float y, float w, float h)
{
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(w, h);
    mPlane.draw();
    ofPopMatrix();
}

void LightModeRainbow::animateIn()
{
    cout << "LightModeRainbow::animateIn" << endl;
    mCurState = STATE_IN;
    Tweenzor::add(&mGlobalFade, 0.0f, 1.0f, 0.0f, 2.0f, EASE_IN_OUT_QUAD);
    mIsResponse = false;
}

void LightModeRainbow::animateOut(float duration)
{
    LightMode::animateOut();
    cout << "LightModeRainbow::AnimateOut" << endl;
    Tweenzor::add(&mGlobalFade, mGlobalFade, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    Tweenzor::add(&mQuestionHighlight, mQuestionHighlight, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    LightMode::animateOut(duration);
    mIsResponse = false;
}

void LightModeRainbow::setQuestionState()
{
    cout << "LightModeRainbow::setQuestionState" << endl;
    Tweenzor::add(&mQuestionHighlight, mQuestionHighlight, 1.0f, 0.0f, 0.5f, EASE_IN_OUT_QUAD);
    mIsResponse = false;
}

void LightModeRainbow::setResponseState()
{
    cout << "LightModeRainbow::setResponseState" << endl;
    Tweenzor::add(&mQuestionHighlight, mQuestionHighlight, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    mIsResponse = true;
}

void LightModeRainbow::setNormalState()
{
    cout << "LightModeRainbow::setNormalState" << endl;
    Tweenzor::add(&mQuestionHighlight, mQuestionHighlight, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    mIsResponse = false;
}
