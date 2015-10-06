#version 130

in vec3 vPosition;

//out vec4 gl_Position;

uniform mat4 ctm;
uniform float time;

//shader constants
const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main()
{
  //get the Euclidean distance of the current vertex from the center of the mesh
  float distance = length(vPosition);  

  //create a sin function using the distance, multiply frequency and add
  //the elapsed time
  float y = amplitude*sin(-PI*distance*frequency+time);

  //convert to clip space

  gl_Position = ctm * vec4( vPosition.x, y, vPosition.z, 1.0 );
}
