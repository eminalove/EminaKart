#version 330 core

layout (location = 0) in vec2 aPosition;    //Vertex position

out vec2 vTexCoord;

uniform float scale;
uniform mat4 model;                         //Transformation matrix
uniform mat4 projection;                    //Projection matrix
uniform mat4 view;                          //View Matrix
uniform float aspect;
uniform float rotation;

void main(){
    vTexCoord = vec2((aPosition.x+1.0)/2.0, 1 - (aPosition.y+1.0)/2.0); //Allow for interpolation
    
    //Transform the vertex position by the transformation matrix
    mat4 mvp = projection * view * model;
    vec4 centrePos = mvp * vec4(0.0,0.0,0.0, 1.0);
    vec2 newPos = vec2(aPosition.x * cos(rotation) - aPosition.y * sin(rotation), aPosition.x * sin(rotation) + aPosition.y * cos(rotation));
    gl_Position = vec4(centrePos.x+newPos.x*aspect*scale, centrePos.y+newPos.y*scale, centrePos.z,centrePos.w);
    //gl_Position = vec4(aPosition,0.0,1.0);
}