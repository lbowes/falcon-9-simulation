#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out vec2 texCoord;
out vec3 normal;
out vec3 toLight;
out vec3 toCamera;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	vec4 worldPosition = modelMatrix * vec4(a_Position, 1.0f);
	
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	texCoord = a_TexCoord;
 	normal = mat3(modelMatrix) * a_Normal;
	toLight = vec3(-0.5f, 0.5f, -0.5f);
	toCamera = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz - worldPosition.xyz;
}