#include "ofxBlurShader.h"

ofxBlurShader::ofxBlurShader() {
    enabled = false;
    initialized = false;
}

void ofxBlurShader::setup(float w, float h) {
    
	fbo1.allocate(w,h);
	fbo2.allocate(w,h);

	//#ifdef TARGET_OPENGLES
	//	hShader.load(mModel->assetPath + "/blur/shadersES2/hShader");
	//	vShader.load(mModel->assetPath + "/blur/shadersES2/vShader");
	//	//shader.load(mModel->assetPath + "/shaders_gles/nLights.vert",mModel->assetPath + "/shaders_gles/nLights.frag");
	//#else
	//	if(ofIsGLProgrammableRenderer()){
	//		hShader.load(mModel->assetPath + "/blur/shadersGL3/hShader.vert", mModel->assetPath + "/blur/shadersGL3/hShader.frag");
	//		vShader.load(mModel->assetPath + "/blur/shadersGL3/vShader.vert", mModel->assetPath + "/blur/shadersGL3/vShader.frag");
	//	}else{
	//		hShader.load(mModel->assetPath + "/blur/shadersGL2/hShader.vert", mModel->assetPath + "/blur/shadersGL2/hShader.frag");
	//		vShader.load(mModel->assetPath + "/blur/shadersGL2/vShader.vert", mModel->assetPath + "/blur/shadersGL2/vShader.frag");
	//	}
	//#endif
	
	
	string vertexShader =
		"void main() {\
			gl_TexCoord[0] = gl_MultiTexCoord0;\
			gl_Position = ftransform();\
		}";
	
	
	string fragmentShaderHorizontal =
    	"uniform sampler2DRect src_tex_unit0;\
		uniform float blurAmount;\
		void main(void) {\
			vec2 st = gl_TexCoord[0].st;\
			vec4 color;\
			color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -4.0, 0.0));\
			color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -3.0, 0.0));\
			color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -2.0, 0.0));\
			color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -1.0, 0.0));\
			//color += 5.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount, 0));\
			color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 1.0, 0.0));\
			color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 2.0, 0.0));\
			color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 3.0, 0.0));\
			color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 4.0, 0.0));\
			color /= 25.0;\
			gl_FragColor = color;\
		}";
	
	// same but now for vertical neighbours

	string fragmentShaderVertical =
		"uniform sampler2DRect src_tex_unit0;\
		uniform float blurAmount;\
		void main(void) {\
			vec2 st = gl_TexCoord[0].st;\
			vec4 color;\
			color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 4.0));\
			color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 3.0));\
			color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 2.0));\
			color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 1.0));\
			//color += 5.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount) );\
			color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -1.0));\
			color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -2.0));\
			color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -3.0));\
			color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -4.0));\
			color /= 25.0;\
			gl_FragColor = color;\
		}";
	
    
    vShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderVertical);
    vShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    vShader.linkProgram();
    
    hShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderHorizontal);
    hShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    hShader.linkProgram();
    
    initialized = true;
    enabled = true;
	
}

void ofxBlurShader::begin(int amount, int iterations) {
    if (!initialized) ofLog(OF_LOG_ERROR, "ofxBlurShader::setup(w,h) needs to be called first");
    if (!enabled) return;
    
    this->amount = amount;
    this->iterations = iterations;
    
	//start 'recording' the user's drawing commands to the fbo
	fbo1.begin();
}

void ofxBlurShader::end() {
    if (!enabled) return;
        
	fbo1.end();

    for (int i=0; i<iterations; i++) {
    
        //apply horizontal blur to fbo1 and put the result in fbo2
        fbo2.begin();
        hShader.begin();
        hShader.setUniform1f("blurAmount", amount); 
		fbo1.draw(0,0);
        hShader.end(); 
        fbo2.end();
        
        //apply vertical blur to fbo2 and put this back in fbo1
        fbo1.begin(); 
        vShader.begin(); 
        vShader.setUniform1f("blurAmount", amount); 
        fbo2.draw(0,0); 
        hShader.end(); 
        fbo1.end();
    }
    
	//fbo1.clear();
	//fbo2.clear();
    ofSetColor(255);
    fbo1.draw(0,0);
}

