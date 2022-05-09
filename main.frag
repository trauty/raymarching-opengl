#version 330 core

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST 0.01
#define PI 3.14159265359

out vec4 FragColor;

in vec2 fragCoord;

uniform vec2 iResolution;
uniform float iTime;
uniform float iDeltaTime;
uniform float scale;
uniform int iterations;
uniform vec2 iMouse;

mat2 rotate(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}


float torus(vec3 p, vec2 r)
{
    float x = length(p.xz) - r.x;
    return length(vec2(x, p.y)) - r.y;
}

float sierpinskiTetrahedron(vec3 z)
{
    float r;
    int n = 0;
    while (n < iterations) {
       if(z.x + z.y < 0) z.xy = -z.yx; // fold 1
       if(z.x + z.z < 0) z.xz = -z.zx; // fold 2
       if(z.y + z.z < 0) z.zy = -z.yz; // fold 3	
       z = z * scale - 2.0 * (scale - 1.0);
       n++;
    }
    return (length(z)) * pow(scale, -float(n));
}


float getDist(vec3 p)
{
    vec4 s = vec4(0, 1, 6, 1);
    float sphereDist = length(p - s.xyz) - s.w;
    //float planeDist = p.y;
    
    float td = sierpinskiTetrahedron(p);
    float dist = td;
    //dist = min(dist, td);
    return dist;
}


vec3 R(vec2 uv, vec3 p, vec3 l, float z) {
    vec3 f = normalize(l-p),
        r = normalize(cross(vec3(0,1,0), f)),
        u = cross(f,r),
        c = p+f*z,
        i = c + uv.x*r + uv.y*u,
        d = normalize(i-p);
    return d;
}


float rayMarch(vec3 ro, vec3 rd)
{
    float dO = 0.0;
    
    for(int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + rd * dO;
        float dS = getDist(p);
        dO += dS;
        if(dO > MAX_DIST || dS < SURF_DIST)
        {
            break;
        }
    }
    
    return dO;
}

vec3 getNormal(vec3 p)
{
    float d = getDist(p);
    vec2 e = vec2(0.01, 0);
    
    vec3 normal = d - vec3(getDist(p - e.xyy),
                           getDist(p - e.yxy),
                           getDist(p - e.yyx));
                           
    return normalize(normal);
}

float getLight(vec3 p)
{
    vec3 lightPos = vec3(0, 5, -6);
    vec3 lightVec = normalize(lightPos - p);
    vec3 normalVec = getNormal(p);
    
    float diffuse = clamp(dot(normalVec, lightVec), 0.0, 1.0);
    float d = rayMarch(p + normalVec * SURF_DIST * 2.0, lightVec);
    if(d < length(lightPos - p))
    {
        diffuse *= 0.1;
    }
    
    return diffuse;
}

void main()
{
	vec2 uv = fragCoord;

    vec2 m = iMouse.xy / iResolution.xy;


    vec3 col = vec3(0);
  
    
    vec3 ro = vec3(0, 0, -5.0);

    ro.yz *= rotate(-m.y + 0.4);
    ro.xz *= rotate(0.2 - m.x * PI);

    vec3 rd = R(uv, ro, vec3(0, 0, 0), 1.2);
  
    float dist = rayMarch(ro, rd);
    
    vec3 p = ro + rd * dist;
    
    float diffuse = getLight(p);
    col = vec3(diffuse);
    

    FragColor = vec4(col, 1.0);
}