#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform sampler2D uniform_texture;
uniform  vec2     uniform_resolution_div;

vec4 InvertColor(vec4 color)
{
    return vec4(1.0) - color;
}

void main()
{
    vec2 coords  = vec2(gl_FragCoord.x*uniform_resolution_div.x, gl_FragCoord.y*uniform_resolution_div.y);
    vec4 color   = texture2D(uniform_texture, coords);
    gl_FragColor = InvertColor(color);
}
