#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 450 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0\n;"
	"}\0";

const char *fragmentShaderSource = "#version 450 core\n"
	"out vec4 fragColor;\n"
	"void main()\n"
	"{\n"
	"	fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

//	Compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
/*	glShaderSource takes the shader object to compile as the first parameter, the second specifies how many 
	strings we are passing, and the third is the source code of the vertex shader. We can also check for errors: */
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader!\n" << infoLog << std::endl;
	}
	
//	Compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, 512, NULL, infoLog);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader!\n" << infoLog << std::endl;
	}
	
//	Link shaders to use them after compilation
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
//	Check for errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

//	Clean up shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
//	Define the triangle's coordinates in normalized device coordinates using a float array
//	Each vertex has a z coordinate of 0 to make it look like it is 2D
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

/*	A VBO can store a large number of vertices in the GPUs memory. This buffer has a unique ID
	corresponding to that buffer. We can generate one with a buffer ID using the glGenBuffers
	function. */	
	unsigned int VBO, VAO;
	glGenBuffers(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
/*	The buffer type of a VBO is GL_ARRAY_BUFFER. OpenGL lets us bind to several buffers at once
	provided they have a different buffer type. We can bind the newly created buffer to the
	GL_ARRAY_BUFFER target with the glBindBuffer function: */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
/*	Any buffer calls made on the array buffer will be used to configure the currently bound 
	buffer, VBO. Then we can make a call to the glBufferData function that copies the previously
	defined vertex data into the buffer's memory. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
/*	The first parameter is the type of buffer we want to copy data into. The second parameter
	specifies the size of the data we want to pass to the buffer. The third parameter is the 
	actual data to be sent. The last parameter specifies how we want the graphics card to manage
	the given data:
			GL_STREAM_DATA:
				the data is set only once and used by the GPU at most a few times
			GL_STATIC_DRAW:
				the data is set only once and used many times
			GL_DYNAMIC_DRAW:
				the data is changed a lot and used many times
	Now we will create the shaders. For this file the shaders will be hard coded as strings at 
	the top of the file. */

//	Linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
/*	The first parameter specifices which vertex attribute we want to configure.
	The second parameter specifies the size of the vertex attribute. A vec3 is composed of 3 values. 
	The third parameter specifies the type of data being passed. 
	The fourth parameter specifies if we want the data to be normalized.
	The fifth argument is the stride and tells us the space between consecutive vertex attributes (3 times the size of a float away).
	The last parameter is of type void* and requires a cast; it is the offset of where the data begins in the buffer. */
	
//	Vertex Array Object
/*	A VAO can be bound just like a VBO. OpenGL requires a VAO otherwise it might not draw anything.
	A VAO stores:
			Calls to glEnableVertexAttribArray or glDisableVertexAttribArray
			Vertex attribute configurations via glVertexAttribPointer
			VBOs assosicated with vertex attributes */

	
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
//	Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3)
//	Where 0 is the starting index of the vertex array and 3 specifies the amount of vertices to draw
		


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   
    glViewport(0, 0, width, height);
}
