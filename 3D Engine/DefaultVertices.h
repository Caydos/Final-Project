#ifndef DEFAULT_VERTICES_H
#define DEFAULT_VERTICES_H

static float lineVertices[10] = {
	-.5,.0f,	0.0f, 0.0f, 0.0f,
	.5f,.0f,	0.0f, 0.0f, 0.0f
};

static float triangleVertices[24] = {		/*Colors*/			/* Textures*/
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f,	0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 0.0f,	1.0f, 1.0f
};
static float quadVertices[32] = {
	0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f,		0.5f, 0.5f, 0.5f,	0.0f, 1.0f,   // top left 
};

static float cubeVertices[288] = {
  //back face
   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,
  //front face
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 1.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
  // left face
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
  // right face
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 0.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 1.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
   //bottom
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 6, 0.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 6, 1.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 6, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f
};
static float cubeVertices2[288] = {
	//back face
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,
	//front face
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	// left face
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 3, 0.0f,
	// right face
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 2, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6, 0.0f,
	 //bottom
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 4, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 6, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 6, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 6, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,	1.0f / 6 * 5, 0.0f
};
#endif // !DEFAULT_VERTICES_H