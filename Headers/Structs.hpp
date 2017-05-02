#ifndef STRUCTS_H_GUARD
#define STRUCTS_H_GUARD

#include <glad/glad.h>
#include <glm/glm.hpp>

struct bufferContainer {
	GLuint scene_vao;
	GLuint position_vbo;
	GLuint colors_vbo;
	GLuint faces_ibo;
	GLuint edges_ibo;
	GLuint normals_vbo;
};

struct dynamics_comp {
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    glm::vec3 mass;
    glm::vec3 force;
};

#endif // STRUCTS_H_GUARD