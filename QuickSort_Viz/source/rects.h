#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Number {
	GLuint vao;
	float number;
	float x_position;
};

struct Rects {
	GLint program;
	GLuint vbo;

	float per_width;
	std::vector<Number> numbers;

	public:
	void init(GLint program, std::vector<glm::vec3> colors, std::vector<float> numbers);
	void draw();
	void swap_positions(int index_0, int index_1);
	void Rects::print_positions();

	private:
	void add_number(float number, float x_pos, float height, glm::vec3 color);
};
