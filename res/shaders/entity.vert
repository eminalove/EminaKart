#version 420 core

//Taking in the position from the buffer
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

//Passing it to the fragment shader
out vec3 vNormal;
out vec2 vTexture;

//Take in the model matrix
uniform mat4 model;
//Take in the projection matrix
uniform mat4 projection;
//Take in the view matrix;
uniform mat4 view;

void main(){
    //Allows for interpolation
    vTexture = aTexture;
    mat3 norm_matrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(norm_matrix * aNormal);
    vNormal = N;

    //Put the vertex into world space
    gl_Position = projection*view*model*vec4(aPosition,1.0);
}


