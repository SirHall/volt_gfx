#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 gfxPVM;

out vec3 vertPos;
out vec2 texCoord;

void main()
{
    gl_Position = gfxPVM * vec4(position, 1.0);
    vertPos     = position;
    texCoord    = aTexCoord;
}