/* 	Now we are going to elaborate on the depth buffer to see how it actually determines if a 	
	fragment is in front. 
	
	The depth buffer is a buffer that stores information per fragment and has the same width and 
	height as the color buffer. It is automatically created by the windowing system and stores its
	depth values as 16, 24, or 32 bit floats. In most systems a precision of 24 bits is used. 
	
	When depth testing is enabled, OpenGL tests the depth value of a fragment against the content 
	of the depth buffer. OpenGL performs a depth test and if this test passes, the fragment is 
	rendered and the depth buffer is updated with the new depth value. If it fails, the fragment is
	discarded. 
	
	Depth testing is done in screen space after the fragment shader has run. The screen space 
	coordinates relate directly to the viewport defined by the glViewport function and can be 
	accessed via the gl_FragCoord variable in the fragment shader. The x and y componets of 
	gl_FragCoord represent the fragment's screen space coordinates. This variable also contains a 
	z-component which contains the depth value of the fragment. Depth testing is disabled by 
	default so to enable depth testing we need to enable it with the GL_DEPTH_TEST option:
	
			glEnable(GL_DEPTH_TEST);
			
	Once enabled, OpenGL stores fragments their z values in the depth buffer if they passed the 
	depth test and discards fragments that failed. If depth testing is enabled, you should clear
	the depth buffer before each frame using GL_DEPTH_BUFFER_BIT.
	
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
	There are certain scenarios where you want to perform the depth test on all fragments and 
	discard them accordingly, but not update the depth buffer. Basically, temporarily using a
	read only depth buffer. OpenGL allows us to disable writing to the depth buffer by setting its
	depth mask to GL_FALSE.
	
			glDepthMask(GL_FALSE);
			
	

	Depth Test Function
	
	OpenGL allows us to modify the comparison operators it uses for the depth test. This lets
	us control when OpenGL should pass or discard fragments and when to update the depth buffer.
	We can set the comparison operator by calling glDepthFunc.
	
			glDepthFunc(GL_LESS);
	
	The function accepts several comparison operators:
	
			GL_ALWAYS
				the depth test always passes
			GL_NEVER
				the depth test never passes
			GL_LESS
				passes if the fragment's depth value is less than the stored depth value
			GL_EQUAL
				passes if the fragment's depth value is equal to the stored depth value
			GL_LEQUAL
				passes if the fragment's depth value is less than or equal to the stored depth value
			GL_GREATER
				passes if the fragment's depth value is greater than the stored depth value
			GL_NOTEQUAL
				passes if the fragment's depth value is not equal to the stored depth value
			GL_GEQUAL
				passes if the fragment's depth value is greater than or equal to the stored depth value
	
	By default, the depth function GL_LESS is used that discards all the fragments that have a 
	depth higher than or equal to the current depth buffer's value.

	
	Depth Value Precision
	
	The depth buffer contains depth values between 0.0 and 1.0 and it compares its content with
	the z values of all the objects in the scene as seen from the viewer. These z values can be
	any value between the projection-frustum's near and far plane. We need some way to transform
	these view-space z values to the range of [0, 1] and one way is to do so linearly:
	
			F(depth) (z - near) / (far - near)
			
	Here, near and far are the values provided to the projection matrix to set the visible 
	frustum. However a linear depth buffer like this is almost never used because of projection
	properties a non-linear depth equation is used that is proportional to 1/z. The result is 
	that we get enormous precision when z is small and much less precision when z is far away.
	
	A better equation to use would be:
	
			F(depth) = ((1/z) - (1/near)) / ((1/far) - (1/near))
			
	The values in the depth buffer are not linear in clip-space. They are linear in view space 
	before the projection matrix is applied. A value of 0.5 in the depth buffer does not mean that
	the pixel's z value is halfway in the frustum; the z value of the vertex is quite close to the
	near plane. 


	Visualzing the Depth Buffer
	
	
	We can out put the depth value of the fragment as a color and display the depth values of 
	all fragments in the scene:
	
			void main() 
			{
				fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
			}
			
	Non-linear depth values have a high precision for small z values and a low precision for 
	high values. The depth value of the fragment increases rapidly over distance so almost
	all the vertices have a value close to 1.0. We can transform the non-linear depth values
	back into linear values. We reverse the process of projection. We re-transform the
	depth values from the range [0, 1] to normalized device coordinates [-1, 1]. Then we reverse
	the non-linear equation and apply this inversed equation to the resulting depth value:
	
	First transform the depth value to NDC:
	
			float ndc = depth * 2.0 - 1.0;
			
	Then take the resulting ndc value and apply the inverse transformation:
	
			float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));
			
	The fragment shader then looks like:
	
			out vec4 fragColor
			
			float near = 0.1;
			float far = 100.0;
			
			float LinearizeDepth(float depth)
			{
				float z = depth * 2.0 - 1.0; // back to NDC
				return (2.0 * near * far) / (far + near -z * (far - near));
			}
			
			void main()
			{
				float depth = LinearizeDepth(gl_FragCoord.z) / far;
				fragColor = vec4(vec3(depth), 1.0);
			}
			
	The linearized depth values range from near to far so most of its values will be above 1.0 
	and displayed as completely white. By dividing the linear depth value by far in the main 
	function we convert the linear depth value to the range [0, 1]. Then the scene gradually 
	becomes brighter the closer the fragments are to the projection frustum's far plane, which 
	works better for visualization purposes. 
	
	
	Z-Fighting
	
	A common visual artifact that may occur when two planes or triangles are so closely aligned 
	to each other than the depth buffer does not have enough precision to figure out which one of 
	the two shapes is in front of the other. The result is that the two shapes continually seem to 
	switch order which causes glitchy patterns. This is called z-fighting because it looks like the
	two shapes are fighting to stay on top. Z-fighting can't completely be prevented but there are 
	a few tricks that will mitigate z-fighting.
	
	
	Preventing Z-Fighting
	
	The first and most important trick is to never place objects too close to each other in a way
	that some of their triangles closely overlap. By creating a small offset between two objects
	you can completely remove z-fighting between them. 
	
	A second trick is to set the near plane as far as possible. If the near plane is far from 
	the viewer, there will be significantly greater precision over the entire frustum range. 
	However, setting the near plane too far could cause clipping of near objects so it is a matter
	of tweaking and experimentation. 
	
	Another great trick at the cost of some performance is to use a higher precision depth buffer.
	Most modern GPUs support 32 bit depth buffers, increasing the precision by a significant 
	amount. */
			



