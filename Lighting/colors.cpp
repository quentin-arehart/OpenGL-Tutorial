/*	Colors are represented digitally using a red, green, and blue component abbreviated as RGB. Using a combination
	of those three values can yield nearly any color. For example, to get a coral color, we define a color vector
	as:
		
			glm::vec3 coral(1.0f, 0.5f, 0.31f);
			
	The rules of color reflection apply to graphics. When we define a light source in OpenGL we want to give it a
	color. If we would multiply the light source's color with an object's color value, the resulting color would be
	the reflected color of the object and thus its perceived color:
	
			glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
			glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
			glm::vec3 result = lightColor * toyColor; // = 1.0f, 0.5f, 0.31f
			
	The toy absorbs a large portion of the white light, but reflects several RGB values based on its own color 
	value. We can define an object's color as the amount of each color component it refelcts from a light source.
	If we used a green light, the same principles apply:
	
			glm::vec3 lightColor(0.0f, 1.0f, 0.0f);
			glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
			glm::vec3 result = lightColor * toyColor; // = 0.0f, 0.5f, 0.0
			
	The toy has no red and blue light to absorb and/or reflect. They toy also absorbs half of the light's green
	value and reflects half as well. The coral object suddenly becomes a dark greenish object. 
*/
