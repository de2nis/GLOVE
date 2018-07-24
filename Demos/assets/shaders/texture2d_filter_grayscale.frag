#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform sampler2D uniform_texture;
uniform  vec2     uniform_resolution_div;

vec4 GrayScaleColor(vec4 color)
{
    float  avg = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    return vec4(vec3(avg), 1.0);
}

void main()
{
    vec2 coords  = vec2(gl_FragCoord.x*uniform_resolution_div.x, gl_FragCoord.y*uniform_resolution_div.y);
    vec4 color   = texture2D(uniform_texture, coords);
    gl_FragColor = GrayScaleColor(color);
}
