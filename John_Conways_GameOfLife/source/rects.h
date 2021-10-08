#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Rects {
	GLint program;
	GLuint vbo;
	GLuint vao;
	glm::vec2 scale;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> new_colors;
	int num_rows, num_cols;
	float per_rect_dist;

	Rects(GLint program, glm::vec2 scale, int width, int height);
	void update_next_colors();
	void set_new_color_off(int row, int col);
	void set_new_color_on(int row, int col);
	void set_old_color_on(int row, int col);
	int get_old_color(int row, int col);
	void draw();
};
