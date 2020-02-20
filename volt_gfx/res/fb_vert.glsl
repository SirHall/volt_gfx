#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 gfxPVM;
uniform mat4 gfxProjection;

out vec3 vertPos;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertPos     = position;
    texCoord    = aTexCoord;
}