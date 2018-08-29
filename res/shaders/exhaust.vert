#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float maxLength;
uniform float percentAirPressure;
uniform float time;
uniform vec3 ambientFlow;
uniform vec3 ambientPressureDir;

out vec4 colour;

vec4 getColour(float percentDownFlame) {
	vec4 
		start = vec4(1.0f, 0.0f, 0.0f, 1.0f),
		end = vec4(0.0f, 1.0f, 0.0f, 0.0f),
		outputColour = start + (end - start) * percentDownFlame;
	
	outputColour.a = percentAirPressure;

	return outputColour;
}

void main() {
	vec3 position = a_Position;
	position.y *= maxLength;

	position += ambientFlow * pow(a_Position.y, 2.0);





	colour = getColour(abs(a_Position.y));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}