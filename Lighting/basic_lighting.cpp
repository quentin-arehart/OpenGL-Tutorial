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

/*	Now we still need the light and fragment positions. Because the light position is a single
	static variable, it can be declared as a uniform in the fragment shader. Afterwards the
	render loop must be updated: */

			uniform vec3 lightPos;

			// Render loop
			lightingShader.setVec3("lightPos", lightPos);

/*	The last thing needed is the fragment position. All the lighting calculations will be done in
	world space so we want a vertex position that is in world space first. We can accomplish this
	by multiplying the vertex position attribute with the model matrix only to transform it into
	world space coordinates. */

			// Vertex shader
			out vec3 fragPos;
			out vec3 Normal;
			void main()
			{
    			gl_Position = projection * view * model * vec4(aPos, 1.0);
				fragPos = vec3(model * vec4(aPos, 1.0));
    			Normal = aNormal;
			} 

			// Frag shader
			in vec3 fragPos;

/*	This in variable will be interpolated from the 3 world position vectors of the triangle to 
	form the fragPos vector that is the per-fragment world position. 
	
	The first thing needed for the lighting calculations is the direction vector between the light
	source and the fragment position. This is calculated by taking the difference vector between
	the two vectors. All relevant vectors should be normalized. */

			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(lightPos - fragPos);

/*	Typically we do not care about the magnitude of a vector or their position, only their
	direction. Almost all calculations are done with unit vectors in order to simplify them.
	Make sure to always normalize the relevant vectors as it is a common mistake to forget to 
	do so.
	
	Afterwards, we must calculate the diffuse impact of the light on the current fragment by
	taking the dot product between the norm and lightDir vectors. The resulting value is then
	multiplied with the light's color to get the diffuse component. 
	
	If the angle between both vectors is greather than 90 degrees the dot product will become
	negative. For this reason we use the max function that returns the highest of both its
	parameters to make sure the diffuse component never becomes negative. 
	
	We then add the ambient and diffuse colors and multiply the result by the color of the 
	object. */

			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * lightColor;
	
			vec3 result = (ambient + diffuse) + objectColor;
			fragColor = vec4(result, 1.0);



/*	Specular Lighting
	
	This lighting is based on the light's direction vector and the object's normal vectors,
	but also on the view direction. It is based on the reflective properties of surfaces. 
	We calculate a reflection vector by reflecting the light direction around the normal 
	vector.
	
	The view vector is the one extra variable needed to perform the specular lighting 
	calculation, which can be done by using the viewer's world space position and the fragment
	position. To get the world space coordinates of the viewer, take the position vector of the
	camera object. */

			uniform vec3 viewPos;
			lightingShader.setVec3("viewPos", camera.Position);

/*	Now we can calculate the specular intensity. First we define a value to give the highlight
	a medium-bright color so that the impact is not too great. */
	
			float specularStrength = 0.5;

//	Next we calculate the view direction and the corresponding reflect vector along the normal axis.
	
			vec3 viewDir = normalize(viewPos - fragPos);
			vec3 reflectDir = reflect(-lightDir, norm
			
/*	The lightDir vector must be negated. The reflect function expects the first vector to 
	point from the light source toward the fragment position, but currently it is pointing 
	the other way around. The second argument expects a normal vector.
	
	Now we calculate the specular component: */
	
			float spec = pow(max(dot(viewDir, reflectDir), 0.0) 32);
			vec3 specular = specularStrength * spec * lightColor;
									  
/*	First we calculate the dot product between the view direction and the reflect direction,
	and then raise it to a power of 32. The 32 is a shininess value of the highlight. A higher
	value more properly reflects the light instead of scattering it around. 
	
	Lastly, add this to the ambient and diffuse components and multiply by the object's color. */
			
			vec3 result = (ambient + diffuse + specular) * objectColor;
			fragColor = vec4(result, 1.0);
			
	



				
				
				
				
				
				
				
				
				
