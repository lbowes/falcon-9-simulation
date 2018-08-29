#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	texCoord = a_TexCoord;
	gl_Position = modelMatrix * viewMatrix * projectionMatrix * vec4(a_Position, 1.0f);
}