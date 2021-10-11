#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct WaveMotion {
	GLint program;
	GLuint vbo;
	GLuint vao;
	std::vector<glm::vec3> scales;
	std::vector<glm::vec3> scales_t;
	std::vector<glm::vec3> positions;
	std::vector<glm::mat4> models;
	int num_rows, num_cols;
	unsigned int num_vertices;

	WaveMotion(GLint program, glm::vec3 scale);
	void update(float t);
	int get_index(int row, int col);
	void draw();
};
