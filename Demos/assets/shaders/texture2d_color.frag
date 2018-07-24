#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform sampler2D uniform_texture;
varying vec2      v_texCoord_out;

void main()
{
// Render screen using a fixed texture
    gl_FragColor = texture2D(uniform_texture, v_texCoord_out);
}
