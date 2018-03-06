#version 120

varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
    vColor = gl_Color;
    vTexCoord = gl_MultiTexCoord0.xy;
    gl_Position = ftransform();
}
