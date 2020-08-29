#version 330 core

in vec3 gfxPos;
in vec2 gfxUV;

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