/*	Lighting in OpenGL is based on approximations of reality using simplified models that are much easier to 
	process and look relatively similar. These models are based on the physics of light. One such model is known
	as the Phong lighting model. The major building blocks of this model consist of 3 components: ambient, 
	diffuse, ad specular lighting. 
	
		Amibient:
			even when it is dark there is usually some light somewhere in the world so objects are almost never
			completely dark. To simulate this we use an ambient lighting onstant that always gives the object
			some color.
		Diffuse:
			simulates the directional impact a light object has on an object. The more a part of an object faces
			the light source, the brighter it becomes. 
		Specular:
			simulates the bright spot of a light that appears on shiny objects. Specular highlights are more 
			inclined to the color of the light than the color of the object. 
		
	To create visually interesting scenes we want to at least stimulate these three lighting components. 


	Ambient Lighting
	
	Light can reflect on other surfaces and have an indirect impact on the lighting of an object. Algorithms that
	take this into account are called global illumination aglorithms, but they are expensive and complicated to 
	caluclate. 
	
	To circumvent this we can use a simplistic model of global illumination called ambient lighting. We use a 
	small constant (light) color that we add to the final resulting color of the object's fragments, thus making
	it look like there is always some scattered light without a direct light source. 
	
	Adding ambient lighting is very easy; we take the light's color, multiply it with a small constant ambient
	factor, multiply this with the object's color, and use that as the fragment's color in the cube object's 
	shader: */

			void main() {
				float ambientStrength = 0.1;
				vec3 ambient = ambientStrength * lightColor;
				
				vec3 result = ambient * objectColor;
				fragColor = vec4(result, 1.0);
			}


/*	Diffuse Lighting

	This lighting gives the object more brightness the closer its fragments are aligned to the light rays from
	a light source. We need to measure at what angle the light ray touches the fragment. If the light ray is 
	perpendicular to the object's surface the light has the greatest intensity. To measure this angle, we use
	something called a normal vector, or a vector that is perpendicular to the fragment's surface. The angle 
	between the two vectors can then be calculated with the dot product. 
	
	The lower the angle between two unit fectors, the more the dot product is inclined towards a value of 1. 
	When the angle between the vectors reaches 90 degrees, the dot product becomes 0. 
	
	To get only the cosine of the angle between both vectors, we work with unit vectors. Therefore all vectors
	need to be normalized otherwise the dot product returns more than the cosine. 
	
	The resulting dot product thus returns a scalar that we can use to calculate the the light's impact on the 
	fragment's color, resulting in differently lit fragments based on their orientation towards the light. 
	
	We will need to calculate:
	
		Normal vectors
		The directed light ray: the difference vector between the light's position and the fragment's position.
		
	
	Normal Vectors
	
	3D cubes are not complicated so we can manually add the normal vectors to the vertex data. We also need to
	update the vertex shader: */

			#version 460 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aNormal;

/*	We also need to update the vertex attribute pointers as well. The light source's cube uses the same vertex	
	array for its vertex data. We only want to use the first 3 floats for each vertex and ignore the last 3
	floats so we only need to update the stride parameter to 6 times the size of a float and we are done: */
	
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

//	Forward the normal vectors from the vertex shader to the fragment shader

			out vec3 Normal;
			void main()
			{
    			gl_Position = projection * view * model * vec4(aPos, 1.0);
    			Normal = aNormal;
			} 

			// Fragment
			in vec3 Normal;
			





				
				
				
				
				
				
				
				
				
