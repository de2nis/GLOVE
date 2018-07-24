#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform  vec4     uniform_color;
uniform float     uniform_radius;
uniform ivec2     uniform_resolution;
uniform ivec2     uniform_resolution_cen;

void main()
{
    float radius = uniform_radius * float (uniform_resolution.y);

    float d = length(vec2(uniform_resolution_cen) - gl_FragCoord.xy) - radius;
    float t = clamp (d, 0.0, 1.0);

    if (t == 0.0)
        gl_FragColor = uniform_color;
    else
        discard; // gets color from glClearColor()
}
