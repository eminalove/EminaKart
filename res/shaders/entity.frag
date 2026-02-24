#version 440 core

in vec3 vNormal;
in vec2 vTexture;

uniform vec4 colour;
uniform float time;

//Take in the texture
layout(binding = 0) uniform sampler2D texSampler;

//my imaginary sun
vec3 sunDir = vec3(0.4,1,0.6);

//Colour of the fragment
out vec4 color;
void main(){
    //Dot product between the normal and sun direction
    float dotProd = dot(normalize(vNormal),sunDir);
    
    //Get the colour from the texture and apply the normal shading.
    color = texture(texSampler, vTexture)*colour*max(min(dotProd*1.3,1.3),0.5);
    if(color.a < 0.1) discard;
}

