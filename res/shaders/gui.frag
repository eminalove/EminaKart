#version 420 core

in vec2 textureCoords;

out vec4 outputColor;

//The texture for the GUI elements
layout(binding = 0) uniform sampler2D guiTexture;
//Current subsprite
uniform int curSprite;
//How many subsprites
uniform int numSprites;
//Colour of the sprite
uniform vec4 color;


void main(){
    //Width of one subsprite
    float width = 1.0 / float(numSprites);
    //Get the X coordinate of the subsprite
    vec2 newTexCoords = vec2(textureCoords.x*width + width*float(curSprite), textureCoords.y);
    //Sample the texture and apply the colour
    
    outputColor = texture(guiTexture, newTexCoords);
    outputColor.a *= color.a;
    outputColor.rgb *= outputColor.a;
    outputColor.rgb *= color.rgb;
}

