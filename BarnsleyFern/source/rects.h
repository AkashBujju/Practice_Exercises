#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Points {
	GLint program;
	GLuint vbo;
	unsigned int end_index;
	unsigned int max_index;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> vaos;

	Points(GLint program);
	void add();
	void draw();
};
