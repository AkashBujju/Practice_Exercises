#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Terrain {
	GLint program;
	GLuint vbo;
	GLuint vao;
	glm::vec2 scale;
	int num_rows, num_cols;
	unsigned int num_vertices;

	Terrain(GLint program, glm::vec2 scale, int width, int height);
	void draw();
};
