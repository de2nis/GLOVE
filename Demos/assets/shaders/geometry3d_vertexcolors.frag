#version 100

#ifdef GL_ES
  precision mediump float;
#endif

varying vec3      v_colorCoord_out;

void main()
{
    gl_FragColor    = vec4( v_colorCoord_out.r,
                            v_colorCoord_out.g,
                            v_colorCoord_out.b, 0.5);
}
