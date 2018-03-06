#version 120

//attribute vec4 position;

//uniform mat4 modelViewProjectionMatrix;

void main(){
//    gl_Position = modelViewProjectionMatrix * gl_Vertex;
    gl_Position = ftransform();
}
