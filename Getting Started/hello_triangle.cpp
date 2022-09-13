#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	unsigned int VBO;
	glGenBuffers(1, &VBO);
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


	
	
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


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
