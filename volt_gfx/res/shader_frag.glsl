#version 330 core

out vec4 color;

in vec3 vertPos;
in vec2 texCoord;

uniform vec4 u_Color;

uniform sampler2D gfxTex0;

void main()
{
    vec4 col = texture(gfxTex0, texCoord);
    if (col.a < 0.85)
        discard;
    gl_FragColor = col;
}