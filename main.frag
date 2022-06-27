#version 330 core

#define MAX_STEPS 1000
#define MAX_DIST 100.
#define SURF_DIST 0.01
#define PI 3.14159265359

out vec4 FragColor;

in vec2 fragCoord;

uniform vec2 iResolution;
uniform float iTime;
uniform float iDeltaTime;
uniform float scale;
uniform float exponent;
uniform int iterations;
uniform vec2 iLook;
uniform vec2 iKey;

float foldingLimit = 0.5;
float minRadius2 = 1.0;
float fixedRadius2 = 1.0;
float r = 2.0;

mat3 rotateX(float theta) 
{
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}

mat3 rotateY(float theta) 
{
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

mat3 rotateZ(float theta) 
{
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
}

// Identity matrix.
mat3 identity() 
{
    return mat3(
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1)
    );
}

void sphereFold(inout vec3 z, inout float dz) 
{
	float r2 = dot(z, z);
	if (r < minRadius2) 
    { 
		float temp = (fixedRadius2 / minRadius2);
		z *= temp;
		dz *= temp;
	} 
    else if (r2 < fixedRadius2) 
    { 
		float temp =(fixedRadius2 / r2);
		z *= temp;
		dz *= temp;
	}
}

void boxFold(inout vec3 z, inout float dz) 
{
	z = clamp(z, -foldingLimit, foldingLimit) * 2.0 - z;
}


float torus(vec3 p, vec2 r)
{
    float x = length(p.xz) - r.x;
    return length(vec2(x, p.y)) - r.y;
}

float mandelBulb(vec3 pos) 
{
    float power = exponent;
    power = clamp(abs(sin(iTime * 0.5) * 5), 1.1, 6.0);
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < iterations ; i++) {
		r = length(z);
		if (r > 100) break;
		
		// convert to polar coordinates
		float theta = acos(z.z / r);
		float phi = atan(z.y, z.x);
		dr =  pow(r, power - 1.0) * power * dr + 1.0;
		
		// scale and rotate the point
		float zr = pow(r, power);
		theta = theta * power;
		phi = phi * power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5 * log(r) * r / dr;
}

float mandelTest(vec3 z)
{
	vec3 offset = z;
	float dr = 1.0;
	for (int n = 0; n < iterations; n++) 
    {
		boxFold(z,dr);
		sphereFold(z,dr);    
 		
        z=scale*z + offset; 
        dr = dr*abs(scale)+1.0;
	}
	float r = length(z);
	return r/abs(dr);
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
    return (length(z)) * pow(scale, -float(n)) * 1;
}

float getDist(vec3 p)
{
    vec4 s = vec4(0, 1, 6, 1);
    float sphereDist = length(p - s.xyz) - s.w;
    float planeDist = p.y;
    
    //float td = mandelBulb(p);
    //float dist = td;
    float dist = min(sphereDist, planeDist);
    return dist;
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
    uv.x *= iResolution.x / iResolution.y;

    vec3 col = vec3(0);
    
    vec3 ro = vec3(0, 2.0, -5.0);

    vec2 mov = iKey * 0.1;

    ro = vec3(mov.x, 2.0, mov.y);

    vec3 rd = normalize(vec3(uv.x, uv.y, 1));

    rd *= rotateX(iLook.y);
    rd *= rotateY(iLook.x);
  
    float dist = rayMarch(ro, rd);

    if (dist > MAX_DIST)
    {
        col = vec3(0.30, 0.36, 0.60) + (rd.y * 0.7);
    }
    else
    {
        vec3 p = ro + rd * dist;
    
        float diffuse = getLight(p);
        col = vec3(diffuse);
    }

    col = pow(col, vec3(0.4545));
    FragColor = vec4(col, 1.0);
}