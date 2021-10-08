#include "rects.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <string>

Rects::Rects(GLint program, glm::vec2 scale, int num_rows, int num_cols) {
	this->program = program;
	this->scale = scale;
	this->num_rows = num_rows;
	this->num_cols = num_cols;
	this->per_rect_dist = 0.01f;

	float per_rect_width = 0.05f;
	float per_rect_height = 0.05f;

	float vertices[] = {
		-per_rect_width / 2.0f, +per_rect_height / 2.0f,
		-per_rect_width / 2.0f, -per_rect_height / 2.0f,
		+per_rect_width / 2.0f, -per_rect_height / 2.0f,
		+per_rect_width / 2.0f, -per_rect_height / 2.0f,
		+per_rect_width / 2.0f, +per_rect_height / 2.0f,
		-per_rect_width / 2.0f, +per_rect_height / 2.0f
	};

	unsigned int num_cells = num_rows * num_cols;
	colors.reserve(num_cells);
	new_colors.reserve(num_cells);

	glm::vec2 start = glm::vec2((-num_cols / 2.0f) * (per_rect_width + per_rect_dist), (+num_rows / 2.0f) * (per_rect_height + per_rect_dist));
	for(unsigned int i = 0; i < num_rows; ++i) {
		float y = start.y -(i * (per_rect_height + per_rect_dist));
		for(unsigned int j = 0; j < num_cols; ++j) {
			float x = start.x + (j * (per_rect_width + per_rect_dist));
			positions.push_back(glm::vec2(x, y));
			colors.push_back(glm::vec3(0, 0, 0));
			new_colors.push_back(glm::vec3(0, 0, 0));
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Rects::update_next_colors() {
	for(unsigned int i = 1; i < num_rows - 1; ++i) {
		for(unsigned int j = 1; j < num_cols - 1; ++j) {
			int sum = 0;
			sum += get_old_color(i - 1, j);
			sum += get_old_color(i + 1, j);
			sum += get_old_color(i, j - 1);
			sum += get_old_color(i, j + 1);
			sum += get_old_color(i - 1, j - 1);
			sum += get_old_color(i + 1, j + 1);
			sum += get_old_color(i - 1, j + 1);
			sum += get_old_color(i + 1, j - 1);

			if(get_old_color(i, j) == 0 && sum == 3)
				set_new_color_on(i, j);
			else if(get_old_color(i , j) == 1 && (sum < 2 || sum > 3))
				set_new_color_off(i, j);
		}
	}

	colors = new_colors;
}

int Rects::get_old_color(int row, int col) {
	int index = num_cols * row + col;
	if(colors[index].x == 0)
		return 0;

	return 1;
}

void Rects::set_old_color_on(int row, int col) {
	int index = num_cols * row + col;
	colors[index] = glm::vec3(1, 1, 1);
}

void Rects::set_new_color_off(int row, int col) {
	int index = num_cols * row + col;
	new_colors[index] = glm::vec3(0, 0, 0);
}

void Rects::set_new_color_on(int row, int col) {
	int index = num_cols * row + col;
	new_colors[index] = glm::vec3(1, 1, 1);
}

void Rects::draw() {
	unsigned int num_cells = num_rows * num_cols;
	for(unsigned int i = 0; i < num_cells; ++i) {
		glUseProgram(program);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(positions[i].x, positions[i].y, 0));
		int model_loc = glGetUniformLocation(program, "u_model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		int color_loc = glGetUniformLocation(program, "inColor");
		glUniform3fv(color_loc, 1, glm::value_ptr(colors[i]));

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
