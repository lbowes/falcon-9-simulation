#version 330 core

out vec4 outputColour;
in vec3 colour;

void main(){	
	outputColour = vec4(colour, 0.5f);	
}