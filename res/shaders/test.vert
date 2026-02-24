#version 330 core

//Taking in the position from the buffer
layout (location = 0) in vec3 aPosition;

//Passing it to the fragment shader
out vec3 vPosition;

void main(){
    //Allows for interpolation
    vPosition = aPosition;

    gl_Position = vec4(aPosition,1.0);
}


