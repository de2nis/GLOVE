#version 100

#ifdef GL_ES
  precision mediump float;
#endif

varying vec2      v_textCoord_out;

uniform sampler2D uniform_texture0;
uniform sampler2D uniform_texture1;

uniform float     uniform_mix_value;

void main()
{
    gl_FragColor    = texture2D(uniform_texture0, v_textCoord_out)*vec4(uniform_mix_value) +
                      texture2D(uniform_texture1, v_textCoord_out)*vec4(1.0-uniform_mix_value);
    gl_FragColor.a  = 0.5; // for transparecy (blending)
}
