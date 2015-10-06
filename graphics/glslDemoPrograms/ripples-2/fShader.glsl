#version 130

uniform vec3 fColor;

void main()
{
 //gl_FragColor = vec4( 1, 0, 0, 1 );
  gl_FragColor = vec4( fColor, 1 );
}
