#version 330 core
#define M_PI 3.1415926535897932384626433832795
#define MAX_DIST 10
#define MAX_STEPS 1000
#define MIN_DIST 0.001

in vec3 vertPos;
in vec2 texCoord;

uniform vec4      u_Color;
uniform sampler2D gfxTex0;
uniform sampler2D gfxTex1;
uniform float     t;
uniform float     dt;
uniform float     ratio;
uniform vec3      camPos;
uniform mat4      gfxPV;

out vec4 color;

// Magnitude
float Mag(vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float GetDist2(vec3 pos)
{
    float sphereDist = Mag(pos - vec3(0.0, 1.0, 0.0)) - 1.0;
    float planeDist  = pos.y;
    return min(sphereDist, planeDist);
}

// Get distance to scene (Mandelbulb)
float GetDist(vec3 pos)
{
    int   FRACTAL_IT = 100;
    float ESCAPE     = 100.0;
    float POWER      = 8;

    vec3  z  = pos;
    float dr = 1.0;
    float r  = 0.0;
    for (int i = 0; i < FRACTAL_IT; i++)
    {
        r = Mag(z);
        if (r > ESCAPE)
            break;

        // convert to polar coordinates
        float theta = acos(z.z / r) + (t / 30.0);
        float phi   = atan(z.y, z.x);
        dr          = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        // scale and rotate the point
        float zr = pow(r, POWER);
        theta    = theta * POWER;
        phi      = phi * POWER;

        // convert back to cartesian coordinates
        z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        z += pos;
    }
    return 0.5 * log(r) * r / dr;
}

vec3 GetNormal(vec3 pos)
{
    float dist = GetDist(pos);
    vec2  e    = vec2(0.01, 0.0);
    return normalize(dist - vec3(GetDist(pos - e.xyy), GetDist(pos - e.yxy),
                                 GetDist(pos - e.yyx)));
}

vec3 GetColor(vec3 pos)
{
    // return hsv2rgb(vec3(length(pos) * 3, 1.0, 1.0));
    return texture(gfxTex1, vec2((1.0 - (length(pos) * 0.90)) * 3.0, 1.0)).xyz * 2;
}

// Raymarch from position
vec3 Raymarch(vec3 origin, vec3 dir)
{
    float totalDist = 0.0;
    float minDist = MAX_DIST;
    vec3  pos       = origin;
    int   i         = 0;
    for (i = 0; i < MAX_STEPS; i++)
    {
        float dist = GetDist(pos);
        if(dist < minDist)
            minDist = dist;
        totalDist += dist;
        pos += dist * dir;
        if (dist < MIN_DIST || totalDist >= MAX_DIST)
            break;
    }
    return vec3(totalDist, i, minDist);
}

float GetLight(vec3 pos, vec3 lightPos)
{
    vec3  lightDir      = normalize(lightPos - pos);
    vec3  normal        = GetNormal(pos);
    float diffuse       = dot(normal, lightDir) * 0.5 + 1.0;
    vec3  lightRayMarch = Raymarch(pos + (normal * MIN_DIST * 2.0), lightDir);
    if (lightRayMarch.x < length(lightDir - pos))
        diffuse *= (0.25);
    return diffuse;
}

vec3 GetReflections(vec3 pos, int subdivisions, vec3 lightPos, vec3 skyCol)
{
    float ambientStrength = 0.1;

    vec3  reflection = vec3(1.0);
    float totalSubs  = (subdivisions * subdivisions * subdivisions);

    for (int i = 1; i <= subdivisions; i++)
    {
        for (int j = 1; j <= subdivisions; j++)
        {
            for (int k = 1; k <= subdivisions; k++)
            {
                float increment = 2.0 / subdivisions;
                vec3  dir =
                    normalize(vec3(i * increment + 0.5, j * increment + 0.5,
                                   k + increment + 0.5));
                vec3 reflectionMarch = Raymarch(pos, dir);
                if (reflection.x + 1 >= MAX_DIST)
                {
                    // This is the background
                    reflection *= (skyCol * ambientStrength);
                }
                else
                {
                    // We hit something
                    // float hitLight =
                    //     GetLight(pos + dir * reflectionMarch.x, lightPos);
                    vec3 hitColor = GetColor(pos + dir * reflectionMarch.x);
                    reflection += (hitColor) / totalSubs;
                }
            }
        }
    }
    return reflection;
}

void main()
{
    vec2 uv  = texCoord * 2 - 1.0;
    vec3 dir = normalize(vec3(uv.x * ratio, uv.y, 1.0));

    vec3 lightPos = vec3(0.0, 5.0, -2.0);
    // vec3 skyCol   = vec3(135, 206, 235) / 255.0;
    vec3 skyCol = vec3(0.0);

    vec3 pos = vec3(0.0, 0.0, -2.0);

    vec3  march = Raymarch(pos, dir);
    float dist  = march.x;
    //  GetLight(uv + dir * dist, lightPos);
    vec3 col;
    if (dist + 1 < MAX_DIST)
    {
        col = 
        vec3(GetLight(pos + dir * dist, lightPos)) *
              GetColor(pos + dir * dist);

        // Apply reflections
        // float reflectionAlpha = 0.95;
        // col = col + (GetReflections(pos + dir * dist, 4, lightPos, skyCol) *
        //              (1 - reflectionAlpha));
    }
    else
    {
        col = skyCol;
        if(march.z < 0.025)
            col = vec3(1.0 - (march.z / 0.025));
    }
    color = vec4(col, 1.0);
}
