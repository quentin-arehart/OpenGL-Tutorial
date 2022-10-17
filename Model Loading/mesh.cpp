/* Let's define a mesh class of our own. At minimum, a mesh should need a set of vertices, where
	each vertex contains a position vector, a normal vector, and a texture coordinate vector. A
	mesh shpild also contain indices for indexed drawing, and material data in the form of textures
	(diffuse/specular maps): */

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type; // the id of the texture and its type
};

// Define the structure of the mesh class:

class Mesh {
	public: 
		// mesh data
		vector<Vertex>			vertices;
		vector<unsigned int>	indices;
		vector<Texture>			textures;
	
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			
			setupMesh();
		}
			
	/*	Before rendering we need to bind the appropriate textures before calling glDrawElements. We will call
		each diffuse texture texture_diffuseN and each specular texture texture_specularN where N is any number
		from 1 to the maximum number of textures allowed. The resulting code becomes: */
		void Draw(Shader &shader) {
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			for(unsigned int i = 0; i < textures.size(); i++) 
			{
				glActiveTexture(GL_TEXTURE0 + i);
				
				string number;
				string name = textures[i].type;
				if (name == "texture_diffuse") 
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")	
					number = std::to_string(specularNr++);
						 
				
				shader.setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
			glActiveTexture(GL_TEXTURE0);
				
			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	/*	First we calculate the N-component per texture type and concatenate it to the texture's type string to 
		get the appropriate uniform name. We then locate the appropriate sampler, give it the location value
		to correspond with the currently active texture unit, and bind the texture. This is also the reason why
		we need the shader in the Draw function. We also added "material." to the resulting uniform name because
		we usually store the textures in a material struct. 
	*/
			
						
					
	
	
	private:
		// render data
		unsigned int VAO, VBO, EBO;
			
		
	
/*	In the constructor we give mesh all the necessary data, we inititalize the buffers in the setupMesh function
	and finally draw the mesh via the Draw function. We give a shader to the draw function; by passing the 
	shader to the mesh we can set several uniforms before drawing. 
	
	Now we will set up the appropriate buffers and specify the vertex shader layout via vertex attribute 
	pointers. */

	void setupMesh() {
	
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	
		// vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	
		glBindVertexArray(0);
	
	}	
};
	

	
	
	
	
