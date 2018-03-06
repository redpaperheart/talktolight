#version 120

// attribute vec4 position;
// attribute vec4 color;
// attribute vec4 normal;
// attribute vec2 texcoord;

varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
    vColor = gl_Color;
    vTexCoord = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}
