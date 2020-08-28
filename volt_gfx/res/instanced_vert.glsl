#version 330 core

in vec3 gfxPos;
in vec2 gfxUV;
in mat4 gfxTransform;

uniform mat4 gfxPV;

out vec2 texCoord;

void main()
{
    gl_Position = gfxPV * gfxTransform * vec4(gfxPos, 1.0);
    texCoord    = gfxUV;
}