#version 130

// input attributes
in vec3 vPosition;
in vec3 vColor;

// output attributes.  Because gl_Position is implicitly defined
// by glsl, it isn't necessary for us to define it here, but let's
// do it anyway just to make sure we see what's going on.
out vec4 fColor;
//out vec4 gl_Position;

// a uniform 4x4 matrix
uniform mat4 ctm;

void main()
{
  gl_Position = ctm * vec4(vPosition, 1.0);
  fColor = vec4(vColor, 1.0);	
}