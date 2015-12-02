 #version 130

in vec3 vPosition;

//out vec4 gl_Position;
out vec4 fColor;

uniform mat4 ctm;
uniform float time;

uniform bool calcXWave,calcZWave,calcCenterWave;
float r,g,b;

//shader constants
const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main()
{
    // calculate distance of the current vertex from the center of the mesh
    float distance = length(vPosition);
    
    //create a sin function using the distance, multiply frequency and add
    //the elapsed time
    float y = 0;
    if(calcCenterWave)
    {
        float distance =length(vPosition);
        y=y+amplitude*sin(-PI*distance*4+time);
        
    }
    if(calcZWave)
    {
        float distance =length(abs(vPosition.z));
        y=y+amplitude*sin(-PI*distance*3+time);
        
    }
    if(calcXWave)
    {
        float distance =length(abs(vPosition.x));
        y=y+amplitude*sin(-PI*distance*2+time);
        
    }
    //convert to clip space
    gl_Position = ctm * vec4( vPosition.x, y, vPosition.z, 1.0 );
    
    r = sin(-PI * distance *4 + time);
    g = sin(-PI * distance *4 + time+2*PI/3);  
    b = sin(-PI * distance *4 + time+4*PI/3);  
    fColor = vec4(r, g, b, 1);  
    
}
