#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform  vec4    uniform_color;

void main()
{
    gl_FragColor = uniform_color;
}
