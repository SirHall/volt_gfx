#version 330 core

out vec4 color;

in vec3 vertPos;
in vec2 texCoord;

uniform vec4 u_Color;

uniform sampler2D tex0;

// Simplex 2D noise
//
vec3 permute(vec3 x) { return mod(((x * 34.0) + 1.0) * x, 289.0); }

float snoise(vec2 v)
{
    const vec4 C  = vec4(0.211324865405187, 0.366025403784439,
                        -0.577350269189626, 0.024390243902439);
    vec2       i  = floor(v + dot(v, C.yy));
    vec2       x0 = v - i + dot(i, C.xx);
    vec2       i1;
    i1       = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i      = mod(i, 289.0);
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0)) + i.x +
                     vec3(0.0, i1.x, 1.0));
    vec3 m = max(
        0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
    m       = m * m;
    m       = m * m;
    vec3 x  = 2.0 * fract(p * C.www) - 1.0;
    vec3 h  = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);
    vec3 g;
    g.x  = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

vec3 clamp3(vec3 v)
{
    return clamp(v, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
}

float clamp1(float v) { return clamp(v, 0.0, 1.0); }

void main()
{
    vec2 suv = vec2(((vertPos * 0.5) + 0.5).xy);

    // Stage 1
    float baseMap  = snoise(suv * 100);
    float midLevel = 0.0;

    float zoneMap      = snoise(suv * 30);
    float zoneMidLevel = 0.0;

    float areaMap      = snoise(suv * 4);
    float areaMidLevel = 0.0;

    float chance = clamp1(baseMap + midLevel) * clamp1(zoneMap + zoneMidLevel) *
                   clamp1(areaMap + areaMidLevel);

    // Stage 2
    float contrastMap            = snoise(suv * 30);
    float contrastCenterMap      = snoise(suv * 50);
    float contrastCenterMidLevel = 0.8;
    float contrastStrength       = 1.0;

    chance = clamp1(chance + (sign(chance - clamp1(contrastCenterMap +
                                                   contrastCenterMidLevel)) *
                              contrastMap * contrastStrength * chance));

    color = vec4(chance, chance, chance, 1.0);
}