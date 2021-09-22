#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Circles {
	GLint program;
	GLuint vbo;
	unsigned int num_vertices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> vaos;
	std::vector<float> radii;

	Circles(GLint program);
	void add(glm::vec3 position, glm::vec3 color, float radius);
	void draw();
};
