#version 330 core

// layout(location = 0) in vec3 position;
// layout(location = 1) in vec2 aTexCoord;
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