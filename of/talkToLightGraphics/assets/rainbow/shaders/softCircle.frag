#version 120

varying vec4 vColor;
varying vec2 vTexCoord;

float expStep( float x, float k, float n )
{
    return exp( -k * pow(x,n) );
}

float expStep2( float edge0, float edge1, float x, float k, float n )
{
    float tmp = clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 );
    return 1.0 - exp( -k * pow(tmp,n) );
}

float ramp( float edge0, float edge1, float x )
{
    return clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 );
}

float softCircle(in vec2 uv, in float rad, float softness)
{
    rad *= 0.5;

    float len  = length(uv - vec2(0.5));
    float normlen = clamp(1.0 - 2.0 * len, 0.0, 1.0);

    // float edge = ramp(rad - softness * 0.5, rad, len);
    // float edge = smoothstep(rad - softness * 0.5, rad, len);
    // float edge = expStep2( rad - softness * 0.5, rad, len, 4.0, 2.0);
    float edge = expStep(normlen, 3.0, 6.0);

    return 1.0 - edge;
}

void main()
{
    gl_FragColor.rgb = vColor.rgb * softCircle(vTexCoord, 1.0, 1.0) * vColor.a;
    gl_FragColor.a = 1.0;// softCircle(vTexCoord, 1.0, 1.0) * vColor.a;
}

