#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec3 a_Normal;

out vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_Position, 1.0f);
 	normal = mat3(modelMatrix) * a_Normal;
}