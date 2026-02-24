#version 420 core

in vec2 textureCoords;

out vec4 outputColor;

uniform float blend;
uniform float blendFull;
uniform float time;

void main(){
    //outputColor = vec4(0.5,abs(sin(textureCoords.x*10.0+time)), abs(cos(textureCoords.y*10.0+time)),0.0);
    outputColor = vec4(0.0,0.0,0.0,0.0);
    float y = textureCoords.y*10.0;
    float ymod = mod(y,1.0);
    float bClamp = min(blend*1.2,1.0);
    if(blendFull<1.0){
        if(ymod<blend) outputColor = vec4(0.0,0.0,0.0,bClamp);
    }else{
        if(ymod>1.0-blend) outputColor = vec4(0.0,0.0,0.0,bClamp);
    }
    
}