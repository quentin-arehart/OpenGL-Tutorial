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

		
	

