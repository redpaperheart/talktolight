#version 120

uniform sampler2D tex0;
uniform float questionHighlight;
uniform vec2 windowSize;

varying vec4 vColor;
varying vec2 vTexCoord;

float luminance(vec3 rgb)
{
    // Algorithm from Chapter 10 of Graphics Shaders.
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    return dot(rgb, W);
}

void main()
{
    vec3 tex = texture2D(tex0, gl_FragCoord.xy / windowSize).rgb;
    gl_FragColor.rgb = mix(tex, vec3(luminance(tex)), questionHighlight);
    gl_FragColor.a = vColor.a;
}

