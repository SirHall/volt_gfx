#version 330 core

in vec3 vertPos;
in vec2 texCoord;

uniform vec4 u_Color;

uniform sampler2D tex0;

void main()
{
    float scale = 1.0;
    int   iter  = 100;

    vec2 z = (texCoord - vec2(0.37, 0.5)) * 4;

    int i;

    for (i = 0; i < iter; i++)
    {
        float x = (z.x * z.x - z.y * z.y) - z.x;
        float y = (z.y * z.x + z.x * z.y) - z.y;

        if ((x * x + y * y) > 4.0)
            break;
        z.x = x;
        z.y = y;
    }

    // gl_FragColor = texture1D(tex0, (i == iter ? 0.0 : float(i)) / 100.0);
    gl_FragColor =
        vec4(vec3(i == iter ? vec3(1.0)
                            : mix(vec3(0.0), vec3(64, 88, 199) / 255.0,
                                  float(i) / 5.0)),
             1.0);
}