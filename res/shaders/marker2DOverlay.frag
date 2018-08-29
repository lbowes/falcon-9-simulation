#version 330 core

out vec4 colour;

in vec2 texCoord;

uniform sampler2D markerTexture;

void main(){
	vec4 texColor = texture(markerTexture, texCoord);
    if(texColor.a < 0.5)
        discard;
	
	texColor.a *= 0.5f;
	colour = texColor;	
}