#version 330 core

layout (location = 0) in vec2 aPosition;    //Vertex position

out vec2 textureCoords;

uniform vec2 position;
uniform vec2 scale;

uniform float aspect;

void main(){
    //Get the UV coordinates from the vertex position
    textureCoords = vec2((aPosition.x+1.0)/2.0, 1 - (aPosition.y+1.0)/2.0);
    
    //Adjust for the screens aspect ratio
    vec2 newPos = aPosition;
    newPos.x = newPos.x * aspect;

    //Change to a more reasonable size
    newPos /= 8.0;

    //Set the final position
    gl_Position = vec4(position*vec2(aspect,1.0)/8.0+newPos*scale,0.0,1.0);
}

