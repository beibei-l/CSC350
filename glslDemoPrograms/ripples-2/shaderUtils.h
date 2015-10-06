//
// Filename: shaderUtils.h
//
// Author: Douglas Harms
//
// This file contains functions useful for openGL programs that use glsl
// shaders.  It is based heavily on Ed Angel's code.
//
#include <iostream>
#include <fstream>
#include <GL/glut.h>

using namespace std;

// read in the file and return a null-terminated string from its contents
static char* readShaderSource( const char* filename ) {
  ifstream inFile;

  // open the file and return if it failed
  inFile.open( filename );
  if( !inFile ) {
    return NULL;
  }

  // get the length of the file
  inFile.seekg( 0L, ios_base::end );
  long size = inFile.tellg();

  // allocate buffer with enough space to hold the string and null char
  char* buf = new char[size + 1];

  // reset the file to the beginning, read in the contents, and close it
  inFile.seekg( 0L, ios_base::beg );
  inFile.read( buf, size );
  inFile.close();

  // add null char at end and return the null-terminated string
  buf[size] = '\0';
  return buf;
}


// create a glsl shader program from vertex and fragment shader files
GLuint initShaders( const char* vertexShaderFilename,
		    const char* fragmentShaderFilename ) {

  GLchar* source;

  // create the glsl program
  GLuint program = glCreateProgram( );

  // read in and compile the vertex shader
  source = readShaderSource( vertexShaderFilename );

  // exit if there were errors reading the file
  if( source == NULL ) {
    cerr << "Failed to read vertex shader from file " << vertexShaderFilename;
    cerr << endl;
    exit( EXIT_FAILURE );
  }

  // create the shader and compile it
  GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vShader, 1, (const GLchar **)&source, NULL );
  glCompileShader( vShader );

  // check to make sure it compiled correctly
  GLint compiled;
  glGetShaderiv( vShader, GL_COMPILE_STATUS, &compiled );
  if( !compiled ) {
    cerr << vertexShaderFilename << " failed to compile." << endl;
    GLint logSize;
    glGetShaderiv( vShader, GL_INFO_LOG_LENGTH, &logSize );
    char* logMsg = new char[logSize];
    glGetShaderInfoLog( vShader, logSize, NULL, logMsg );
    cerr << logMsg << endl;
    delete[] logMsg;
    exit( EXIT_FAILURE );
  }

  // attach vertex shader to the program
  glAttachShader( program, vShader );

  // read in and compile the fragment shader
  source = readShaderSource( fragmentShaderFilename );

  // exit if there were errors reading the file
  if( source == NULL ) {
    cerr << "Failed to read fragment shader from file " << fragmentShaderFilename;
    cerr << endl;
    exit( EXIT_FAILURE );
  }

  // create the shader and compile it
  GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fShader, 1, (const GLchar **)&source, NULL );
  glCompileShader( fShader );

  // check to make sure it compiled correctly
  glGetShaderiv( fShader, GL_COMPILE_STATUS, &compiled );
  if( !compiled ) {
    cerr << fragmentShaderFilename << " failed to compile." << endl;
    GLint logSize;
    glGetShaderiv( fShader, GL_INFO_LOG_LENGTH, &logSize );
    char* logMsg = new char[logSize];
    glGetShaderInfoLog( fShader, logSize, NULL, logMsg );
    cerr << logMsg << endl;
    delete[] logMsg;
    exit( EXIT_FAILURE );
  }

  // attach the fragment shader to the program
  glAttachShader( program, fShader);

  // link the programs and check for errors
  glLinkProgram( program );
  GLint linked;
  glGetProgramiv( program, GL_LINK_STATUS, &linked );
  if( !linked ) {
    cerr << " GLSL program failed to link." << endl;
    GLint logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize );
    char* logMsg = new char[logSize];
    glGetProgramInfoLog( program, logSize, NULL, logMsg );
    cerr << logMsg << endl;
    delete[] logMsg;
    exit( EXIT_FAILURE );
  }

  // return the program id
  return program;
}
  
