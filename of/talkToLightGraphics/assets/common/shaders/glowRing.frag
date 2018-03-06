#version 120

uniform float circRad;
uniform float inRad;
uniform float rectPixelSize;
uniform vec4 glowColor;

varying vec4 vColor;
varying vec2 vTexCoord;

float ramp( float edge0, float edge1, float x )
{
    return clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 );
}

float gradCircle(in vec2 uv, float startRad, float endRad)
{
    startRad *= 0.5;
    endRad *= 0.5;
    
    float len  = length(uv - vec2(0.5));
    float edge = ramp(startRad, endRad, len);
    return 1.0 - edge;
}

float circle(in vec2 uv, in float rad)
{
    rad *= 0.5;
    
    float len  = length(uv - vec2(0.5));
    float aa = 3.0 / rectPixelSize;
    float edge = smoothstep(rad - aa, rad, len);
    
    return 1.0 - edge;
}

void main()
{
    float circ = circle(vTexCoord, circRad) - circle(vTexCoord, inRad);
    float glow = gradCircle(vTexCoord, circRad, 1.0) - gradCircle(vTexCoord, inRad - (1.0 - circRad), inRad);
    
    gl_FragColor = mix(glowColor, vColor, circ);
    gl_FragColor.a *= max(circ, glow);
}

