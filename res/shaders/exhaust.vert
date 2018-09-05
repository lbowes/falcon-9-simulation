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

vec4 mod289(vec4 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r) {
	return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float cnoise(vec2 P) {
  	vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  	vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  	Pi = mod289(Pi); // To avoid truncation effects in permutation
  	vec4 ix = Pi.xzxz;
  	vec4 iy = Pi.yyww;
  	vec4 fx = Pf.xzxz;
  	vec4 fy = Pf.yyww;

  	vec4 i = permute(permute(ix) + iy);

  	vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  	vec4 gy = abs(gx) - 0.5 ;
  	vec4 tx = floor(gx + 0.5);
  	gx = gx - tx;

  	vec2 g00 = vec2(gx.x,gy.x);
  	vec2 g10 = vec2(gx.y,gy.y);
  	vec2 g01 = vec2(gx.z,gy.z);
  	vec2 g11 = vec2(gx.w,gy.w);

  	vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  	g00 *= norm.x;
  	g01 *= norm.y;
  	g10 *= norm.z;
  	g11 *= norm.w;

  	float n00 = dot(g00, vec2(fx.x, fy.x));
  	float n10 = dot(g10, vec2(fx.y, fy.y));
  	float n01 = dot(g01, vec2(fx.z, fy.z));
  	float n11 = dot(g11, vec2(fx.w, fy.w));

  	vec2 fade_xy = fade(Pf.xy);
  	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  	return 2.3 * n_xy;
}

vec4 getColour(float alongFlame_percent) {
	const vec4 
		start = vec4(1.0f, 0.0f, 0.0f, 1.0f),
		end = vec4(0.0f, 0.0f, 1.0f, 0.0f);

	float 
		vacuumIntensity = 0.05f,
		currentIntensity = vacuumIntensity + (1.0f - vacuumIntensity) * u_airPressure_percent,
		transparency = currentIntensity * pow(1.0f - alongFlame_percent, 2.0f - alongFlame_percent);

	return vec4(vec3(start.xyz + (end.xyz - start.xyz) * alongFlame_percent), transparency);
}

float getLength() {
	const float 
		minLength = 45.0f,
		maxLength = 80.0f;
	
	//The throttle should not have a visual effect on the plume in a vacuum, it should be very large regardless.
	//At lower altitudes the throttle's effect is most visible.
	float currentMinLength = minLength + (maxLength - minLength) * (1.0f - u_airPressure_percent);

	return currentMinLength + (maxLength - currentMinLength) * u_throttleInRange_percent;
}

//A linearly interpolated radius from the nozzle radius to the current pressure radius at the end of the plume
float rL(float alongFlame_percent) {
	const float 
		exitRadius_SL = 0.0f,
		exitRadius_vac = 30.0f;

	float exitRadius_current = exitRadius_SL + (exitRadius_vac - exitRadius_SL) * (1.0f - u_airPressure_percent);

	return u_nozzleDiameter_m + (exitRadius_current - u_nozzleDiameter_m) * alongFlame_percent;
}

//A quadratic addition added to the linear radius to produce a curved 'inverted raindrop' (sea level) or 'parasol' shaped plume (vacuum)
float rQ(float alongFlame_percent) {
	const float 
		size_SL = 0.3f,   //maximum size of addition at sea level
		size_vac = 20.0f, //maximum size of addition at vacuum
		t_SL = 0.03f,      //curve turning point position along the length of the flame at sea level
		t_vac = 1.0f;     //curve turning point position along the length of the flame at vacuum

	//Calculate the current location of the turning point
	float 
		size_current = size_SL + (size_vac - size_SL) * (1.0f - u_airPressure_percent),
		t_current = t_SL + (t_vac - t_SL) * (1.0f - u_airPressure_percent);

	//Addition increasing up to turning point
	if(alongFlame_percent <= t_current) {
		float x = alongFlame_percent / t_current;	
		return (1.0f - pow(1.0f - x, 3.0f)) * size_current;
	}
	//Addition constant beyond turning point
	else
		return size_current;
}

float getRadius(float alongFlame_percent) {
	if(alongFlame_percent == 0.0)
		return u_nozzleDiameter_m;
	else {
		return 
			rL(alongFlame_percent) +
			rQ(alongFlame_percent);	
	}
}

//Ambient pressure distortion
vec3 AP_distort(float alongFlame_percent) {
	if(alongFlame_percent == 0.0f)
		return vec3(0.0f);
	else {	
		//A 'power' factor representing the maximum displacement added by the ambient pressure			
		const float s = 40.0f;

		return normalize(-u_ambientPressureDir) * (1.0f - pow(1.0f - alongFlame_percent, 2.0)) * (1.0f - u_airPressure_percent) * s;
	}
}

void main() {
	vec3 position = a_Position;
	position.y *= getLength();

	float x = abs(a_Position.y);

	vec3 radius = vec3(a_Position.x, 0.0, a_Position.z) * getRadius(x);
	position.x = radius.x;
	position.z = radius.z;

	position += AP_distort(x);

	//position += normalize(a_Position) * cnoise(vec2(time * 10.0, a_Position.y * 10.0));



	colour = getColour(abs(a_Position.y));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}