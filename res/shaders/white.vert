#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_Position, 1.0f);
}