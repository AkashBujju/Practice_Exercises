#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Pool {
	GLint program;
	GLuint vbo;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec4> current_colors;
	std::vector<glm::vec4> previous_colors;
	std::vector<GLuint> vaos;

	glm::vec2 pool_dims;
	glm::vec2 num_particle_dims;
	glm::vec4 pool_color;
	float particle_gap;
	unsigned int num_particles;

	public:
	Pool(GLint program, glm::vec2 pool_dims, float particle_gap, glm::vec4 pool_color);
	glm::vec4 get_current_colors(int x, int y);
	glm::vec4 get_previous_colors(int x, int y);
	void swap_colors();
	void set_current_colors(int x, int y, glm::vec4 color);
	void update();
	void draw();
};
