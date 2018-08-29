#version 330 core

out vec4 outputColour;

in vec2 texCoord;
in vec3 normal;
in vec3 toLight;
in vec3 toCamera;

uniform sampler2D ourTexture;

void main(){
	float reflectivity = 10.0f;
	float shineDamper = 20.0f;	
	bool normalColours = false;
	const float lightIntensity = 0.05f;
	const vec3 lightColour = vec3(0.1725490196078431, 0.4941176470588235, 0.7843137254901961);
	vec4 diffuse;

	if(normalColours)
		diffuse = vec4((normal.x + 1.0f) * 0.5f, (normal.y + 1.0f) * 0.5f, (normal.z + 1.0f) * 0.5f, 1.0f);
	else
		diffuse = vec4(/*texture(ourTexture, texCoord).rgb*/vec3(1.0f) * ((dot(normal, toLight)) + 1.0f) * 0.5f, 1.0f) + lightColour * lightIntensity;

	vec3 normalizedToCamera = normalize(toCamera);
	vec3 lightDirection = -toLight;	
	vec3 reflectedLightDirection = reflect(lightDirection, normal);
	float specularFactor = dot(reflectedLightDirection, normalizedToCamera);
	specularFactor = max(specularFactor, 0.0f);
	float dampedFactor = pow(specularFactor, shineDamper);	
	vec3 specularLighting = vec3(dampedFactor) * reflectivity * lightColour;

	//outputColour = diffuse + vec4(specularLighting, 1.0f);
	outputColour = vec4(1.0f);

	//temporary to simulate basic atmospheric colour on stage
	//outputColour = mix(outputColour, vec4(lightColour, 1.0f), 0.4f);
}