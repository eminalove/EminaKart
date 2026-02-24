#version 420 core

in vec2 textureCoords;

out vec4 outputColor;

uniform float blend;
uniform float time;
float hash( vec2 a )
{
    return fract( sin( a.x * 3433.8 + a.y * 3843.98 ) * 45933.8 );
}
float noise( vec2 U )
{
    vec2 id = floor( U );
          U = fract( U );
    U *= U * ( 3. - 2. * U );  

    vec2 A = vec2( hash(id)            , hash(id + vec2(0,1)) ),  
         B = vec2( hash(id + vec2(1,0)), hash(id + vec2(1,1)) ),  
         C = mix( A, B, U.x);

    return mix( C.x, C.y, U.y );
}
void main(){
    
    outputColor = vec4(1.0,1.0,1.0,0.0);
    float b = (blend+noise(textureCoords*4.0)/8.0)*2.0;
    float x = textureCoords.x + textureCoords.y;
    x = -((x-1.0)*(x-1.0))+1.0;
    if(x < b){
        outputColor = vec4(1.0, 0.867, 0.969,1.0);
    }
    if(x + 0.3 < b){
        outputColor = vec4(1.0, 0.757, 0.945,1.0);
    }
    if(x + 0.5 < b){
        outputColor = vec4(1.0, 0.647, 0.922,1.0);
    }
    if(x + 0.7 < b){
        outputColor = vec4(1.0, 0.529, 0.898,1.0);
    }
}