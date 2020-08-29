#version 330 core

in vec3 gfxPos;
in vec2 gfxUV;

uniform mat4 gfxPVM;

out vec3 vertPos;
out vec2 texCoord;

void main()
{
    gl_Position = gfxPVM * vec4(gfxPos, 1.0);
    vertPos     = gfxPos;
    texCoord    = gfxUV;
}