#version 100

#ifdef GL_ES
  precision mediump float;
#endif

#define GAMMA         2.2
#define GAMMA_1       1.0/GAMMA

uniform sampler2D uniform_texture;
uniform  vec2     uniform_resolution_div;

vec4 GammaCorrection(vec4 color)
{
    return vec4(pow(color.r, GAMMA_1), pow(color.g, GAMMA_1), pow(color.b, GAMMA_1), color.a);
}

void main()
{
    vec2 coords  = vec2(gl_FragCoord.x*uniform_resolution_div.x, gl_FragCoord.y*uniform_resolution_div.y);
    vec4 color   = texture2D(uniform_texture, coords);
    gl_FragColor = GammaCorrection(color);
}
