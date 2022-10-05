/*	A light source that casts light upon objects is called a light caster. 

	Directional Light
	
	When a light source is far away the light coming in from the source are close to parallel with 
	each other. When a light source is modeled to be infinitely far away it is called a directional
	light since all of its rays have the same direction. It is independent of the location of the
	light source.
	
	We can model direction light by defining a light direction vector instead of a position 
	vector: */

			struct Light {
				vec3 direction;
				
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};

			void main() {
				vec3 lightDir = normalize(-light.direction);
			}
	
			
			lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);

/*	The lightDir vector is negated because up until now the lighting calculations expect a light 
	direction to be from the fragment towards the light source, but typically a directional light
	is specified to be a global direction pointing from the light source. 
	
	Some people tend to prefer to keep all the vectors defined as vec4. When defining them as vec4,
	it is important to set the w component to 1.0 so translations and projections are properly 
	applied. However, when defining a direction vector as a vec4, we do not want translations to
	have an effect, so the w component is defined as 0.0. 
	
	Direction vectors can then be represented as (-0.3f, -1.0f, -0.3f, 0.0f). This can also 
	function as an easy check for light types. You could check if the w component is equal to 1.0
	to see that now we have a light's position vector and if w is equal to 0.0 then we have a 
	light's direction vector: */

			if (lightVector.w == 0.0) 
				// do directional light calculations
			else if (lightVector.w == 1.0)
				// do light calculations using the light's position
				
/*	Point Light

	Directional lights are great for global lights that illuminate the entire scene, but usually
	we also want several point lights scattered throughout the scene. A point light is a light
	source with a given position somewhere in the world that illuminates in all directions,
	where the light rays fade out over distance, like bulbs and torches. 
	
	To reduce the intensity of light over the distance a light ray travels is called 
	attenuation. Once way to reduce light intensity is to use a linear equation, which tends to 
	look fake due to the nature of light acting differently. The following formulate
	calculates an attenuation value based on a fragment's distance to the light source which
	is multiplied by the light's intensity vector.
	
				Fatt = 1.0 / (Kc + Kl * d + Kq * d^2)
				
	Where d represents the distance from the fragment to the light source. Then to calculate
	the attenuation value we define 3 terms: a constant Kc, a linear Kl, and a quadratic Kq.
	
			The constant is usually kept at 1.0 to make sure the denominator never gets smaller
			than 1.
			The linear term is multiplied with the distance value that reduces the 
			intensity in a linear fashion. 
			The quadratic term is multiplied with the quadrant of the distance anf sets a 
			quadratic decrease of intensity for the light source. The quadratic term will be
			much less significant compared to the linear term when the distance is small, but
			gets larger as the distance grows. 
			
	Setting the right values depends on factors such as the environment, the distance, the type
	of light, etc. It is a question of experience and a moderate amount of tweaking. There is a
	table that shows some of the values these forms could take:
	
	
				Distance		Constant		Linear		Quadratic
				7				1.0				0.7			1.8
				13				1.0				0.35		0.44
				20				1.0				0.22		0.20
				32				1.0				0.14		0.07	
				50				1.0				0.09		0.032
				65				1.0				0.07		0.017
				100				1.0				0.045		0.0075
				160				1.0				0.027		0.0028
				200				1.0				0.022		0.0019
				325				1.0				0.014		0.0007
				600				1.0				0.007		0.0002
				3250			1.0				0.0014		0.000007

	To implement attenuation we need to define the values in the fragment shader, best stored in
	the Light struct. */
				
				struct Light {
					vec3 position;
					vec3 ambient;
					vec3 diffuse;
					vec3 specular;
					
					float constant;
					float linear;
					float quadratic;
				};

				lightingShader.setFloat("light.constant", 1.0f);
				lightingShader.setFloat("light.linear", 0.09f);
				lightingShader.setFloat("light.quadratic", 0.032f);

/*	We need the distance to the light source for this equation. We can retrieve it by calculating
	the difference vector between the fragment and the light source and taking that resulting
	vector's length. We can use GLSL's built in length function. */

			float distance = length(light.position - fragPos);
			float attenuation = 1.0 / (light.constant + light.linear * distance + 
									   light.quadratic * (distance * distance));

/*	Then we include the attenuation value in the lighting calucations by multiplying the value
	with the ambient, diffuse, and specular colors. */

			ambient *= attentuation;
			diffuse *= attentuation;
			specular *= attentuation;


/*	Spot Light

	
	
	
	