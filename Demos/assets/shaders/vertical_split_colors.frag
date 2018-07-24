#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform  float   uniform_split_value;

void main()
{
    if (gl_FragCoord.x <  uniform_split_value)
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);

    //gl_FragColor = (gl_FragCoord.x <  uniform_split_value) ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(0.0, 1.0, 0.0, 1.0);
}
