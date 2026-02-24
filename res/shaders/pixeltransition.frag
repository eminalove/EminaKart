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
    //outputColor = vec4(0.5,abs(sin(textureCoords.x*10.0+time)), abs(cos(textureCoords.y*10.0+time)),0.0);
    outputColor = vec4(0.0,0.0,0.0,0.0);
    float t = blend*10.0;
    vec2 newCoords = floor((textureCoords-time*0.4)*20.0*(t+0.1))/(20.0*(t+0.1));
    
    vec4 col = mix(vec4(1.0, 0.89, 0.976,1.0),vec4(1.0, 0.584, 0.91,1.0),textureCoords.x);

    float n = noise(newCoords*10.0);
    if(n<blend) outputColor = col;
}