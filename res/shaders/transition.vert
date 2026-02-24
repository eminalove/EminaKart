#version 330 core

layout (location = 0) in vec2 aPosition;    //Vertex position

out vec2 textureCoords;

void main(){
    textureCoords = vec2((aPosition.x+1.0)/2.0, 1 - (aPosition.y+1.0)/2.0); //Allow for interpolation
    
    gl_Position = vec4(aPosition,0.0,1.0);
}