/*	To use more than one light source in the scene we put the lighting calculations into GLSL 
	functions. They are similar to C functions; we have a function name, return type, and must 
	declare a prototype at the top of the code if the function hasn't been declared yet before
	the main function. Each type of light will have its own function. 
	
	When using multiple lights in a scene the approach is usually as follows: we have a single 
	color vector that represents the fragment's color output. For each light, the light's 
	contribution to the fragment is added to this output color vector. Each light in the scene will
	calculate its individual impact and contribute that to the final output color. A general 
	structure would look like: */


out vec4 fragColor;

void main() {
	// define output color value
	vec3 output = vec3(0.0);
	// add the directional light's contribution
	output += someFunctionToCalculateDirectionalLight();
	// do the same for point lights
	for (int i = 0; i < nr_of_point_lights; i++)
		output += someFunctionToCalculatePointLight();
	// add other lights as well, like spotlights
	output += someFunctionToCalculateSpotLight();
	
	fragColor = vec4(output, 1.0);
}

/* Directional Light

	First we need to set the required variables that we minimally need for a directional light 
	source. THe variables can be stored in a struct, defined as a uniform. */

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

// The dirLight uniform can then be passed to a function with the following prototype:

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);

// Declare the prototype somewhere above the main function

// The function requires a DirLight struct and two other vectors for its computation:

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiniess);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse * specular);
}

// Point Light

struct PointLight {
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

/* 	We use a pre-procesor directive to define the number of point lights in the scene. Then we
	use this constant to create an array of PointLight structs. The prototype is: */

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Now the function itself:

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position, fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
							   light.quadratic * (distance * distance));
	
	// combine
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

// Now we can put it all together in the main function!

void main() {
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	
	// directional lighting
	vec3 result = calcDirLighting(dirLight, norm, viewDir);
	
	// point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
	
	fragColor = vec4(result, 1.0);
}

/*	Each light type adds its contribution to the resulting output color until all light sources
	are processed. 
	
	How can we set the uniform values of the point lights since the point light uniform is 
	an array of structs? It works like setting the uniforms of a single struct, only this time
	we have to define the appropriate index when querying the uniform's location. */

lightingShader.setFloat("pointLights[0].constant", 1.0f);
			


