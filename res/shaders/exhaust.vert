//Credit to https://github.com/ashima/webgl-noise/blob/master/src/classicnoise2D.glsl for:
// - mod289()
// - permute()
// - taylorInvSqrt()
// - fade()
// - cnoise()

#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float u_nozzleDiameter_m;
uniform float u_throttleInRange_percent;
uniform float u_airPressure_percent;
uniform float u_time;
uniform vec3 u_ambientFlowDir;
uniform vec3 u_ambientPressureDir;

out vec4 colour;

vec3 mod289(vec3 x)
{
  	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
  	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  	return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
  	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec3 P)
{
  	vec3 Pi0 = floor(P); // Integer part for indexing
  	vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  	Pi0 = mod289(Pi0);
  	Pi1 = mod289(Pi1);
  	vec3 Pf0 = fract(P); // Fractional part for interpolation
  	vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  	vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  	vec4 iy = vec4(Pi0.yy, Pi1.yy);
  	vec4 iz0 = Pi0.zzzz;
  	vec4 iz1 = Pi1.zzzz;

  	vec4 ixy = permute(permute(ix) + iy);
  	vec4 ixy0 = permute(ixy + iz0);
  	vec4 ixy1 = permute(ixy + iz1);

  	vec4 gx0 = ixy0 * (1.0 / 7.0);
  	vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  	gx0 = fract(gx0);
  	vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  	vec4 sz0 = step(gz0, vec4(0.0));
  	gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  	gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  	vec4 gx1 = ixy1 * (1.0 / 7.0);
  	vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  	gx1 = fract(gx1);
  	vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  	vec4 sz1 = step(gz1, vec4(0.0));
  	gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  	gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  	vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  	vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  	vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  	vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  	vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  	vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  	vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  	vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  	vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  	g000 *= norm0.x;
  	g010 *= norm0.y;
  	g100 *= norm0.z;
  	g110 *= norm0.w;
  	vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  	g001 *= norm1.x;
  	g011 *= norm1.y;
  	g101 *= norm1.z;
  	g111 *= norm1.w;

  	float n000 = dot(g000, Pf0);
  	float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  	float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  	float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  	float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  	float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  	float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  	float n111 = dot(g111, Pf1);

  	vec3 fade_xyz = fade(Pf0);
  	vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  	vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  	float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  	return 2.2 * n_xyz;
}

float lerp(float a, float b, float x) {
	return a + (b - a) * clamp(x, 0.0f, 1.0f);
}

float alpha_distortion(vec3 position_exhaust, float alongFlame_percent) 
	//Calculates a value between 0.0f and 1.0f to multiply the existing transparency with
	//'uniqueSeed' must be unique for each vertex at a given layer (x or z position)
{
	const float speed = 20.0f;
	
	//float a = cnoise(vec2(position.x - u_time, alongFlame_percent) * speed) * 0.5f;            //A pseudo random alpha value that varies with time
	float a = cnoise(vec3(position_exhaust.x, alongFlame_percent - u_time * speed, position_exhaust.z)) * 0.5f;            //A pseudo random alpha value that varies with time

	return pow(a, 2.0) * 2.0;
}

vec4 getColour(vec3 position, float alongFlame_percent) 
	//'uniqueSeed' must be unique for each vertex at a given layer (x or z position)
{
	const vec3 
		start = vec3(1.0f, 0.0f, 0.26f),//vec3(0.992f, 1.0f, 0.996f),
		end = vec3(0.919f, 0.844f, 0.983f);

	float 
		vacuumIntensity = 0.05f,
		currentIntensity = lerp(vacuumIntensity, 1.0f, u_airPressure_percent),
		transparency = currentIntensity * pow(1.0f - alongFlame_percent, 2.0f - alongFlame_percent);

	transparency *= max(alpha_distortion(position, alongFlame_percent), pow(1.0f - alongFlame_percent, 3.0f));

	return vec4(start + (end - start) * alongFlame_percent, transparency);
}

float getLength() {
	const float 
		minLength = 45.0f, //45.0f
		maxLength = 80.0f; //80.0f
	
	//The throttle should not have a visual effect on the plume in a vacuum, it should be very large regardless.
	//At lower altitudes the throttle's effect is most visible.
	float currentMinLength = lerp(minLength, maxLength, 1.0f - u_airPressure_percent);

	return lerp(currentMinLength, maxLength, u_throttleInRange_percent);
}

float rL(float alongFlame_percent) 
	//A linearly interpolated radius from the nozzle radius to the current pressure radius at the end of the plume
{
	const float 
		exitRadius_SL = 0.0f,
		exitRadius_vac = 30.0f;

	float exitRadius_current = lerp(exitRadius_SL, exitRadius_vac, 1.0f - u_airPressure_percent);

	return lerp(u_nozzleDiameter_m, exitRadius_current, alongFlame_percent);
}

float rQ(float alongFlame_percent) 
	//A quadratic addition added to the linear radius to produce a curved 'inverted raindrop' (sea level) or 'parasol' shaped plume (vacuum)
{
	const float 
		size_SL = 0.3f,   //maximum size of addition at sea level
		size_vac = 20.0f, //maximum size of addition at vacuum
		t_SL = 0.03f,     //curve turning point position along the length of the flame at sea level
		t_vac = 1.0f;     //curve turning point position along the length of the flame at vacuum

	//Calculate the current location of the turning point
	float 
		size_current = lerp(size_SL, size_vac, 1.0f - u_airPressure_percent),
		t_current = lerp(t_SL, t_vac, 1.0f - u_airPressure_percent);

	//Addition increasing up to turning point
	if(alongFlame_percent <= t_current) {
		float x = alongFlame_percent / t_current;	
		return (1.0f - pow(1.0f - x, 3.0f)) * size_current;
	}
	//Addition constant beyond turning point
	else
		return size_current;
}

vec3 radial_distort(vec3 position_exhaust, float alongFlame_percent) {
	if(alongFlame_percent == 0.0f)
		return vec3(0.0f);
	else {
		//Sine wave
		//return normalize(position_exhaust) * (sin(alongFlame_percent * 20.0f - u_time * 20.0f) + 1.0f) / 2.0f * 2.0f;

		const float 
			speed = 20.0f,
			maxDistort = 10.0f;

		float distort = (cnoise(vec3(position_exhaust.x, u_time * speed, position_exhaust.z)) + 1.0f) / 2.0f * maxDistort;
			
		vec3
			pos_norm = normalize(vec3(position_exhaust.x, 0.0f, position_exhaust.z)),
			r = pos_norm * distort * pow(alongFlame_percent, 3.0f);

		return r;
	}
}

float getRadius(float alongFlame_percent) 
	//Total of rL() and rQ()
{
	if(alongFlame_percent == 0.0)
		return u_nozzleDiameter_m;
	else {
		return 
			rL(alongFlame_percent) +
			rQ(alongFlame_percent);
	}
}

vec3 AP_distort(float alongFlame_percent) 
	//Ambient pressure distortion
{
	if(alongFlame_percent == 0.0f)
		return vec3(0.0f);
	else {	
		//A 'power' factor representing the maximum displacement added by the ambient pressure			
		const float s = 40.0f;

		return normalize(-u_ambientPressureDir) * (1.0f - pow(1.0f - alongFlame_percent, 2.0)) * (1.0f - u_airPressure_percent) * s;
	}
}

void main() {
	float 
		x = abs(a_Position.y),
		radius = getRadius(x);

	vec3 position_exhaust;
	position_exhaust.x = a_Position.x * radius;
	position_exhaust.y = a_Position.y * getLength();
	position_exhaust.z = a_Position.z * radius;

	position_exhaust += AP_distort(x);
	position_exhaust += radial_distort(position_exhaust, x);

	//position += normalize(a_Position) * cnoise(vec2(time * 10.0, a_Position.y * 10.0));



	colour = getColour(position_exhaust, abs(a_Position.y));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position_exhaust, 1.0f);
}