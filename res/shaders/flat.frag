#version 330 core

out vec4 colour;

in vec3 normal;

vec3 sunDirection = vec3(-0.5f, 0.5f, -0.5f);

void main(){
	//Greyscale normals	
	colour = vec4(((dot(normal, sunDirection)) + 1.0f) * 0.5, ((dot(normal, sunDirection)) + 1.0f) * 0.5f, ((dot(normal, sunDirection)) + 1.0f) * 0.5f, 1.0f);

	//Coloured with normals
	//colour = vec4((normal.x + 1.0f) * 0.5f, (normal.y + 1.0f) * 0.5f, (normal.z + 1.0f) * 0.5f, 1.0f);
}