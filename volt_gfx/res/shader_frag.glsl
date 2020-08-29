#version 330 core

in vec2 texCoord;

uniform vec4 u_Color;

uniform sampler2D gfxTex0;

out vec4 color;

void main()
{
    vec4 col = texture(gfxTex0, texCoord);
    if (col.a < 0.85)
        discard;
    color = col;
}