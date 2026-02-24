#version 420 core

in vec2 vTexCoord;

out vec4 outputColor;

layout(binding = 0) uniform sampler2D texSampler;

uniform float blend;
uniform float transition;
uniform int numTextures;
uniform int curTexture;
uniform float depth;
uniform float scale;

uniform int variants;
uniform int variant;


float noise(vec2 pos){
    return fract(sin(dot(pos,vec2(12.9898,78.233)))*43758.5453);
}

void main(){
    float width = 1.0 / float(numTextures);
    float varWidth = 1.0 / float(variants);
    vec2 newTexCoords = vec2(vTexCoord.x*width + width*float(curTexture), vTexCoord.y*varWidth + varWidth*float(variant));
    outputColor = texture(texSampler, newTexCoords);
    if(curTexture!=0){
        vec2 prevTexCoords = vec2(vTexCoord.x*width + width*float(curTexture-1), vTexCoord.y);
        vec4 lastCol = texture(texSampler, prevTexCoords);
        outputColor = mix(lastCol,outputColor,transition);
    }
    
    float newBlend = (1.0-blend*blend*blend*blend*blend) * (outputColor.a);
    outputColor.a = newBlend;
    if(outputColor.a > 1.0) outputColor.a = 1.0;
    outputColor *= 0.7;
    float newD = depth/(scale*2.0);
    if(newD <= 1.0) outputColor.a*=newD;
    /*if(newBlend < 1.0){
        float ns = noise(vTexCoord);
        if(ns>newBlend) discard;
    }*/
}