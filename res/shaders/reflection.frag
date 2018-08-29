#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
	vec3 I = normalize(Position - cameraPos);	    
	
	float ratio = 1.00 / 1.52;
    
    	vec3 R = reflect(I, normalize(Normal));
   	//vec3 R = refract(I, normalize(Normal), ratio);

	
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
	//FragColor = vec4((Normal.x + 1.0f) / 2.0f, (Normal.y + 1.0f) / 2.0f, (Normal.z + 1.0f) / 2.0f, 1.0f);
	      
}