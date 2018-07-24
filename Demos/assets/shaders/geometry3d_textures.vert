#version 100

attribute vec3 v_posCoord_in;
attribute vec2 v_textCoord_in;

varying   vec2 v_textCoord_out;

uniform   mat4 uniform_mvp;

void main()
{
    v_textCoord_out  = v_textCoord_in;
    gl_Position      = uniform_mvp * vec4(v_posCoord_in, 1.0);
}
