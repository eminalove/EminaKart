#version 420 core

in vec2 textureCoords;

out vec4 outputColor;

uniform float blend;
uniform float time;

void main(){
    //outputColor = vec4(0.5,abs(sin(textureCoords.x*10.0+time)), abs(cos(textureCoords.y*10.0+time)),0.0);
    outputColor = vec4(0.0,0.0,0.0,0.0);
    float x = blend;
    float a = ((1.3*x-0.3)*(10*x-3.9)*(4.8*x-4)+5)/10.0;
    float dist = distance(textureCoords, vec2(0.5,0.5));

    if(dist>1.0-a) outputColor.a=1.0;
    else if(abs(dist-(1.0-a))<0.05)outputColor.a = 1.0-(abs(dist-(1.0-a))/0.05);
}