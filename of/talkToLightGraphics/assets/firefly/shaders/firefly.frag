#version 120

uniform vec2 windowSize;
varying vec4 vColor;
varying float vBlur;

float ramp( float edge0, float edge1, float x )
{
    return clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 );
}

float softCircle(in vec2 uv, in float rad, float softness)
{
    rad *= 0.5;

    float len  = length(uv - vec2(0.5));
    float edge = ramp(rad - softness * 0.5, rad, len);

    return 1.0 - edge;
}

void main()
{
    float circ = softCircle(gl_TexCoord[0].st, 0.8, vBlur); // for point sprites
    float vignette = 1.0 - 0.5 * distance(gl_FragCoord.xy, windowSize * 0.5) / windowSize.x;

    gl_FragColor.rgb = vColor.rgb * circ;
    gl_FragColor.a = vColor.a * circ * vignette;

    // gl_FragColor = vec4(vignette, 1.0 - vignette, 0.0, 1.0);
//    gl_FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);

}

