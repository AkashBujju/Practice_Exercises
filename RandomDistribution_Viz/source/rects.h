#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Rects {
	GLint program;
	GLuint vbo;
	unsigned int num_vertices;
	glm::vec2 scale;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> vaos;

	Rects(GLint program, glm::vec2 scale);
	void add(glm::vec3 position, glm::vec2 dims, glm::vec3 color);
	void draw();
};
