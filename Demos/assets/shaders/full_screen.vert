#version 100

attribute vec2 v_posCoord_in;
attribute vec2 v_texCoord_in;

varying   vec2 v_texCoord_out;

void main()
{
    v_texCoord_out = v_texCoord_in;
    gl_Position    = vec4(v_posCoord_in, 0.0, 1.0);
}
