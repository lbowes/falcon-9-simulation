#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out vec3 Normal;
out vec3 Position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    Normal = mat3(transpose(inverse(modelMatrix))) * a_Normal;
    Position = vec3(modelMatrix * vec4(a_Position, 1.0));
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_Position, 1.0);
}