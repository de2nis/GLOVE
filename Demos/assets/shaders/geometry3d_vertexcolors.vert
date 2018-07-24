#version 100

attribute vec3 v_posCoord_in;
attribute vec3 v_colorCoord_in;

varying   vec3 v_colorCoord_out;

uniform   mat4 uniform_mvp;

void main()
{
    v_colorCoord_out = v_colorCoord_in;
    gl_Position      = uniform_mvp * vec4(v_posCoord_in, 1.0);
}
