#version 330 core

//Take in an interpolated version of the position
in vec3 vPosition;

uniform float time;

//Colour of the fragment
out vec4 color;
float getCol(vec2 offset){
    float val = sin(20.0 * vPosition.x + offset.x) * sin(20.0 * vPosition.y + offset.y);
    return val;
}
void main(){
    
    color = vec4(getCol(vec2(time,time)),getCol(vec2(time/2.0,time*-1.1)),getCol(vec2(time*-1.4,time/2.3)),1.0);
}

