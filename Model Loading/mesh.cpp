/* Let's define a mesh class of our own. At minimum, a mesh should need a set of vertices, where
	each vertex contains a position vector, a normal vector, and a texture coordinate vector. A
	mesh shpild also contain indices for indexed drawing, and material data in the form of textures
	(diffuse/specular maps): */

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoords;
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
	
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		void Draw(Shader &shader);
	
	private:
		// render data
		unsigned int VAO, VBO, EBO;
			
		void setupMesh();
};
	
