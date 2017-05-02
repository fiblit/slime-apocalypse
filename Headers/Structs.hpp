#ifndef STRUCTS_H_GUARD
#define STRUCTS_H_GUARD

// Don't know how we bring these in from the top level
#include <glad/glad.h>// glad: an OpenGL function loader: https://github.com/Dav1dde/glad

typedef struct {
	GLuint scene_vao;
	GLuint position_vbo;
	GLuint colors_vbo;
	GLuint faces_ibo;
	GLuint edges_ibo;
	GLuint normals_vbo;
} bufferContainer;

#endif // STRUCTS_H_GUARD