/*	When talking about camera/viewspace, we are talking about all the vertex coordinates as seen from
	the camera' perspective as the origin of the scene: the view matrix transforms all the world 
	coordinates into view coordinates that are relative to the camera's position and direction. To
	define a camera, we need its position in world space, the direction it's facing, a vector pointing
	to the right, and upwards from the camera. That is to say, a coordinate system with three 
	perpendicular unit axes with
	
*/
