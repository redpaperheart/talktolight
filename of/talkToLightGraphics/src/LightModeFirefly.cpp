
#include "LightModeFirefly.h"

//--------------------------------------------------------------
void LightModeFirefly::setup( std::shared_ptr<LightCircle> assistantLightRef)
{
    LightMode::setup( assistantLightRef );
    
    mBase.seed( std::random_device{}() );
    
    mFireflies.resize(mMaxFireflies);
    
    mPalette = {
        ofFloatColor(0.80, 0.86, 0.21),
        ofFloatColor(0.19, 0.51, 0.32),
        ofFloatColor(0.62, 0.91, 0.52),
        ofFloatColor(0.50, 0.94, 0.72),
        ofFloatColor(0.25, 0.84, 0.79),
    };
    
    reloadShaders();
    resize(ofGetWindowWidth(), ofGetWindowHeight());
    
    // setup vbo
    mPoints.resize(mMaxFireflies);
    mSizes.resize(mMaxFireflies);
    mColors.resize(mMaxFireflies);
    mUniqueOffsets.resize(mMaxFireflies);
    
    for (int i = 0; i < mMaxFireflies; i++) {
        mUniqueOffsets[i] = ofRandom(2.0f * M_PI);
    }
}

void LightModeFirefly::reloadShaders()
{
#ifdef TARGET_OPENGLES
    mShader.load(mModel->assetPath + "/firefly/shaders_es2/firefly.vert", mModel->assetPath + "/firefly/shaders_es2/firefly.frag");
#else
    mShader.load(mModel->assetPath + "/firefly/shaders/firefly.vert", mModel->assetPath + "/firefly/shaders/firefly.frag");
#endif
    mShader.bindDefaults();
}

void LightModeFirefly::resize(int w, int h)
{
}

//--------------------------------------------------------------
void LightModeFirefly::update()
{
    //DS: only update as long as this mode is still active
    if(mCurState == STATE_OUT) return;
    
    LightMode::update();
    
    // Smooth volume
    float vol = mIsResponse ? mModel->mVolCur * mModel->fireflyVolumeScaler * mModel->volumeScaler: 0.0f;
    float volScaled = ofMap(vol, 0.0, 0.02, 0.0, 1.0, true);
    mSmoothVol = ofLerp(mPrevVol, volScaled, (volScaled - mPrevVol) > 0.1 ? 0.5 : 0.3);
    mPrevVol = mSmoothVol;
    
    // Calculate volume effect on alpha
    mVolumeAlpha = ofLerp(0.0, 0.5, mSmoothVol);
    
    // Calculate volume effect on movement speed
    float now = ofGetElapsedTimef();
    float delta = now - mLastTime;
    mLastTime = now;
    mElapsedTime += delta * ofLerp(1.0, 3.0, mSmoothVol);
    
   // Update fireflies
    int firefliesAdded = 0;
    float halfWidth = ofGetWindowHeight() * 0.5f;
    
    for (auto &fly : mFireflies) {
        if (!fly.alive) {
            if (firefliesAdded < 1) {
                firefliesAdded ++;
                
                fly.alive = true;
                fly.t = 0.0f;
                fly.rad = ofRandom(15, 65);
                fly.saturation = ofRandom(128.0f, 255.0f);
                fly.startTime = now;
                fly.life = ofRandom(6.0f, 16.0f);
                fly.pos.x = randGaussian(-halfWidth, halfWidth); // TODO: update to gaussian distribution
                fly.pos.y = randGaussian(-halfWidth, halfWidth);
                fly.col = mPalette.at(ofRandom(mPalette.size()));
                fly.col.a = 0.0f;
            }
        }
        else {
            fly.t = (now - fly.startTime) / fly.life;
            fly.alive = fly.t < 1.0f;
     
            float alpha = 5.0f * fly.t * exp(1.0f - 5.0f * fly.t);
            fly.col.a = alpha;
        }
    }
    
    // Update vbo buffers
    for (int i = 0; i < mFireflies.size(); i++) {
        const auto &fly = mFireflies[i];
        mPoints[i] = ofVec3f(fly.pos);
        mSizes[i] = fly.rad;
        mColors[i] = fly.col;
    }
    
    // Upload the data to the vbo
    int total = mMaxFireflies;
    int radLoc = mShader.getAttributeLocation("radius");
    int offLoc = mShader.getAttributeLocation("uniqueOffset");

    mVbo.setVertexData(&mPoints[0], total, GL_DYNAMIC_DRAW);
    mVbo.setColorData(&mColors[0], total, GL_DYNAMIC_DRAW);
    mVbo.setAttributeData(radLoc, &mSizes[0], 1, total, GL_DYNAMIC_DRAW);
    mVbo.setAttributeData(offLoc, &mUniqueOffsets[0], 1, total, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------

void LightModeFirefly::draw()
{
    if(mCurState == STATE_OUT) return;
    
    // Draw fireflies with VBO for extra efficiency
    ofPushMatrix();
    ofTranslate(ofGetWindowSize() * 0.5f);
    ofRotate(ofGetElapsedTimef() * 1.0f);
    
    ofPushStyle();
    ofFill();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    
    mShader.begin();
    mShader.setUniform1f("time", mElapsedTime);
    mShader.setUniform1f("realTime", ofGetElapsedTimef());
    mShader.setUniform1f("volumeAlpha", mVolumeAlpha);
    mShader.setUniform1f("globalAlpha", mGlobalFade);
    mShader.setUniform1f("centerAttraction", mCenterAttraction);
    mShader.setUniform2f("windowSize", ofGetWindowSize());
    
    mVbo.draw(GL_POINTS, 0, mMaxFireflies);
    mShader.end();
    
    ofDisablePointSprites();
    ofDisableBlendMode();
        
    ofPopStyle();
    ofPopMatrix();
}

void LightModeFirefly::animateIn()
{
    cout << "LightModeFirefly::animateIn" << endl;
    mCurState = STATE_IN;
    Tweenzor::add(&mGlobalFade, 0.0f, 1.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    mIsResponse = false;
}

void LightModeFirefly::animateOut(float duration)
{
    cout << "LightModeFirefly::AnimateOut" << endl;
    Tweenzor::add(&mGlobalFade, 1.0f, 0.0f, 0.0f, duration, EASE_IN_OUT_QUAD);
    Tweenzor::add(&mCenterAttraction, mCenterAttraction, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    LightMode::animateOut(2.0);
    mIsResponse = false;
}

void LightModeFirefly::setQuestionState()
{
    cout << "LightModeFirefly::setQuestionState" << endl;
    Tweenzor::add(&mCenterAttraction, mCenterAttraction, 1.0f, 0.0f, 0.5f, EASE_IN_OUT_QUAD);
    mIsResponse = false;
}

void LightModeFirefly::setResponseState()
{
    cout << "LightModeFirefly::setResponseState" << endl;
    Tweenzor::add(&mCenterAttraction, mCenterAttraction, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    mIsResponse = true;
}

void LightModeFirefly::setNormalState()
{
    cout << "LightModeFirefly::setNormalState" << endl;
    Tweenzor::add(&mCenterAttraction, mCenterAttraction, 0.0f, 0.0f, 1.0f, EASE_IN_OUT_QUAD);
    mIsResponse = false;
}

float LightModeFirefly::randGaussian()
{
    return mNormDist( mBase );
}

float LightModeFirefly::randGaussian(float a, float b)
{
    return mNormDist( mBase ) * ( b - a ) + a;
}
