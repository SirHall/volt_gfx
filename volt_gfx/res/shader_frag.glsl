#version 330 core

out vec4 color;

in vec3 vertPos;
in vec2 texCoord;

uniform vec4 u_Color;

uniform sampler2D tex0;

void main()
{
    vec4 col = texture(tex0, texCoord);
    if (col.a < 0.85)
        discard;
    color = col;
    // color = (vertPos * 0.5) + 0.5;
}