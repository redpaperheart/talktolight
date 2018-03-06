#version 120

#pragma include "../../noise/noise2D.glsl"

uniform float time;
uniform float realTime;
uniform float volumeAlpha;
uniform float globalAlpha;
uniform float centerAttraction;
uniform vec2 windowSize;

attribute float uniqueOffset;
attribute float	radius;

varying float vBlur;
varying vec4 vColor;

float perlin( float x, float y, float scale, float amp)
{
	const int octaves = 2;
    float result = 0.0;

    for( int i = 0; i < octaves; i++ ) {
        result += snoise( vec2(	x * scale, y * scale) ) * amp;
        x *= 4.0;
        y *= 4.0;
        // amp *= 0.5f * volumeAlpha;
        amp *= mix(0.25, 1.0, volumeAlpha);
    }

    return result / float(octaves);
}

void main()
{
    // pass varyings to fragment
    gl_PointSize  = radius * 1.1;

    //generate some noise values based on vertex position and the time value
	float t = time * 0.1;
	const float ampl = 300.0;
	const float scal = 1.0;
    vec4 pos = gl_Vertex;
    vec2 basePos = pos.xy;
    
    pos.xy = mix(pos.xy, vec2(0), centerAttraction * 0.8);
    pos.x += perlin( basePos.x - uniqueOffset, t, scal, ampl);
    pos.y += perlin( basePos.y + uniqueOffset, t, scal, ampl);
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;

    // calculate alpha
    vec4 col = gl_Color;
    float twinkle = sin(uniqueOffset + realTime * 1.0) * 0.4 + 0.6; // [0.2, 1]
    col.a *= twinkle;
    col.a += volumeAlpha;
    col.a *= globalAlpha;
    // col.a = min(col.a, 1.0);
    vColor = col;

    // test animating blur
    // vBlur = (uniqueOffset / 6.5) * 0.5;
    // vBlur = 0.9 - col.a * 0.5;
//    vBlur = (1.0 - twinkle) * 0.45;
    vBlur = 0.5 - twinkle * 0.4;//(1.0 - twinkle) * 0.45;
    // vBlur = 0.5 - twinkle * 0.5;

}
