#version 100

#ifdef GL_ES
  precision mediump float;
#endif

uniform sampler2D uniform_texture;
uniform  vec2     uniform_resolution_div;

vec4 SobelColor(vec2 coords)
{
    float x = uniform_resolution_div.x;
    float y = uniform_resolution_div.y;

    vec2 c;
    c = vec2(coords.x, coords.y + y);
    vec4 top         = texture2D(uniform_texture, c);
    c = vec2(coords.x, coords.y - y);
    vec4 bottom      = texture2D(uniform_texture, c);
    c = vec2(coords.x - x, coords.y);
    vec4 left        = texture2D(uniform_texture, c);
    c = vec2(coords.x + x, coords.y);
    vec4 right       = texture2D(uniform_texture, c);
    c = vec2(coords.x - x, coords.y + y);
    vec4 topLeft     = texture2D(uniform_texture, c);
    c = vec2(coords.x + x, coords.y + y);
    vec4 topRight    = texture2D(uniform_texture, c);
    c = vec2(coords.x - x, coords.y - y);
    vec4 bottomLeft  = texture2D(uniform_texture, c);
    c = vec2(coords.x + x, coords.y - y);
    vec4 bottomRight = texture2D(uniform_texture, c);

    vec4 sx = -topLeft - 2.0 * left - bottomLeft + topRight   + 2.0 * right  + bottomRight;
    vec4 sy = -topLeft - 2.0 * top  - topRight   + bottomLeft + 2.0 * bottom + bottomRight;
    return sqrt(sx * sx + sy * sy);
}

void main()
{
    vec2 coords  = vec2(gl_FragCoord.x*uniform_resolution_div.x, gl_FragCoord.y*uniform_resolution_div.y);
    gl_FragColor = SobelColor(coords);
}
