/*	Each object has a different reaction to light. To stimulate several types of objects, we need
	to define material properties specific to each surface.
	
	We can define a material color for each of the 3 lighting components: ambient, diffuse, 
	specular. Then add a shininess component and it's all set: */

			struct Material {
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
				float shininess;
			};

			uniform Material material;

/*	In the fragment shader we create a struct to store the matieral properties of the surface. 
	Using a struct keeps data more organized. We define the layout of the struct and declare a 
	uniform variable with the newly created struct as its type. 
	
	We created a material struct in the fragment shader so next we have to change the lighting
	calculations to comply with the new material properties. They can be accessed from the material
	uniform: */
	
			void main()
			{    
   				 // ambient
    			vec3 ambient = lightColor * material.ambient;
  	
    			// diffuse 
    			vec3 norm = normalize(Normal);
    			vec3 lightDir = normalize(lightPos - FragPos);
    			float diff = max(dot(norm, lightDir), 0.0);
    			vec3 diffuse = lightColor * (diff * material.diffuse);
    
    			// specular
    			vec3 viewDir = normalize(viewPos - FragPos);
    			vec3 reflectDir = reflect(-lightDir, norm);  
    			float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    			vec3 specular = lightColor * (spec * material.specular);  
        
    			vec3 result = ambient + diffuse + specular;
    			FragColor = vec4(result, 1.0);
			}

//	We also must set the individual uniforms in the render loop:

			lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
			lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
			lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			lightingShader.setFloat("material.shininess", 32.0f);

/*	We set the ambient and diffuse component to the color we want to object to have and set the
	specular component of the object to a medium bright color. Shininess is kept at 32.
	
	However, if the program is run now the object will be much too bright. We must specify 
	intensity vectors for each of the lighting components to adjust the light accordingly: */

			vec3 ambient = vec3(0.1) * material.ambient;
			vec3 diffuse = vec3(1.0) * (diff * material.diffuse);
			vec3 specular = vec3(1.0) * (spec * material.specular);

//	We also want a similar struct for the light properties:

			struct Light {
				vec3 position;
				
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
			};
			
			uniform Light light;

/*	A light source has a different intensity for its ambient, diffuse, and specular components. The 
	ambient light is usually set to a low intensity because we don't want the ambient color to be too
	dominant. The diffuse component of a light source is usually set to the exact color we would like it
	to have, and the specular component is typically kept at vec3(1.0), shining at full intensity. We also
	add the light's position vector to the struct. We then update the fragment shader: */

			vec3 ambient = light.ambient * material.ambient;
			vec3 diffuse = light.diffuse * (diff * material.diffuse);
			vec3 specular = light.specular * (spec * material.specular);

//	Then set the light intensities in the application:
			lightingShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
			lightingShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
			lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 


/*	It is also possible to change the light color over time. A different light color greatly influences the
	object color output. It also has a significant impact on the visual output. 
	
	We can easily change the color over time by changing the light's ambient and diffuse colors via sin
	and glfwGetTime: */

			glm::vec3 lightColor:
			lightColor.x = sin(glfwGetTime() * 2.0f);
			lightColor.y = sin(glfwGetTime() * 0.7f);
			lightColor.z = sin(glfwGetTime() * 1.3f);
  
			glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); 
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 
  
			lightingShader.setVec3("light.ambient", ambientColor);
			lightingShader.setVec3("light.diffuse", diffuseColor);
			
			
				
				
