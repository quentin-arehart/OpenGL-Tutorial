/*	When talking about camera/viewspace, we are talking about all the vertex coordinates as seen from
	the camera' perspective as the origin of the scene: the view matrix transforms all the world 
	coordinates into view coordinates that are relative to the camera's position and direction. To
	define a camera, we need its position in world space, the direction it's facing, a vector pointing
	to the right, and upwards from the camera. That is to say, a coordinate system with three 
	perpendicular unit axes with the camera's position as the origin.
	
	1. Camera Position
	
	The camera position is a camera in world space that points to the camera's position: */

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

/*	2. Camera Direction
	
	The next vector is the direction at which the camera is pointing. For now, the camera points
	to the origin of the scene (0, 0, 0). If we subtract two vectors from each other, we get a 
	vector that is the difference of these two vectors. Subtracting the camera position vector
	from the scene's origin vector results in the direction vector we want. For the view matrix's
	coordinate system we want its z-axis to be positive, and by convention the camera points 
	towards the negatibe z-axis we want to negate the direction vector. If we switch the 
	subtraction order around we now get a vector pointing at the camera's positive z-axis: */

	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f,);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

/*	3. Right Axis
	
	The right vector represents the positive x-axis of the camera space. To get this vector we
	first specify an up vector that points upwards in world space. Then we do a cross product
	or the up vector and the direction vector. Since the result of a cross product is a vector 
	perpendicular to both vectors, we will get a vector that points in the positive x-axis 
	direction. Switching the cross product order would result in a vector that points in the
	negative x-axis direction. */
	
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

/*	Up Axis
	
	Now that we have both the x-axis and z-axis vector, retrieving the vector that points to
	the camera's positive y-axis is easy: take the cross product of the right and direction
	vector: */

	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

/*										Look At

	Using these camera vectors, we can now create a LookAt matrix that proves very useful for 
	creating a camera. GLM does all the hard work for us: */

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 0,0f, 3.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 1.0f, 0.0f));

/*	The glm::lookAt function requires a position, target, and up vector respectively. We can also
	rotate the camera around the scene. We keep the target of the scene at (0, 0, 0). We use a 
	little trigonometry to create an x and z coordinate each frame that represents a point on a 
	circle and will be used for the camera position. By recalculating the x and y coordinate over
	time we're traversing all the points in a circle and making the camera rotate around the 
	scene. We enlarge this circle by a pre-defined radius and create a new view matrix each from
	using the glfwGetTime function: */

	const float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	glm::mat4 view;
	view = glm::LookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

/*										Walk Around

	To be able to move around the scene ourselves we need to set up a camera system, and it is 
	useful to define those variables at the top of the program, and alter the lookAt function 
	accordingly: */

	glm::vec3 cameraPos = glm::vec3 (0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

//	Now we can add some key commands to processInput 

	void processInput(GLFWwindow *window) {
		...
		const float cameraSpeed = 0.05f;
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		   cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

/*	Whenever we press the WSAD keys, the camera's position is updated accordingly. To move 
	forward or backward we add or substract the direction vector from the position vector scaled
	by some speed value. If we want to move sideways we do a cross product to create a right vector
	and we move along it. The right vector must be normalized otherwise the movement speed will not
	be consistent. 
	
	Currently, we have a constant value for movement speed. In practice, different machines have
	different processing power and the result of that is that some are able to render much more frames
	per second than others. When shipping an application it is important to make sure that it runs 
	the same on all hardware. 
	
	Many applications and games usually keep track of a deltatime variable that stores the time it took
	to render the last frame. Then all velocities are multiplied by this deltaTime value. That is to
	say the velocity of the camera will be balanced out so each user will have the same experience. 
	
	To calculate the deltaTime, we keep track of two global variables: */

	float deltaTime = 0.0f; //	time between current and last frame
	float lastFrame = 0.0f;	//	time of last frame

//	Within each frame calculate the new deltaTime for later use:
	
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

//	Now we can take into account deltaTime when calculating velocities:

	void processInput(GLFWwindow *window) {
		
		float cameraSpeed = 2.5f * deltaTime;
		[...]
	}
		  
//	The camera will now move at a constant 2.5 units per second. 

/*										Look Around

	To look around the scene with the mouse, we have to change the cameraFront vector based on
	the input of the mouse. 
	
	Euler angles are 3 values that can represent any rotation in 3D: pitch, yaw, and roll.
		
			Pitch:	how much we are looking up or down (x axis)
			Yaw:	magnitude we're looking right or left (y axis)
			Roll: 	how much we are rolling (z axis)
	
	Using trignomonetry we can deduce formulas for yaw and pitch.
	
	Given a yaw value we can create a camera direction vector:
	
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)); 
		direction.z = sin(glm::radians(yaw));
		
	Now pitch:
	
		direction.y = sin(glm::radians(pitch));
	
	However, the xz sides are influenced by cos(pitch) so we need to make sure that they are also
	part of the direction vector. Including everything:
	
		direction.x = cos(glm::radians(yaw)) * direction.x = cos(glm::radians(pitch));  
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	To make sure the camera points in the negative z direction by default we can give the yaw
	a default value of a 90 degree clockwise rotation. Positive degrees rotate counterclockwise.
	
	yaw = -90.0f
	
	
										Mouse Input
										
	Yaw and pitch values are obtained from a mouse, controller, joystick, etc. Horizontal 
	movement affects the yaw and vertical movement affects the pitch. The idea is to store the
	last frame's mouse positions and calculate in the current frame how much the values changed.
	
	First we tell GLFW to hide the cursor and capture it:
	
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	To calculate the pitch and yaw values, we need to tell GLFW to listen to mouse-movement events. 
	
		void mouse_callback(GLFWwindow *window, double xpos, double ypos);
		
	xpos and ypos represent the current mouse position. 
	
		glfwSetCursorPosCallback(window, mouse_callback);
		
	Calculate the offset of the mouse since the last frame. Initialize the mouse to the center of 
	the screen.
		
		float lastX = 400, lastY = 300;
		
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		
		const float senstitivty = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		
	Add the offset values to the globally declared pitch and yaw values.
	
		yaw += xoffset;
		pitch += yoffset;
	
*/

