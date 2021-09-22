#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Lines {
	GLint program;
	GLuint vbo;
	std::vector<GLuint> vaos;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<float> lengths;
	std::vector<float> angles;

	Lines(GLint program);
	void add(glm::vec3 position, glm::vec3 color, float length, float angle);
	void draw();
};
