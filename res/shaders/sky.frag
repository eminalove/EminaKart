#version 440 core

in vec2 vTexture;

//Take in the texture
layout(binding = 0) uniform sampler2D texSampler;


//Colour of the fragment
out vec4 color;
void main(){
    //Get the colour from the texture and apply the normal shading.
    color = texture(texSampler, vTexture);
}

