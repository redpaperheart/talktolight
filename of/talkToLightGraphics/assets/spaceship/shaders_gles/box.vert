
uniform mat4 modelViewProjectionMatrix;

attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;

varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
    vColor = color;
    vTexCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
