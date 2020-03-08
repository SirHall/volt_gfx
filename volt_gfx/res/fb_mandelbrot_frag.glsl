#version 330 core
#define M_PI 3.1415926535897932384626433832795

in vec3 vertPos;
in vec2 texCoord;

uniform vec4      u_Color;
uniform sampler2D tex0;
uniform float     t;
uniform float     dt;
uniform float     ratio;

float logInterp(vec2 pos, float i, float iter)
{
    // return (i + 1 - log2(log2(pos.x * pos.x + pos.y * pos.y)) / log2(2));

    // return (iter) - log2(log2(pos.x * pos.x + pos.y * pos.y)) / 2;

    float modulus = sqrt(pos.x * pos.x + pos.y * pos.y);
    return i + 1 - (log(log(modulus))) / log(2.0);
    return iter / i;
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    float scale = 1.0;
    int   iter  = 2000;

    vec2 z = vec2(0.0);
    // vec2 c = (texCoord - vec2(0.55 * ratio, 0.5)) * 3.0 / ((t * t) / 10);
    // vec2(0.2665859, -0.003356588)
    vec2 c =
        ((texCoord * vec2(ratio, 1.0)) / (pow((t + 1), (t + 1) / 10) / 100)) +
        vec2(-0.746, 0.1481643);

    int i;

    for (i = 1; i <= iter; i++)
    {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if ((x * x + y * y) > 10000.0)
            break;
        z.x = x;
        z.y = y;
    }
    // logInterp(z, float(i), float(iter))
    // sqrt(z.x * z.x + z.y * z.y)
    gl_FragColor = vec4(
        (i == iter) ? vec3(1.0)
                    //     : mix(vec3(0.0, 0.0, 0.0),
                    //           vec3(abs(sin(3 * t / 10)) / 100, abs(cos(5 * t
                    //           / 10)) / 100,
                    //                abs(sin(7 * t / 10)) / 100),
                    //           vec3(logInterp(z, float(i), float(iter)))),
                    // 1.0);
                    : hsv2rgb(vec3(t * -0.1, 0.0, 0.0) +
                              mix(vec3(0.0, 1.0, 1.0), vec3(0.01, 1.0, 1.0),
                                  vec3(logInterp(z, float(i), float(iter))))),
        1.0);
}
