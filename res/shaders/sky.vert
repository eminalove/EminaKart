#version 420 core

//Taking in the position from the buffer
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

//Passing it to the fragment shader
out vec3 vNormal;
out vec2 vTexture;

//Take in the projection matrix
uniform mat4 projection;
//Take in the view matrix;
uniform mat4 view;

void main(){
    //Allows for interpolation
    vTexture = aTexture;

    //Put the vertex into world space
    gl_Position = projection*view*4.0*vec4(aPosition,1.0);
}


