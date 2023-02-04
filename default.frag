#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;



// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;


// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.40f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	FragColor =  vec4(0.8, 0.8, 0.8, 1) * lightColor * (ambient + diffuse);
	//FragColor =  vec4(0.1, 0.9, 0.1, 1) * lightColor * (diffuse + ambient + specular);
}