#version 330 core

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST 0.01

out vec4 FragColor;

in vec2 fragCoord;

uniform vec2 iResolution;
uniform float iTime;

float getDist(vec3 p)
{
	vec4 sphere = vec4(0, 1, 6, 1);

	float sphereDist = length(p - sphere.xyz) - sphere.w;
	float planeDist = p.y;

	float minDist = min(sphereDist, planeDist);
	
	return minDist;
}

float rayMarch(vec3 rayOrigin, vec3 rayDirection)
{
	float distanceOrigin = 0.;

	for(int i = 0; i < MAX_STEPS; i++)
	{
		vec3 p = rayOrigin + rayDirection * distanceOrigin;
		float distanceFromScene = getDist(p);
		distanceOrigin += distanceFromScene;
		if(distanceOrigin > MAX_DIST || distanceFromScene < SURF_DIST) break;
	}
	return distanceOrigin;
}

void main()
{
	vec2 uv = fragCoord / iResolution.xy;
	
	vec3 color = vec3(0);

	vec3 rayOrigin = vec3(0, 1, 0);
	vec3 rayDirection = normalize(vec3(uv.x, uv.y, 1));

	float d = rayMarch(rayOrigin, rayDirection);

	d /= 6.8;

	color = vec3(d);

	FragColor = vec4(color, 1.0);
}